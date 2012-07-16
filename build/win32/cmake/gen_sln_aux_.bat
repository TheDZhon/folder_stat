@echo off

rem Solution generation script
rem Requires toolset param

if [%1]==[] goto NFOUND

set TARGET_SLN_PATH=sln\%1

GOTO CASE_%1
	:CASE_2005
		set GEN_NAME=Visual Studio 8 2005
		GOTO END_SWITCH
	:CASE_2008
		set GEN_NAME=Visual Studio 9 2008
		GOTO END_SWITCH
	:CASE_2010
		set GEN_NAME=Visual Studio 10
		GOTO END_SWITCH
	:CASE_2012
		set GEN_NAME=Visual Studio 11
		GOTO END_SWITCH
:END_SWITCH

echo Generating solution for %GEN_NAME% 
echo Destination path: %TARGET_SLN_PATH%

if not exist %TARGET_SLN_PATH% mkdir %TARGET_SLN_PATH%
cd %TARGET_SLN_PATH%

cmake ../../../../../src ^
	-G "%GEN_NAME%" ^
	-DENABLE_TESTS=True

cd ../..
pause

