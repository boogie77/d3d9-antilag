static inline void load_orig_ptrs(HMODULE orig_d3d9) {

    extern FARPROC orig_D3DPERF_BeginEvent;
    orig_D3DPERF_BeginEvent = GetProcAddress(orig_d3d9, "D3DPERF_BeginEvent");

    extern FARPROC orig_D3DPERF_EndEvent;
    orig_D3DPERF_EndEvent = GetProcAddress(orig_d3d9, "D3DPERF_EndEvent");

    extern FARPROC orig_D3DPERF_GetStatus;
    orig_D3DPERF_GetStatus = GetProcAddress(orig_d3d9, "D3DPERF_GetStatus");

    extern FARPROC orig_D3DPERF_QueryRepeatFrame;
    orig_D3DPERF_QueryRepeatFrame = GetProcAddress(orig_d3d9, "D3DPERF_QueryRepeatFrame");

    extern FARPROC orig_D3DPERF_SetMarker;
    orig_D3DPERF_SetMarker = GetProcAddress(orig_d3d9, "D3DPERF_SetMarker");

    extern FARPROC orig_D3DPERF_SetOptions;
    orig_D3DPERF_SetOptions = GetProcAddress(orig_d3d9, "D3DPERF_SetOptions");

    extern FARPROC orig_D3DPERF_SetRegion;
    orig_D3DPERF_SetRegion = GetProcAddress(orig_d3d9, "D3DPERF_SetRegion");

    extern FARPROC orig_DebugSetLevel;
    orig_DebugSetLevel = GetProcAddress(orig_d3d9, "DebugSetLevel");

    extern FARPROC orig_DebugSetMute;
    orig_DebugSetMute = GetProcAddress(orig_d3d9, "DebugSetMute");

    extern FARPROC orig_Direct3DCreate9;
    orig_Direct3DCreate9 = GetProcAddress(orig_d3d9, "Direct3DCreate9");

    extern FARPROC orig_Direct3DShaderValidatorCreate9;
    orig_Direct3DShaderValidatorCreate9 = GetProcAddress(orig_d3d9, "Direct3DShaderValidatorCreate9");

    extern FARPROC orig_PSGPError;
    orig_PSGPError = GetProcAddress(orig_d3d9, "PSGPError");

    extern FARPROC orig_PSGPSampleTexture;
    orig_PSGPSampleTexture = GetProcAddress(orig_d3d9, "PSGPSampleTexture");

}