# Microsoft Developer Studio Project File - Name="zeitgeist" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zeitgeist - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "zeitgeist.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "zeitgeist.mak" CFG="zeitgeist - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "zeitgeist - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "zeitgeist - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zeitgeist - Win32 Release"

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
# ADD LIB32 /nologo /out:"../bin/vc++/libzeitgeist.lib"

!ELSEIF  "$(CFG)" == "zeitgeist - Win32 Debug"

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
# ADD LIB32 /nologo /out:"../bin/vc++/libzeitgeist_debug.lib"

!ENDIF 

# Begin Target

# Name "zeitgeist - Win32 Release"
# Name "zeitgeist - Win32 Debug"
# Begin Group "logserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\logserver\logserver.cpp
# End Source File
# Begin Source File

SOURCE=.\logserver\logserver.h
# End Source File
# Begin Source File

SOURCE=.\logserver\logserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\logserver\logserverstreambuf.cpp
# End Source File
# Begin Source File

SOURCE=.\logserver\logserverstreambuf.h
# End Source File
# End Group
# Begin Group "scriptserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\scriptserver\scriptserver.cpp
# End Source File
# Begin Source File

SOURCE=.\scriptserver\scriptserver.h
# End Source File
# Begin Source File

SOURCE=.\scriptserver\scriptserver_c.cpp
# End Source File
# End Group
# Begin Group "fileserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fileserver\fileserver.cpp
# End Source File
# Begin Source File

SOURCE=.\fileserver\fileserver.h
# End Source File
# Begin Source File

SOURCE=.\fileserver\fileserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\fileserver\filesystem.h
# End Source File
# Begin Source File

SOURCE=.\fileserver\filesystem_c.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\base.cpp
# End Source File
# Begin Source File

SOURCE=.\base.h
# End Source File
# Begin Source File

SOURCE=.\base_c.cpp
# End Source File
# Begin Source File

SOURCE=.\class.cpp
# End Source File
# Begin Source File

SOURCE=.\class.h
# End Source File
# Begin Source File

SOURCE=.\core.cpp
# End Source File
# Begin Source File

SOURCE=.\core.h
# End Source File
# Begin Source File

SOURCE=.\corecontext.cpp
# End Source File
# Begin Source File

SOURCE=.\corecontext.h
# End Source File
# Begin Source File

SOURCE=.\node.cpp
# End Source File
# Begin Source File

SOURCE=.\node.h
# End Source File
# Begin Source File

SOURCE=.\node_c.cpp
# End Source File
# Begin Source File

SOURCE=.\object.cpp
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\object_c.cpp
# End Source File
# Begin Source File

SOURCE=.\object_c.h
# End Source File
# Begin Source File

SOURCE=.\zeitgeist.cpp
# End Source File
# Begin Source File

SOURCE=.\zeitgeist.h
# End Source File
# End Target
# End Project
