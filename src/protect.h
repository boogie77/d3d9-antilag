#define MP_PROTECT_BEGIN(address, size) \
    do { \
        HANDLE __mp_process = OpenProcess(PROCESS_ALL_ACCESS, 0, GetCurrentProcessId()); \
        DWORD __mp_old_protect; \
        VirtualProtectEx(__mp_process, address, size, PAGE_EXECUTE_READWRITE, &__mp_old_protect)

#define MP_PROTECT_END(address, size) \
        VirtualProtectEx(__mp_process, address, size, __mp_old_protect, &__mp_old_protect); \
        CloseHandle(__mp_process); \
    } while (0)
