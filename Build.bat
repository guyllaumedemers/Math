::Copyright(c) 2024 gdemers
::
::Permission is hereby granted, free of charge, to any person obtaining a copy
::of this software and associated documentation files(the "Software"), to deal
::in the Software without restriction, including without limitation the rights
::to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
::copies of the Software, and to permit persons to whom the Software is
::furnished to do so, subject to the following conditions :
::
::The above copyright notice and this permission notice shall be included in all
::copies or substantial portions of the Software.
::
::THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
::IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
::FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
::AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
::LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
::OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
::SOFTWARE.

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
SET buildDir="%~dp0Out\\Build\\"

:: compiler flags
:: https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-170
SET cflags=/std:c++17 /EHsc /MT /Od /Fe"%buildDir%Sandbox.exe" /Fo"%buildDir%"

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
cl %cflags% %cppFilenames% /link %lflags% %elinkage%