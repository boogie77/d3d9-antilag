#include <d3d9.h>
#include <stdint.h>
#include "protect.h"

#define TARGET_LATENCY_TICKS (30000)

static HRESULT (WINAPI *orig_d3d9_CreateDevice)(
    IDirect3D9 *This,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice9 **ppReturnedDeviceInterface);

static HRESULT (WINAPI *orig_d3d9_device_Present)(
    IDirect3DDevice9 *This,
    const RECT *pSourceRect,
    const RECT *pDestRect,
    HWND hDestWindowOverride,
    const RGNDATA *pDirtyRegion);

static HRESULT (*obs_get_backbuffer)(
    IDirect3DDevice9 *device,
    IDirect3DSurface9 **surface);

static void (*obs_d3d9_capture)(
    IDirect3DDevice9 *device,
    IDirect3DSurface9 *surface);

static LARGE_INTEGER perf_freq;
static LARGE_INTEGER start_time;
static IDirect3DDevice9 *device;
static IDirect3DDevice9Vtbl *device_vtbl;

#define UPDATE_ELAPSED_TICKS() \
    do { \
        QueryPerformanceCounter(&end_time); \
        elapsed_ticks = (end_time.QuadPart - start_time.QuadPart) * 10000000 / perf_freq.QuadPart; \
    } while (0)

static inline void handle_obs(uintptr_t reloc_offset)
{
    MP_PROTECT_BEGIN(orig_d3d9_device_Present, 5);
    *(uint32_t *)orig_d3d9_device_Present = 0x8b55ff8b;
    *(uint8_t *)((uint32_t)orig_d3d9_device_Present + 4) = 0xec;
    MP_PROTECT_END(orig_d3d9_device_Present, 5);

    obs_get_backbuffer = (void *)(0x5c667440 + reloc_offset);
    obs_d3d9_capture = (void *)(0x5c666680 + reloc_offset);
    volatile uint8_t *obs_stop_loop = (void *)(0x5c6a08ad + reloc_offset);
    *obs_stop_loop = 1;
}

HRESULT WINAPI d3d9_swap_chain_Present(
    IDirect3DSwapChain9 *This,
    const RECT *pSourceRect,
    const RECT *pDestRect,
    HWND hDestWindowOverride,
    const RGNDATA *pDirtyRegion,
    DWORD dwFlags)
{
    HRESULT hr = orig_d3d9_device_Present(
        device,
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion);
    LARGE_INTEGER end_time;
    int64_t elapsed_ticks;
    UPDATE_ELAPSED_TICKS();
    int64_t expected_ticks = 133333 + (elapsed_ticks - TARGET_LATENCY_TICKS) / 5;

    // OBS compatibility.
    if (obs_get_backbuffer) {
        IDirect3DSurface9 *backbuffer;
        obs_get_backbuffer(device, &backbuffer);
        obs_d3d9_capture(device, backbuffer);
        backbuffer->lpVtbl->Release(backbuffer);
    } else if (*(uint16_t *)orig_d3d9_device_Present == 0xf9eb) {
        uintptr_t jmp_slot = (uintptr_t)orig_d3d9_device_Present - 4;
        uintptr_t jmp_base = (uintptr_t)orig_d3d9_device_Present;
        uintptr_t jmp_address = jmp_base + *(uintptr_t *)jmp_slot;
        uintptr_t reloc_offset = jmp_address - 0x5c667550;
        handle_obs(reloc_offset);
    } else if (*(uint8_t *)orig_d3d9_device_Present == 0xe9) {
        uintptr_t jmp_slot = (uintptr_t)orig_d3d9_device_Present + 1;
        uintptr_t jmp_base = (uintptr_t)orig_d3d9_device_Present + 5;
        uintptr_t jmp_address = jmp_base + *(uintptr_t *)jmp_slot;
        uintptr_t reloc_offset = jmp_address - 0x5c667550;
        handle_obs(reloc_offset);
    }

    UPDATE_ELAPSED_TICKS();
    if (elapsed_ticks <= expected_ticks) {
        if (expected_ticks - elapsed_ticks >= 30000) {
            Sleep((expected_ticks - elapsed_ticks) / 10000 - 2);
        }
        do {
            UPDATE_ELAPSED_TICKS();
        } while (elapsed_ticks <= expected_ticks);
    }
    QueryPerformanceCounter(&start_time);
    return hr;
}

HRESULT WINAPI d3d9_CreateDevice(
    IDirect3D9 *This,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice9 **ppReturnedDeviceInterface)
{
    IDirect3DSwapChain9 *swap_chain;
    IDirect3DSwapChain9Vtbl *swap_chain_vtbl;
    HRESULT hr = orig_d3d9_CreateDevice(
        This,
        Adapter,
        DeviceType,
        hFocusWindow,
        BehaviorFlags,
        pPresentationParameters,
        &device);
    if (FAILED(hr)) {
        return hr;
    }

    device_vtbl = device->lpVtbl;
    orig_d3d9_device_Present = device_vtbl->Present;
    device_vtbl->GetSwapChain(device, 0, &swap_chain);
    swap_chain_vtbl = swap_chain->lpVtbl;
    MP_PROTECT_BEGIN(swap_chain_vtbl, sizeof(*swap_chain_vtbl));
    swap_chain_vtbl->Present = d3d9_swap_chain_Present;
    MP_PROTECT_END(swap_chain_vtbl, sizeof(*swap_chain_vtbl));
    *ppReturnedDeviceInterface = device;
    return S_OK;
}

IDirect3D9 *WINAPI Direct3DCreate9(UINT SDKVersion)
{
    extern IDirect3D9 *(WINAPI *orig_Direct3DCreate9)(UINT SDKVersion);
    IDirect3D9 *d3d9;
    IDirect3D9Vtbl *d3d9_vtbl;

    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    QueryPerformanceFrequency(&perf_freq);

    d3d9 = orig_Direct3DCreate9(SDKVersion);
    d3d9_vtbl = d3d9->lpVtbl;
    orig_d3d9_CreateDevice = d3d9_vtbl->CreateDevice;
    MP_PROTECT_BEGIN(d3d9_vtbl, sizeof(*d3d9_vtbl));
    d3d9_vtbl->CreateDevice = d3d9_CreateDevice;
    MP_PROTECT_END(d3d9_vtbl, sizeof(*d3d9_vtbl));
    return d3d9;
}
