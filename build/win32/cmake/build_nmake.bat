@echo off

rem NMake build script

set TARGET_PATH=nmake

if not exist %TARGET_PATH% mkdir %TARGET_PATH%
cd %TARGET_PATH%

cmake ../../../../src ^
	-G "NMake Makefiles" ^
	-DENABLE_TESTS=True
nmake
nmake test /A
cd ..
pause