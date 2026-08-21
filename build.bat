@echo off
setlocal

set "TARGET=%~1"
if not defined TARGET set "TARGET=test"

if /I "%TARGET%"=="clean" goto clean
if /I "%TARGET%"=="test" goto test
if /I "%TARGET%"=="test_sprintf" goto test

echo Unknown target: %TARGET%
exit /b 1

:ensure_compiler
where cl >nul 2>nul
if not errorlevel 1 exit /b 0

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
	echo Could not find cl.exe or vswhere.exe.
	exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%i"
if not defined VSINSTALL (
	echo Could not find a Visual Studio C++ installation.
	exit /b 1
)

call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b 1
exit /b 0

:test
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /std:c11 /W4 /WX /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_sprintf tests...
cl %COMMON_FLAGS% /O2 /arch:AVX2 tests\test_sprintf.c /Fe:test_sprintf.exe
if errorlevel 1 exit /b 1
test_sprintf.exe
if errorlevel 1 exit /b 1

echo Building rg_sprintf scalar tests...
cl %COMMON_FLAGS% /O2 /DRG_SPRINTF_NO_SIMD tests\test_sprintf.c /Fe:test_sprintf_scalar.exe
if errorlevel 1 exit /b 1
test_sprintf_scalar.exe
if errorlevel 1 exit /b 1

echo Building assembly helpers...
ml64 /nologo /c /Fo rg_sprintf_asm_x64.obj src\asm\sprintf\win_x64\rg_sprintf_asm_x64.asm
if errorlevel 1 exit /b 1

echo Building rg_sprintf hybrid assembly tests...
cl %COMMON_FLAGS% /O2 /arch:AVX2 /DRG_SPRINTF_TEST_HYBRID tests\test_sprintf.c rg_sprintf_asm_x64.obj /Fe:test_sprintf_asm.exe
if errorlevel 1 exit /b 1
test_sprintf_asm.exe
if errorlevel 1 exit /b 1

echo Building rg_sprintf hybrid C fallback tests...
cl %COMMON_FLAGS% /O2 /DRG_SPRINTF_TEST_HYBRID /DRG_SPRINTF_NO_ASM /DRG_SPRINTF_NO_SIMD tests\test_sprintf.c /Fe:test_sprintf_fallback.exe
if errorlevel 1 exit /b 1
test_sprintf_fallback.exe
if errorlevel 1 exit /b 1

echo Building rg_sprintf assembly-header fallback tests...
cl %COMMON_FLAGS% /O2 /DRG_SPRINTF_TEST_ASM /DRG_SPRINTF_NO_ASM /DRG_SPRINTF_NO_SIMD tests\test_sprintf.c /Fe:test_sprintf_asm_fallback.exe
if errorlevel 1 exit /b 1
test_sprintf_asm_fallback.exe
if errorlevel 1 exit /b 1

echo Building rg_sprintf portable secure tests...
cl %COMMON_FLAGS% /O2 /arch:AVX2 /DRG_SPRINTF_SECURE tests\test_sprintf.c /Fe:test_sprintf_secure.exe
if errorlevel 1 exit /b 1
test_sprintf_secure.exe
if errorlevel 1 exit /b 1

echo Building rg_sprintf assembly secure tests...
cl %COMMON_FLAGS% /O2 /arch:AVX2 /DRG_SPRINTF_TEST_ASM /DRG_SPRINTF_SECURE /DRG_SPRINTF_HAS_ASM tests\test_sprintf.c rg_sprintf_asm_x64.obj /Fe:test_sprintf_asm_secure.exe
if errorlevel 1 exit /b 1
test_sprintf_asm_secure.exe
if errorlevel 1 exit /b 1

echo All tests passed.
exit /b 0

:clean
del /q test_sprintf.exe test_sprintf_scalar.exe test_sprintf_asm.exe 2>nul
del /q test_sprintf_fallback.exe test_sprintf_asm_fallback.exe 2>nul
del /q test_sprintf_secure.exe test_sprintf_asm_secure.exe 2>nul
del /q test_sprintf.obj rg_sprintf_asm_x64.obj 2>nul
exit /b 0
