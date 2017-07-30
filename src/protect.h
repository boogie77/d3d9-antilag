static inline void mp_assign(
    void **addr,
    void *value)
{
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, GetCurrentProcessId());
    DWORD old_protect;
    VirtualProtectEx(process, addr, sizeof(void *), PAGE_EXECUTE_READWRITE, &old_protect);
    *addr = value;
    VirtualProtectEx(process, addr, sizeof(void *), old_protect, &old_protect);
}
