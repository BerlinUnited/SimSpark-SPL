####
# Variables
#
# Overwrite some of the default settings
#

Application.title = 'SceneTest';

#Viewport.xRes = 1024;
#Viewport.yRes = 768;
#Viewport.xRes = 800;
#Viewport.yRes = 600;
#Viewport.fullScreen = true;

####
# OpenGL Server
#
openglServer = new ('kerosin/OpenGLServer', '/sys/server/opengl');
openglServer.init();

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
inputServer.setScanCodeMapping('german.scan.rb');

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
sceneServer.createScene('/usr/scene1');
cd ('/usr/scene1');
world = new ('kerosin/World', '_world');
world.setGravity(0.0, -9.81, 0.0);
new ('kerosin/Space', '../_space');

camera = new ('kerosin/Transform', '../camera0');
camera.setLocalPos(0.0, 0.0, 10.0);
new ('kerosin/Camera', 'camera');
light = new ('kerosin/Light', '../_light');
light.setRadius(30.0);
light.setDiffuseColor(1.0, 1.0, 1.0);
body = new ('kerosin/RigidBody', '../_body');
body.useGravity(false);
new ('kerosin/FPSController', 'fps');
#collider = new ('kerosin/SphereCollider', '../../_collider');
#collider.setRadius(3.0);

# add a sphere
cd ('/usr/scene1/');
trans = new ('kerosin/Transform', 'sphere1');
trans.setLocalPos(0.0, 0.0, 0.0);
sphere = new ('kerosin/StaticMesh', '_vis');
sphere.load('model/sphere.void');
#sphere.setRadius(1.0);
#body = new ('kerosin/Body', '../_physics');
#body.setSphere(1.0, 10.0);
#body.setMass(1.0);
#geom = new ('kerosin/SphereCollider', '../_geometry');
#geom.setRadius(10.0);
cd ('/usr/scene1/');
trans = new ('kerosin/Transform', 'mesh1');
trans.setLocalPos(0.0, 0.0, 1.0);
#mesh = new ('kerosin/StaticMesh', 'test');
#mesh.load('model/sphere.void');

# add lights
#cd ('/usr/scene1');
#trans = new ('kerosin/Transform', 'light1');
#trans.setLocalPos(0.0, 0.0, 0.0);
#light = new ('kerosin/Light', '_light');
#light.setRadius(2.0);
#light.setDiffuseColor(1.0, 1.0, 1.0);

