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

:: build path executable
SET buildDir="%~dp0Out/Build"

:: retrieve all translation units
SET cppFilenames=
FOR /f usebackq %%i in (`DIR /ad /b %~dp0`) do (
	IF "%%i" EQU "Test" (
		PUSHD "%%i"
		FOR /r %%k in (*.cc) do (
			SET cppFilenames=!cppFilenames! %%k
		)
		POPD
	) ELSE IF "%%i" EQU "Sources" (
		PUSHD "%%i"
		FOR /r %%k in (*.cc) do (
			:: ignore main as we only care about gtest_main to run our test executable
			IF NOT "%%k" EQU "%~dp0%%i\Main.cc" (
				SET cppFilenames=!cppFilenames! %%k
			)
		)
		POPD
	)
)

:: project include directory
SET projDir="%~dp0Includes"

:: vendor include directories
SET googletestDir="%~dp0Vendor/googletest/googletest"

:: imgui source files we care about
SET googletestSrc="%googletestDir%/src/gtest-all.cc" "%googletestDir%/src/gtest_main.cc"
SET cppFilenames=!cppFilenames! %googletestSrc%

:: compiler flags
SET cflags=/std:c++20 /EHsc /MT /Od /I"%projDir%" /I"%googletestDir%" /I"%googletestDir%/include" /Fe"%buildDir%/Test/Test.exe" /Fo"%buildDir%/Test/"

:: libraries
SET languagelibs=libucrt.lib libvcruntime.lib libcmt.lib libcpmt.lib
SET systemlibs=kernel32.lib user32.lib Shell32.lib Imm32.lib

:: program linkage with system libs
SET elinkage=%languagelibs% %systemlibs%

:: system library path
SET winkit_ucrt="%WinKit_ucrt%"
SET winkit_um="%WinKit_um%"
SET vcruntime="%VS_cruntime%"

:: linker flag
SET lflags=/NODEFAULTLIB /MACHINE:X64 /SUBSYSTEM:CONSOLE /LIBPATH:%winkit_um% /LIBPATH:%winkit_ucrt% /LIBPATH:%vcruntime%

:: compiler command
cl %cflags% %cppFilenames% /link %lflags% %elinkage%
