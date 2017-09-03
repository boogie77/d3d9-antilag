#include <malloc.h>
#include <stdio.h>
#include <windows.h>
#include "log.h"

void vlprintf(const char *format, va_list args)
{
    int size = _vsnprintf(NULL, 0, format, args) + 1;
    char *buffer = _alloca(size);
    _vsnprintf(buffer, size, format, args);
    OutputDebugStringA(buffer);
}

void lprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vlprintf(format, args);
    va_end(args);
}
