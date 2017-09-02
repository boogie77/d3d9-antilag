@echo off
set ROOT=%~dp0
set BIN=%ROOT%\bin
set OBJ=%ROOT%\obj
set SRC=%ROOT%\src
set TOOLS=%ROOT%\tools

set GCC=gcc
set YASM=%TOOLS%\yasm-1.3.0-win32.exe

set GCC_FLAGS=-O2 -c
set GCC_LINK_FLAGS=-shared -nostartfiles -static -s -Wl,--enable-stdcall-fixup -e _DllMain
set YASM_FLAGS=-f win32

md %BIN% 2> nul
md %OBJ% 2> nul

%YASM% %YASM_FLAGS% -o %OBJ%\stub.o %SRC%\genfiles\stub.asm
%GCC% %GCC_FLAGS% -o %OBJ%\dllmain.o %SRC%\dllmain.c
%GCC% %GCC_FLAGS% -o %OBJ%\create.o %SRC%\create.c
%GCC% %GCC_FLAGS% -o %OBJ%\log.o %SRC%\log.c
%GCC% %GCC_LINK_FLAGS% -o %BIN%\d3d9.dll ^
    %OBJ%\stub.o ^
    %OBJ%\dllmain.o ^
    %OBJ%\create.o ^
    %OBJ%\log.o ^
    %SRC%\genfiles\export.def ^
    -lmsvcrt
