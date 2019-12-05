@ECHO OFF

IF [%1]==[] (
GOTO emptyParameter
) ELSE (GOTO checkDebug)
GOTO eof


REM Checking if parameter == "debug"
:checkDebug
IF "%1"=="debug" (GOTO deb) ELSE (GOTO checkRelease)

REM Checking if parameter == "release"
:checkRelease
IF "%1"=="release" (GOTO rel) ELSE (GOTO unknownParameter)

REM Parameter is empty. Running default debug file
:emptyParameter
echo Parameter is empty. Running default debug file.
GOTO deb


REM Unknown parameter
:unknownParameter
ECHO Unknown parameter.
GOTO eof

REM Release
:rel
ECHO Running release file
CALL make.bat release
bin\release\nevada.exe
GOTO eof

REM Debug
:deb
ECHO Running debug file
CALL make.bat debug
bin\debug\nevada.exe
GOTO eof


REM The end...
:eof
ECHO Bye!
