section .text

global _D3DPERF_BeginEvent
_D3DPERF_BeginEvent:
    jmp dword [_orig_D3DPERF_BeginEvent]

global _D3DPERF_EndEvent
_D3DPERF_EndEvent:
    jmp dword [_orig_D3DPERF_EndEvent]

global _D3DPERF_GetStatus
_D3DPERF_GetStatus:
    jmp dword [_orig_D3DPERF_GetStatus]

global _D3DPERF_QueryRepeatFrame
_D3DPERF_QueryRepeatFrame:
    jmp dword [_orig_D3DPERF_QueryRepeatFrame]

global _D3DPERF_SetMarker
_D3DPERF_SetMarker:
    jmp dword [_orig_D3DPERF_SetMarker]

global _D3DPERF_SetOptions
_D3DPERF_SetOptions:
    jmp dword [_orig_D3DPERF_SetOptions]

global _D3DPERF_SetRegion
_D3DPERF_SetRegion:
    jmp dword [_orig_D3DPERF_SetRegion]

global _DebugSetLevel
_DebugSetLevel:
    jmp dword [_orig_DebugSetLevel]

global _DebugSetMute
_DebugSetMute:
    jmp dword [_orig_DebugSetMute]

global _Direct3DShaderValidatorCreate9
_Direct3DShaderValidatorCreate9:
    jmp dword [_orig_Direct3DShaderValidatorCreate9]

global _PSGPError
_PSGPError:
    jmp dword [_orig_PSGPError]

global _PSGPSampleTexture
_PSGPSampleTexture:
    jmp dword [_orig_PSGPSampleTexture]


section .bss

global _orig_D3DPERF_BeginEvent
_orig_D3DPERF_BeginEvent: resd 1

global _orig_D3DPERF_EndEvent
_orig_D3DPERF_EndEvent: resd 1

global _orig_D3DPERF_GetStatus
_orig_D3DPERF_GetStatus: resd 1

global _orig_D3DPERF_QueryRepeatFrame
_orig_D3DPERF_QueryRepeatFrame: resd 1

global _orig_D3DPERF_SetMarker
_orig_D3DPERF_SetMarker: resd 1

global _orig_D3DPERF_SetOptions
_orig_D3DPERF_SetOptions: resd 1

global _orig_D3DPERF_SetRegion
_orig_D3DPERF_SetRegion: resd 1

global _orig_DebugSetLevel
_orig_DebugSetLevel: resd 1

global _orig_DebugSetMute
_orig_DebugSetMute: resd 1

global _orig_Direct3DCreate9
_orig_Direct3DCreate9: resd 1

global _orig_Direct3DShaderValidatorCreate9
_orig_Direct3DShaderValidatorCreate9: resd 1

global _orig_PSGPError
_orig_PSGPError: resd 1

global _orig_PSGPSampleTexture
_orig_PSGPSampleTexture: resd 1
