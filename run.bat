@ECHO OFF

:checking
IF EXIST "bin\debug\nevada.exe" (GOTO running) ELSE (GOTO preparing)
GOTO theend

:preparing
CALL make.bat debug
GOTO checking

:running
bin\debug\nevada.exe
GOTO theend

:theend
