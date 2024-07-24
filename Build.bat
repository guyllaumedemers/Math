@ECHO OFF

SETLOCAL enabledelayedexpansion

:: retrieve all translation units
SET cppFilenames=
FOR /f usebackq %%i in (`DIR /ad /b %~dp0`) do (
	:: update current directory as FOR /r [drive:path] doesnt support %VAR%
	PUSHD "%%i"
	FOR /r %%k in (*.cc) do (
		SET cppFilenames=!cppFilenames! %%k
		ECHO %%k
	)
	POPD
)

:: build path executable
SET buildDir=%~dp0Out\Build\Sandbox.exe

:: compiler flags
:: https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-170
SET cflags=/std:c++17 /MT /Od

:: libraries
SET languagelibs=libucrt.lib libvcruntime.lib libcmt.lib libcpmt.lib
SET systemlibs=kernel32.lib
::SET externallibs=sdl2.lib imgui.lib
SET externallibs=

:: program linkage with external libs
SET elinkage=%languagelibs% %systemlibs% %externallibs%

:: Important!! %WinKit_ucrt% %WinKit_um% %VS_cruntime% are custom environment variables defined on my system
:: which are required to target x64 .lib for the ucrt and vcruntime. https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-170

:: library path
SET winkit_ucrt="%WinKit_ucrt%"
SET winkit_um="%WinKit_um%"
SET vcruntime="%VS_cruntime%"
SET sdl2=
SET imgui=

:: Note /LIBPATH support a single dir per-call https://learn.microsoft.com/en-us/cpp/build/reference/libpath-additional-libpath?view=msvc-170

:: linker flag
:: https://learn.microsoft.com/en-us/cpp/build/reference/linker-options?view=msvc-170
SET lflags=/NODEFAULTLIB /MACHINE:X64 /SUBSYSTEM:CONSOLE /LIBPATH:%winkit_um% /LIBPATH:%winkit_ucrt% /LIBPATH:%vcruntime%

:: Important!! Our target machine build for x64. Make sure to run command using MS Development Command Prompt for
:: the right target platform.

:: compiler command
cl %cflags% %cppFilenames% /link %lflags% %elinkage% /out:%buildDir%