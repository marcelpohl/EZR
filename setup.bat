@ECHO OFF

WHERE cmake
IF ERRORLEVEL == 1 (
	ECHO ERROR: CMake needs to be installed!
	GOTO end
) 
CLS

IF "%GLM_ROOT%"=="" (
	ECHO WARNING: The necessary libraries for this framework are not found.
	pause
) 
CLS	

ECHO Choose your IDE:
ECHO (1) Eclipse CDT mit MinGW
ECHO (2) Sublime Text 2 mit MinGW
ECHO (3) VisualStudio2010
ECHO (4) VisualStudio2012
ECHO (5) VisualStudio2013
ECHO (6) VisualStudio2015
ECHO (7) VisualStudio2017
ECHO ----------------------
CHOICE /n /c "1234567" /M ":"

IF ERRORLEVEL==7 ( SET FOLDER="BUILD_VisualStudio17" & SET GEN="Visual Studio 15 2017" & GOTO create )
IF ERRORLEVEL==6 ( SET FOLDER="BUILD_VisualStudio15" & SET GEN="Visual Studio 14 2015" & GOTO create )
IF ERRORLEVEL==5 ( SET FOLDER="BUILD_VisualStudio13" & SET GEN="Visual Studio 12" & GOTO create )
IF ERRORLEVEL==4 ( SET FOLDER="BUILD_VisualStudio12" & SET GEN="Visual Studio 11" & GOTO create )
IF ERRORLEVEL==3 ( SET FOLDER="BUILD_VisualStudio10" & SET GEN="Visual Studio 10" & GOTO create )
IF ERRORLEVEL==2 ( SET FOLDER="BUILD_SublimeText2" & SET GEN="Sublime Text 2 - MinGW Makefiles" & GOTO create )
IF ERRORLEVEL==1 ( SET FOLDER="BUILD_Eclipse" & SET GEN="Eclipse CDT4 - MinGW Makefiles" & GOTO create )
ECHO Something went wrong... 
GOTO end

:create
mkdir %FOLDER%
cd %FOLDER%
cmake -G%GEN% ../src/
if "!errorlevel!"=="1" cd.. &echo Ein CMake Fehler ist aufgetreten. !FOLDER! wurde nicht erzeugt.

:end
pause
