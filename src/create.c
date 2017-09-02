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

static LARGE_INTEGER perf_freq;
static LARGE_INTEGER start_time;
static IDirect3DDevice9 *device;
static IDirect3DDevice9Vtbl *device_vtbl;

HRESULT WINAPI d3d9_swap_chain_Present(
    IDirect3DSwapChain9 *This,
    const RECT *pSourceRect,
    const RECT *pDestRect,
    HWND hDestWindowOverride,
    const RGNDATA *pDirtyRegion,
    DWORD dwFlags)
{
    HRESULT hr = device_vtbl->Present(
        device,
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion);
    LARGE_INTEGER end_time;
    QueryPerformanceCounter(&end_time);
    int64_t elapsed_ticks = (end_time.QuadPart - start_time.QuadPart) * 10000000 / perf_freq.QuadPart;
    int64_t expected_ticks = 133333 + (elapsed_ticks - TARGET_LATENCY_TICKS) / 4;
    while (elapsed_ticks < expected_ticks) {
        QueryPerformanceCounter(&end_time);
        elapsed_ticks = (end_time.QuadPart - start_time.QuadPart) * 10000000 / perf_freq.QuadPart;
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
    QueryPerformanceFrequency(&perf_freq);

    d3d9 = orig_Direct3DCreate9(SDKVersion);
    d3d9_vtbl = d3d9->lpVtbl;
    orig_d3d9_CreateDevice = d3d9_vtbl->CreateDevice;
    MP_PROTECT_BEGIN(d3d9_vtbl, sizeof(*d3d9_vtbl));
    d3d9_vtbl->CreateDevice = d3d9_CreateDevice;
    MP_PROTECT_END(d3d9_vtbl, sizeof(*d3d9_vtbl));
    return d3d9;
}
