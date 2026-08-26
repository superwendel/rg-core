@echo off
setlocal

set "TARGET=%~1"
if not defined TARGET set "TARGET=test"

if /I "%TARGET%"=="clean" goto clean
if /I "%TARGET%"=="test" goto test
if /I "%TARGET%"=="test_sprintf" goto test_sprintf
if /I "%TARGET%"=="test_log" goto test_log
if /I "%TARGET%"=="test_assert" goto test_assert
if /I "%TARGET%"=="test_mem" goto test_mem
if /I "%TARGET%"=="test_containers" goto test_containers
if /I "%TARGET%"=="test_time" goto test_time
if /I "%TARGET%"=="test_bin" goto test_bin
if /I "%TARGET%"=="test_hash" goto test_hash
if /I "%TARGET%"=="test_random" goto test_random
if /I "%TARGET%"=="test_algo" goto test_algo
if /I "%TARGET%"=="test_string" goto test_string
if /I "%TARGET%"=="test_math" goto test_math

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
call "%~f0" test_sprintf
if errorlevel 1 exit /b 1
call "%~f0" test_log
if errorlevel 1 exit /b 1
call "%~f0" test_assert
if errorlevel 1 exit /b 1
call "%~f0" test_mem
if errorlevel 1 exit /b 1
call "%~f0" test_containers
if errorlevel 1 exit /b 1
call "%~f0" test_time
if errorlevel 1 exit /b 1
call "%~f0" test_bin
if errorlevel 1 exit /b 1
call "%~f0" test_string
if errorlevel 1 exit /b 1
call "%~f0" test_hash
if errorlevel 1 exit /b 1
call "%~f0" test_random
if errorlevel 1 exit /b 1
call "%~f0" test_algo
if errorlevel 1 exit /b 1
call "%~f0" test_math
if errorlevel 1 exit /b 1
echo All tests passed.
exit /b 0

:test_sprintf
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

echo All rg_sprintf tests passed.
exit /b 0

:test_log
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /std:c11 /W4 /WX /D_CRT_SECURE_NO_WARNINGS"

echo Building assembly helpers for rg_log...
ml64 /nologo /c /Fo rg_sprintf_asm_x64.obj src\asm\sprintf\win_x64\rg_sprintf_asm_x64.asm
if errorlevel 1 exit /b 1

echo Building rg_log tests...
cl %COMMON_FLAGS% /O2 /arch:AVX2 tests\test_log.c rg_sprintf_asm_x64.obj /Fe:test_log.exe
if errorlevel 1 exit /b 1
test_log.exe
if errorlevel 1 exit /b 1

echo Building rg_log portable fallback tests...
cl %COMMON_FLAGS% /O2 /DRG_SPRINTF_NO_ASM /DRG_SPRINTF_NO_SIMD tests\test_log.c /Fe:test_log_fallback.exe
if errorlevel 1 exit /b 1
test_log_fallback.exe
if errorlevel 1 exit /b 1

echo All rg_log tests passed.
exit /b 0

:test_assert
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /std:c11 /W4 /WX /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_assert tests...
cl %COMMON_FLAGS% /O2 tests\test_assert.c /Fe:test_assert.exe
if errorlevel 1 exit /b 1
test_assert.exe
if errorlevel 1 exit /b 1

echo Building disabled rg_assert tests...
cl %COMMON_FLAGS% /O2 tests\test_assert_disabled.c /Fe:test_assert_disabled.exe
if errorlevel 1 exit /b 1
test_assert_disabled.exe
if errorlevel 1 exit /b 1

echo All rg_assert tests passed.
exit /b 0

:test_mem
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_mem tests...
cl %COMMON_FLAGS% /std:c11 tests\test_mem.c /Fe:test_mem.exe
if errorlevel 1 exit /b 1
test_mem.exe
if errorlevel 1 exit /b 1

echo Building eager-commit rg_mem tests...
cl %COMMON_FLAGS% /std:c11 /DRG_MALLOC_LAZY_COMMIT=0 tests\test_mem.c /Fe:test_mem_eager.exe
if errorlevel 1 exit /b 1
test_mem_eager.exe
if errorlevel 1 exit /b 1

echo Building secure-reset rg_mem tests...
cl %COMMON_FLAGS% /std:c11 /DRG_MALLOC_SECURE tests\test_mem.c /Fe:test_mem_secure.exe
if errorlevel 1 exit /b 1
test_mem_secure.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_mem compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 tests\test_mem.c /Fe:test_mem_cpp.exe
if errorlevel 1 exit /b 1
test_mem_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_mem tests passed.
exit /b 0

:test_containers
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_containers tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_containers.obj tests\test_containers.c /Fe:test_containers.exe
if errorlevel 1 exit /b 1
test_containers.exe
if errorlevel 1 exit /b 1

echo Building configured rg_containers tests...
cl %COMMON_FLAGS% /std:c11 /DRG_CONTAINERS_MIN_CAP=3 /DRG_SPARSE_INVALID=17 /Fo:test_containers_config.obj tests\test_containers.c /Fe:test_containers_config.exe
if errorlevel 1 exit /b 1
test_containers_config.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_containers compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 /Fo:test_containers_cpp.obj tests\test_containers.c /Fe:test_containers_cpp.exe
if errorlevel 1 exit /b 1
test_containers_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_containers tests passed.
exit /b 0

:test_time
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_time tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_time.obj tests\test_time.c /Fe:test_time.exe
if errorlevel 1 exit /b 1
test_time.exe
if errorlevel 1 exit /b 1

echo Building custom-backend rg_time tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_time_custom.obj tests\test_time_custom.c /Fe:test_time_custom.exe
if errorlevel 1 exit /b 1
test_time_custom.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_time compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 /Fo:test_time_cpp.obj tests\test_time.c /Fe:test_time_cpp.exe
if errorlevel 1 exit /b 1
test_time_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_time tests passed.
exit /b 0

:test_bin
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_bin tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_bin.obj tests\test_bin.c /Fe:test_bin.exe
if errorlevel 1 exit /b 1
test_bin.exe
if errorlevel 1 exit /b 1

echo Building fast-unaligned rg_bin tests...
cl %COMMON_FLAGS% /std:c11 /DRG_BIN_FAST_UNALIGNED=1 /Fo:test_bin_unaligned.obj tests\test_bin.c /Fe:test_bin_unaligned.exe
if errorlevel 1 exit /b 1
test_bin_unaligned.exe
if errorlevel 1 exit /b 1

echo Building bytewise rg_bin tests...
cl %COMMON_FLAGS% /std:c11 /DRG_BIN_LITTLE_ENDIAN=0 /Fo:test_bin_bytewise.obj tests\test_bin.c /Fe:test_bin_bytewise.exe
if errorlevel 1 exit /b 1
test_bin_bytewise.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_bin compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 /DNDEBUG /Fo:test_bin_cpp.obj tests\test_bin.c /Fe:test_bin_cpp.exe
if errorlevel 1 exit /b 1
test_bin_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_bin tests passed.
exit /b 0

:test_hash
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_hash tests...
cl %COMMON_FLAGS% /std:c11 tests\test_hash.c /Fe:test_hash.exe
if errorlevel 1 exit /b 1
test_hash.exe
if errorlevel 1 exit /b 1

echo Building eager-commit rg_hash tests...
cl %COMMON_FLAGS% /std:c11 /DRG_MALLOC_LAZY_COMMIT=0 tests\test_hash.c /Fe:test_hash_eager.exe
if errorlevel 1 exit /b 1
test_hash_eager.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_hash compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 tests\test_hash.c /Fe:test_hash_cpp.exe
if errorlevel 1 exit /b 1
test_hash_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_hash tests passed.
exit /b 0

:test_random
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_random tests...
cl %COMMON_FLAGS% /std:c11 tests\test_random.c /Fe:test_random.exe
if errorlevel 1 exit /b 1
test_random.exe
if errorlevel 1 exit /b 1

echo Building portable-multiply rg_random tests...
cl %COMMON_FLAGS% /std:c11 /DRG_RANDOM_FORCE_PORTABLE_MUL128 tests\test_random.c /Fe:test_random_portable.exe
if errorlevel 1 exit /b 1
test_random_portable.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_random compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 tests\test_random.c /Fe:test_random_cpp.exe
if errorlevel 1 exit /b 1
test_random_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_random tests passed.
exit /b 0

:test_algo
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_algo tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_algo.obj tests\test_algo.c /Fe:test_algo.exe
if errorlevel 1 exit /b 1
test_algo.exe
if errorlevel 1 exit /b 1

echo Building configured rg_algo tests...
cl %COMMON_FLAGS% /std:c11 /DRG_ALGO_RADIX_BITS=4 /DRG_ALGO_STABLE_RUN=5 /DRG_ALGO_INSERTION_CUTOFF=9 /DRG_ALGO_STACK_CAP=1 /Fo:test_algo_config.obj tests\test_algo.c /Fe:test_algo_config.exe
if errorlevel 1 exit /b 1
test_algo_config.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_algo compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 /Fo:test_algo_cpp.obj tests\test_algo.c /Fe:test_algo_cpp.exe
if errorlevel 1 exit /b 1
test_algo_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_algo tests passed.
exit /b 0

:test_string
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building rg_string tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_string.obj tests\test_string.c /Fe:test_string.exe
if errorlevel 1 exit /b 1
test_string.exe
if errorlevel 1 exit /b 1

echo Building AVX2 rg_string tests...
cl %COMMON_FLAGS% /std:c11 /arch:AVX2 /Fo:test_string_avx2.obj tests\test_string.c /Fe:test_string_avx2.exe
if errorlevel 1 exit /b 1
test_string_avx2.exe
if errorlevel 1 exit /b 1

echo Building forced-scalar rg_string tests...
cl %COMMON_FLAGS% /std:c11 /arch:AVX2 /DRG_STRING_NO_SIMD /Fo:test_string_scalar.obj tests\test_string.c /Fe:test_string_scalar.exe
if errorlevel 1 exit /b 1
test_string_scalar.exe
if errorlevel 1 exit /b 1

echo Building secure AVX2 rg_string tests...
cl %COMMON_FLAGS% /std:c11 /arch:AVX2 /DRG_STRING_SECURE /Fo:test_string_secure.obj tests\test_string.c /Fe:test_string_secure.exe
if errorlevel 1 exit /b 1
test_string_secure.exe
if errorlevel 1 exit /b 1

echo Building C++ AVX2 rg_string compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 /arch:AVX2 /Fo:test_string_cpp.obj tests\test_string.c /Fe:test_string_cpp.exe
if errorlevel 1 exit /b 1
test_string_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_string tests passed.
exit /b 0

:test_math
call :ensure_compiler
if errorlevel 1 exit /b 1

set "COMMON_FLAGS=/nologo /W4 /WX /O2 /D_CRT_SECURE_NO_WARNINGS"

echo Building baseline SIMD rg_math tests...
cl %COMMON_FLAGS% /std:c11 /Fo:test_math.obj tests\test_math.c /Fe:test_math.exe
if errorlevel 1 exit /b 1
test_math.exe
if errorlevel 1 exit /b 1

echo Building AVX2 rg_math tests...
cl %COMMON_FLAGS% /std:c11 /arch:AVX2 /Fo:test_math_avx2.obj tests\test_math.c /Fe:test_math_avx2.exe
if errorlevel 1 exit /b 1
test_math_avx2.exe
if errorlevel 1 exit /b 1

echo Building checked scalar rg_math tests...
cl %COMMON_FLAGS% /std:c11 /DRG_MATH_NO_SIMD /DRG_MATH_MAX_PERF=0 /Fo:test_math_scalar.obj tests\test_math.c /Fe:test_math_scalar.exe
if errorlevel 1 exit /b 1
test_math_scalar.exe
if errorlevel 1 exit /b 1

echo Building checked SIMD rg_math tests...
cl %COMMON_FLAGS% /std:c11 /DRG_MATH_MAX_PERF=0 /Fo:test_math_checked.obj tests\test_math.c /Fe:test_math_checked.exe
if errorlevel 1 exit /b 1
test_math_checked.exe
if errorlevel 1 exit /b 1

echo Building plain-layout AVX2 rg_math tests...
cl %COMMON_FLAGS% /std:c11 /arch:AVX2 /DRG_MATH_VEC3_PLAIN /DRG_MATH_VEC4_PLAIN /Fo:test_math_plain.obj tests\test_math.c /Fe:test_math_plain.exe
if errorlevel 1 exit /b 1
test_math_plain.exe
if errorlevel 1 exit /b 1

echo Building reduced-module rg_math tests...
cl %COMMON_FLAGS% /std:c11 /DRG_MATH_NO_SIMD /Fo:test_math_lean.obj tests\test_math_lean.c /Fe:test_math_lean.exe
if errorlevel 1 exit /b 1
test_math_lean.exe
if errorlevel 1 exit /b 1

echo Building C++ rg_math compatibility tests...
cl %COMMON_FLAGS% /TP /std:c++17 /DNDEBUG /Fo:test_math_cpp.obj tests\test_math.c /Fe:test_math_cpp.exe
if errorlevel 1 exit /b 1
test_math_cpp.exe
if errorlevel 1 exit /b 1

echo All rg_math tests passed.
exit /b 0

:clean
del /q test_sprintf.exe test_sprintf_scalar.exe test_sprintf_asm.exe 2>nul
del /q test_sprintf_fallback.exe test_sprintf_asm_fallback.exe 2>nul
del /q test_sprintf_secure.exe test_sprintf_asm_secure.exe 2>nul
del /q test_log.exe test_log_fallback.exe test_assert.exe test_assert_disabled.exe 2>nul
del /q test_mem.exe test_mem_eager.exe test_mem_secure.exe test_mem_cpp.exe 2>nul
del /q test_containers.exe test_containers_config.exe test_containers_cpp.exe 2>nul
del /q test_time.exe test_time_custom.exe test_time_cpp.exe 2>nul
del /q test_bin.exe test_bin_unaligned.exe test_bin_bytewise.exe test_bin_cpp.exe 2>nul
del /q test_hash.exe test_hash_eager.exe test_hash_cpp.exe 2>nul
del /q test_random.exe test_random_portable.exe test_random_cpp.exe 2>nul
del /q test_algo.exe test_algo_config.exe test_algo_cpp.exe 2>nul
del /q test_string.exe test_string_avx2.exe test_string_scalar.exe test_string_secure.exe test_string_cpp.exe 2>nul
del /q test_math.exe test_math_avx2.exe test_math_scalar.exe test_math_checked.exe test_math_plain.exe test_math_lean.exe test_math_cpp.exe 2>nul
del /q test_sprintf.obj test_log.obj test_assert.obj test_assert_disabled.obj test_mem.obj test_hash.obj test_random.obj 2>nul
del /q test_containers.obj test_containers_config.obj test_containers_cpp.obj 2>nul
del /q test_time.obj test_time_custom.obj test_time_cpp.obj 2>nul
del /q test_bin.obj test_bin_unaligned.obj test_bin_bytewise.obj test_bin_cpp.obj 2>nul
del /q test_algo.obj test_algo_config.obj test_algo_cpp.obj 2>nul
del /q test_string.obj test_string_avx2.obj test_string_scalar.obj test_string_secure.obj test_string_cpp.obj 2>nul
del /q test_math.obj test_math_avx2.obj test_math_scalar.obj test_math_checked.obj test_math_plain.obj test_math_lean.obj test_math_cpp.obj 2>nul
del /q rg_sprintf_asm_x64.obj 2>nul
exit /b 0
