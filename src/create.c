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

HRESULT WINAPI d3d9_CreateDevice(
    IDirect3D9 *This,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice9 **ppReturnedDeviceInterface)
{
    // TODO(iceboy): Not implemented.
    return orig_d3d9_CreateDevice(
        This,
        Adapter,
        DeviceType,
        hFocusWindow,
        BehaviorFlags,
        pPresentationParameters,
        ppReturnedDeviceInterface);
}

IDirect3D9 *WINAPI Direct3DCreate9(UINT SDKVersion)
{
    extern IDirect3D9 *(WINAPI *orig_Direct3DCreate9)(UINT SDKVersion);
    IDirect3D9 *d3d9 = orig_Direct3DCreate9(SDKVersion);
    IDirect3D9Vtbl *d3d9_vtbl = d3d9->lpVtbl;

    if (!orig_d3d9_CreateDevice) {
        orig_d3d9_CreateDevice = d3d9_vtbl->CreateDevice;
    }
    mp_assign((void **)&d3d9_vtbl->CreateDevice, d3d9_CreateDevice);
    return d3d9;
}
