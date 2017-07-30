#include <d3d9.h>
#include "protect.h"

static HRESULT (WINAPI *orig_d3d9_CreateDevice)(
    IDirect3D9 *This,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice9 **ppReturnedDeviceInterface);

static ULONG (WINAPI *orig_d3d9_device_Release)(
    IDirect3DDevice9 *This);

static HRESULT (WINAPI *orig_d3d9_device_Reset)(
    IDirect3DDevice9 *This,
    D3DPRESENT_PARAMETERS *pPresentationParameters);

static HRESULT (WINAPI *orig_d3d9_device_Present)(
    IDirect3DDevice9 *This,
    const RECT *pSourceRect,
    const RECT *pDestRect,
    HWND hDestWindowOverride,
    const RGNDATA *pDirtyRegion);

ULONG WINAPI d3d9_device_Release(
    IDirect3DDevice9 *This)
{
    return orig_d3d9_device_Release(This);
}

HRESULT WINAPI d3d9_device_Reset(
    IDirect3DDevice9 *This,
    D3DPRESENT_PARAMETERS *pPresentationParameters)
{
    return orig_d3d9_device_Reset(This, pPresentationParameters);
}

HRESULT WINAPI d3d9_device_Present(
    IDirect3DDevice9 *This,
    const RECT *pSourceRect,
    const RECT *pDestRect,
    HWND hDestWindowOverride,
    const RGNDATA *pDirtyRegion)
{
    return orig_d3d9_device_Present(
        This,
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion);
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
    IDirect3DDevice9 *device;
    IDirect3DDevice9Vtbl *device_vtbl;
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
    orig_d3d9_device_Release = device_vtbl->Release;
    orig_d3d9_device_Reset = device_vtbl->Reset;
    orig_d3d9_device_Present = device_vtbl->Present;

    MP_PROTECT_BEGIN(device_vtbl, sizeof(*device_vtbl));
    device_vtbl->Release = d3d9_device_Release;
    device_vtbl->Reset = d3d9_device_Reset;
    device_vtbl->Present = d3d9_device_Present;
    MP_PROTECT_END(device_vtbl, sizeof(*device_vtbl));
    *ppReturnedDeviceInterface = device;
    return S_OK;
}

IDirect3D9 *WINAPI Direct3DCreate9(UINT SDKVersion)
{
    extern IDirect3D9 *(WINAPI *orig_Direct3DCreate9)(UINT SDKVersion);
    IDirect3D9 *d3d9 = orig_Direct3DCreate9(SDKVersion);
    IDirect3D9Vtbl *d3d9_vtbl = d3d9->lpVtbl;

    orig_d3d9_CreateDevice = d3d9_vtbl->CreateDevice;
    MP_PROTECT_BEGIN(d3d9_vtbl, sizeof(*d3d9_vtbl));
    d3d9_vtbl->CreateDevice = d3d9_CreateDevice;
    MP_PROTECT_END(d3d9_vtbl, sizeof(*d3d9_vtbl));
    return d3d9;
}
