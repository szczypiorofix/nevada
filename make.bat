@ECHO OFF
REM Simple batch file for running `make` instead of `mingw32-make` ;)

IF [%1]==[] (
GOTO emptyParameter
) ELSE (GOTO checkDebug)
GOTO eof

REM Parameter is empty. Running default makefile build.
:emptyParameter
echo Parameter is empty. Running default makefile build.
mingw32-make
GOTO eof


REM Checking if parameter == "debug"
:checkDebug
IF "%1"=="debug" (GOTO deb) ELSE (GOTO checkRelease)


REM Checking if parameter == "release"
:checkRelease
IF "%1"=="release" (GOTO rel) ELSE (GOTO checkRebuild)


REM Checking if parameter == "rebuild"
:checkRebuild
IF "%1"=="rebuild" (GOTO reb) ELSE (GOTO unknownParameter)


REM Debug
:deb
ECHO Running makefile for debug
mingw32-make debug
GOTO eof


REM Release
:rel
ECHO Running makefile for release
mingw32-make release
GOTO eof


REM Rebuild
:reb
ECHO Rebuilding whole project
mingw32-make -B debug
GOTO eof


REM Unknown parameter
:unknownParameter
ECHO Unknown parameter.
GOTO eof


REM The end...
:eof
ECHO Bye!