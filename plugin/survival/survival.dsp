# Microsoft Developer Studio Project File - Name="survival" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=survival - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "survival.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "survival.mak" CFG="survival - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "survival - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "survival - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "survival - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SURVIVAL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SURVIVAL_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libzeitgeist.lib libsalt.lib libkerosin.lib freetype211.lib ode.lib mswin32-ruby16.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../app/simulator/survival/survival.dll"

!ELSEIF  "$(CFG)" == "survival - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SURVIVAL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SURVIVAL_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libzeitgeist_debug.lib libsalt_debug.lib libkerosin_debug.lib freetype211.lib ode.lib mswin32-ruby16.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../app/simulator/survival/survival.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "survival - Win32 Release"
# Name "survival - Win32 Debug"
# Begin Group "assembly script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\survival.rb

!IF  "$(CFG)" == "survival - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\survival.rb

"..\..\app\simulator\survival\survival.rb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	xcopy survival.rb ..\..\app\simulator\survival\ /Y

# End Custom Build

!ELSEIF  "$(CFG)" == "survival - Win32 Debug"

# Begin Custom Build
InputPath=.\survival.rb

"..\..\app\simulator\survival\survival.rb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	xcopy survival.rb ..\..\app\simulator\survival\ /Y

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\export.cpp
# End Source File
# Begin Source File

SOURCE=.\linesegmentperceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\linesegmentperceptor.h
# End Source File
# Begin Source File

SOURCE=.\linesegmentperceptor_c.cpp
# End Source File
# Begin Source File

SOURCE=.\survivalagentaspect.cpp
# End Source File
# Begin Source File

SOURCE=.\survivalagentaspect.h
# End Source File
# Begin Source File

SOURCE=.\survivalagentaspect_c.cpp
# End Source File
# Begin Source File

SOURCE=.\survivalcontrolaspect.cpp
# End Source File
# Begin Source File

SOURCE=.\survivalcontrolaspect.h
# End Source File
# Begin Source File

SOURCE=.\survivalcontrolaspect_c.cpp
# End Source File
# End Target
# End Project
