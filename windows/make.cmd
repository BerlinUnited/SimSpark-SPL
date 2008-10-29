@ECHO OFF
REM
REM Switches
REM

if "%1" == "" goto default
if "%1" == "-p" goto setpath
if "%1" == "/p" goto setpath
if "%1" == "-ch" goto changelog
if "%1" == "-h" goto help
if "%1" == "-help" goto help
if "%1" == "/?" goto help

REM 
REM Help
REM

:help
ECHO Usage: make [-p destination ^| -h ^| -ch]
ECHO Options:
ECHO.  [-p ^| /p] destination	  Specify install directory. Default is rcssserver3D_0.6
ECHO.  [-h ^| -help ^| /?]	Display this help.
ECHO.  [-ch]	Changelog of this script
ECHO.
ECHO Example: make -p ..\..\rcssserver3D-0.6
ECHO Install to the main rcssserver3D directory

goto end

REM
REM User defined destination path
REM

:setpath
if "%2" == "" goto end1
set directory=%2
goto start

REM
REM Default path
REM

:default
set directory=..\..\rcssserver3D-0.6

:start

REM
REM Delete old directory with all files
REM

IF EXIST %directory% (ECHO Directory "%directory%" exists. Do you want to delete it with all its files and sudirectories?[Y/n]) ELSE (goto continue1)
IF EXIST %directory% set /P dirdel=" "
	
IF %dirdel%==y (goto continue2)
IF %dirdel%=="" (goto continue2)
IF %dirdel%==n (goto end2)
goto end

:continue2
rd /S/Q %directory%

:continue1
if exist errors.log (del errors.log)

REM
REM Copy all necessary files of rcssserver3D into one directory
REM 

ECHO copy *.EXE files (4):
ECHO.
xcopy /I /V ..\windows\Release\*.exe %directory%

ECHO.
ECHO copy *.DLL files (12):
ECHO.
xcopy /I /V ..\windows\Release\*.dll %directory%

ECHO.
ECHO copy *.rb files (6):
ECHO.
xcopy /I /V ..\app\simspark\*.rb %directory%

ECHO.
ECHO copy materials (7):
ECHO.
xcopy /I /V ..\app\simspark\materials\*.* %directory%\materials

ECHO.
ECHO copy models (18):
ECHO.
xcopy /I /V ..\app\simspark\models\*.* %directory%\models

ECHO.
ECHO copy script (4):
ECHO.
xcopy /I /V ..\app\simspark\scripts\*.* %directory%\scripts

ECHO.
ECHO copy textures (46):
ECHO.
xcopy /I /V ..\app\simspark\textures\*.* %directory%\textures

ECHO.
ECHO copy rgs-agent-nao (21):
ECHO.
xcopy /I /V ..\app\simspark\rsg\agent\nao\*.* %directory%\rsg\agent\nao

ECHO. 
ECHO copy rsg-agent-soccerbot058 (12):
ECHO.
xcopy /I /V ..\app\simspark\rsg\agent\soccerbot058\*.* %directory%\rsg\agent\soccerbot058

ECHO.
ECHO copy rsg-boxspheres (13):
ECHO.
xcopy /I /V ..\app\simspark\rsg\boxspheres\*.* %directory%\rsg\boxspheres

ECHO.
ECHO copy rsg-jointtests (7):
ECHO.
xcopy /I /V ..\app\simspark\rsg\jointtest\*.* %directory%\rsg\jointtest

ECHO.
ECHO copy other rsg files (22):
ECHO.
xcopy /I /V ..\app\simspark\rsg\agent\*.* %directory%\rsg\agent

ECHO.
ECHO copy font (1):
ECHO.
xcopy /I /V ..\app\simspark\fonts\*.* %directory%\fonts
	
ECHO.
ECHO copy others rubyscipts:
ECHO.
xcopy /I /V ..\app\agenttest\agenttest.rb %directory%
xcopy /I /V ..\lib\zeitgeist\zeitgeist.rb %directory%
xcopy /I /V ..\lib\kerosin\kerosin.rb %directory%
xcopy /I /V ..\lib\kerosin\inputserver\german.scan.rb %directory%
xcopy /I /V ..\lib\kerosin\inputserver\us.scan.rb %directory%
xcopy /I /V ..\app\monitorspark\monitorspark.rb %directory%
xcopy /I /V ..\app\monitorspark\soccerbindings.rb %directory%
xcopy /I /V ..\app\monitorspark\soccersim-monitor.rb %directory%
xcopy /I /V ..\lib\oxygen\oxygen.rb %directory%
xcopy /I /V ..\lib\spark\spark.rb %directory%

setlocal
cd > temp1.txt
set /P defaultdirectory= < temp1.txt
del temp1.txt
cd %directory%
cd > temp1.txt

set /P packagedirectory= < temp1.txt
del temp1.txt

ECHO.
ECHO. Directory is created in %packagedirectory%.

cd %defaultdirectory%
endlocal

goto end

:end1
ECHO Please 
:end2
ECHO.
ECHO Please delete "%directory%" or choose another directory.[To how type "make /?"]
goto end

:changelog
ECHO.
ECHO Fri 24 2008 - Marian Buchta ^<marian.buchta@gmail.com^>
ECHO. -Initial version
ECHO.

:end