####
# Variables
#
# Overwrite some of the default settings
#

Application.title = 'Simulator';

Viewport.xRes = 1024;
Viewport.yRes = 768;
#Viewport.xRes = 800;
#Viewport.yRes = 600;
Viewport.fullScreen = false;

####
# OpenGL Server
#
openglServer = new ('kerosin/OpenGLServer', '/sys/server/opengl');

####
# Image Server
#
new ('kerosin/ImageServer', '/sys/server/image');

####
# Texture Server
#
new ('kerosin/TextureServer', '/sys/server/texture');

####
# Material Server
#
new ('kerosin/MaterialServer', '/sys/server/material');

####
# Input Server
#
inputServer = new ('kerosin/InputServer', '/sys/server/input');
inputServer.init ('InputSystemSDL');
# load german keyboard layout
inputServer.importScanCodeMapping ('sys/script/german.scan.rb');

inputServer.createDevice ('Keyboard');
inputServer.createDevice ('Mouse');
inputServer.createDevice ('Timer');

inputServer.bindCommand ('+escape',		Command.Quit);
inputServer.bindCommand ('axist',		Command.Timer);
inputServer.bindCommand ('axisx',		Command.MouseX);
inputServer.bindCommand ('axisy',		Command.MouseY);
inputServer.bindCommand ('+mouse_left',	Command.MouseLeft);
inputServer.bindCommand ('left',		Command.Left);
inputServer.bindCommand ('a',			Command.Left);
inputServer.bindCommand ('right',		Command.Right);
inputServer.bindCommand ('d',			Command.Right);
inputServer.bindCommand ('up',			Command.Forward);
inputServer.bindCommand ('w',			Command.Forward);
inputServer.bindCommand ('down',		Command.Backward);
inputServer.bindCommand ('s',			Command.Backward);
inputServer.bindCommand ('+l',			Command.Lighting);

####
# FontServer
#
new ('kerosin/FontServer', '/sys/server/font');

####
# PhysicsServer
#
new ('kerosin/PhysicsServer', '/sys/server/physics');

###
# SceneServer
#
sceneServer = new ('kerosin/SceneServer', '/sys/server/scene');
sceneServer.createScene('/usr/scene');
