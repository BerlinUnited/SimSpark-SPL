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
# Begin Group "sceneserver"

# PROP Default_Filter ""
# Begin Group "helper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sceneserver\helper\NVMeshMender.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\helper\NVMeshMender.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\helper\nv_math.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\sceneserver\axis.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\axis.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\axis_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\basenode.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\basenode.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\basenode_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\camera.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\camera_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\fpscontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\fpscontroller.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\fpscontroller_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\indexbuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\indexbuffer.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\light.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\light.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\light_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\scene.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\scene.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\scene_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\sceneserver.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\sceneserver.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\sceneserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\sphere.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\sphere_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\staticmesh.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\staticmesh.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\staticmesh_c.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\transform.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneserver\transform.h
# End Source File
# Begin Source File

SOURCE=.\sceneserver\transform_c.cpp
# End Source File
# End Group
# Begin Group "physicsserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\physicsserver\body.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\body.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\body_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\collider.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\collider.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\collider_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\odeobject.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\odeobject.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\odeobject_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\physicsserver.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\physicsserver.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\physicsserver_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\planecollider.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\planecollider.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\planecollider_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\space.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\space.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\space_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\spherecollider.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\spherecollider.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\spherecollider_c.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\world.cpp
# End Source File
# Begin Source File

SOURCE=.\physicsserver\world.h
# End Source File
# Begin Source File

SOURCE=.\physicsserver\world_c.cpp
# End Source File
# End Group
# Begin Group "materialserver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\materialserver\material.cpp
# End Source File
# Begin Source File

SOURCE=.\materialserver\material.h
# End Source File
# Begin Source File

SOURCE=.\materialserver\materialserver.cpp
# End Source File
# Begin Source File

SOURCE=.\materialserver\materialserver.h
# End Source File
# Begin Source File

SOURCE=.\materialserver\materialserver_c.cpp
# End Source File
# End Group
# Begin Group "agentaspect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\agentaspect\agentaspect.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\agentaspect.h
# End Source File
# Begin Source File

SOURCE=.\agentaspect\agentaspect_c.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\effector.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\effector.h
# End Source File
# Begin Source File

SOURCE=.\agentaspect\effector_c.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\forceeffector.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\forceeffector.h
# End Source File
# Begin Source File

SOURCE=.\agentaspect\forceeffector_c.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\perceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\perceptor.h
# End Source File
# Begin Source File

SOURCE=.\agentaspect\perceptor_c.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\visionperceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\agentaspect\visionperceptor.h
# End Source File
# Begin Source File

SOURCE=.\agentaspect\visionperceptor_c.cpp
# End Source File
# End Group
# Begin Group "controlaspect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\controlaspect\controlaspect.cpp
# End Source File
# Begin Source File

SOURCE=.\controlaspect\controlaspect.h
# End Source File
# Begin Source File

SOURCE=.\controlaspect\controlaspect_c.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\kerosin.cpp
# End Source File
# Begin Source File

SOURCE=.\kerosin.h
# End Source File
# End Target
# End Project
