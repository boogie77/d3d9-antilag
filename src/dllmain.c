#include <windows.h>
#include "genfiles/loader.h"

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    static char orig_d3d9_path[256];
    HMODULE orig_d3d9;

    GetSystemDirectoryA(orig_d3d9_path, sizeof(orig_d3d9_path));
    lstrcatA(orig_d3d9_path, "\\d3d9.dll");
    orig_d3d9 = LoadLibraryA(orig_d3d9_path);
    load_orig_ptrs(orig_d3d9);
    return TRUE;
}
