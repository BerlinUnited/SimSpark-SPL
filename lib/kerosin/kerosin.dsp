# Microsoft Developer Studio Project File - Name="kerosin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=kerosin - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "kerosin.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "kerosin.mak" CFG="kerosin - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "kerosin - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "kerosin - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kerosin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../bin/vc++/libkerosin.lib"

!ELSEIF  "$(CFG)" == "kerosin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../bin/vc++/libkerosin_debug.lib"

!ENDIF 

# Begin Target

# Name "kerosin - Win32 Release"
# Name "kerosin - Win32 Debug"
# Begin Group "inputserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\inputserver\inputdevice.cpp
# End Source File
# Begin Source File

SOURCE=.\inputserver\inputdevice.h
# End Source File
# Begin Source File

SOURCE=.\inputserver\inputserver.cpp
# End Source File
# Begin Source File

SOURCE=.\inputserver\inputserver.h
# End Source File
# Begin Source File

SOURCE=.\inputserver\inputserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\inputserver\inputsystem.cpp
# End Source File
# Begin Source File

SOURCE=.\inputserver\inputsystem.h
# End Source File
# Begin Source File

SOURCE=.\inputserver\scancodemap.cpp
# End Source File
# Begin Source File

SOURCE=.\inputserver\scancodemap.h
# End Source File
# End Group
# Begin Group "imageserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\imageserver\image.cpp
# End Source File
# Begin Source File

SOURCE=.\imageserver\image.h
# End Source File
# Begin Source File

SOURCE=.\imageserver\imageserver.cpp
# End Source File
# Begin Source File

SOURCE=.\imageserver\imageserver.h
# End Source File
# Begin Source File

SOURCE=.\imageserver\imageserver_c.cpp
# End Source File
# End Group
# Begin Group "fontserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fontserver\font.cpp
# End Source File
# Begin Source File

SOURCE=.\fontserver\font.h
# End Source File
# Begin Source File

SOURCE=.\fontserver\fontserver.cpp
# End Source File
# Begin Source File

SOURCE=.\fontserver\fontserver.h
# End Source File
# Begin Source File

SOURCE=.\fontserver\fontserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\fontserver\glyph.cpp
# End Source File
# Begin Source File

SOURCE=.\fontserver\glyph.h
# End Source File
# End Group
# Begin Group "soundserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\soundserver\soundeffect.h
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundmodule.h
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundobject.h
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundserver.cpp
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundserver.h
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundstream.h
# End Source File
# Begin Source File

SOURCE=.\soundserver\soundsystem.h
# End Source File
# End Group
# Begin Group "openglserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\openglserver\glextensionreg.cpp
# End Source File
# Begin Source File

SOURCE=.\openglserver\glextensionreg.h
# End Source File
# Begin Source File

SOURCE=.\openglserver\openglserver.cpp
# End Source File
# Begin Source File

SOURCE=.\openglserver\openglserver.h
# End Source File
# Begin Source File

SOURCE=.\openglserver\openglserver_c.cpp
# End Source File
# End Group
# Begin Group "textureserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\textureserver\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\textureserver\texture.h
# End Source File
# Begin Source File

SOURCE=.\textureserver\texture2d.cpp
# End Source File
# Begin Source File

SOURCE=.\textureserver\texture2d.h
# End Source File
# Begin Source File

SOURCE=.\textureserver\textureserver.cpp
# End Source File
# Begin Source File

SOURCE=.\textureserver\textureserver.h
# End Source File
# Begin Source File

SOURCE=.\textureserver\textureserver_c.cpp
# End Source File
# End Group
# Begin Group "scenegraph"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\kerosin.cpp
# End Source File
# Begin Source File

SOURCE=.\kerosin.h
# End Source File
# End Target
# End Project
