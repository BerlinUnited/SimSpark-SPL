#
# import needed bundles
#

#
# generic plugins
importBundle "filesystemstd"
importBundle "sexpparser"

importBundle "perfectvisionperceptor"
importBundle "forceeffector"

#
# bundle of soccer plugins
importBundle "soccer"

#
# declare some helper methods
#
def addFlag(name,x,y,z)
  flag = new('FieldFlag','/usr/scene/'+name);
  flag.setLocalPos(x,y,z);
end

def addWall(name,a,b,c,d)
  wall = new('kerosin/PlaneCollider', '/usr/scene/'+name);
  wall.setParams(a,b,c,d);
end

#
# init
#

#
# mount a standard file system
fileServer = get('/sys/server/file');
fileServer.mount('FileSystemSTD', 'data/');

#
# setup the PhysicsServer
new('kerosin/PhysicsServer', '/sys/server/physics');

#
# setup the SceneServer
sceneServer = new('kerosin/SceneServer', '/sys/server/scene');
sceneServer.createScene('/usr/scene');

#
# setup the MonitorServer and a simple MonitorSystem
monitorServer = new('oxygen/MonitorServer', '/sys/server/monitor');
monitorServer.registerMonitorSystem('SexpMonitor');

#
# setup the GameControlServer
gameControlServer = new('oxygen/GameControlServer', '/sys/server/gamecontrol');
gameControlServer.initParser('SexpParser');
gameControlServer.initEffector('CreateEffector');
gameControlServer.initControlAspect('GameStateAspect');

#
# setup the SpadesServer
spadesServer = new('oxygen/SpadesServer', '/sys/server/spades');

#
# set the time of a single simulation step
Spades.TimePerStep = 0.01

# 
# scene setup
#

#
# create world and space aspects
world = new('kerosin/World', '/usr/scene/_world');
world.setGravity(0.0, -9.81, 0.0);
new('kerosin/Space', '/usr/scene/_space');

#
# setup all arena colliders

# setup soccer field sizes
fieldLength = 52.5;
fieldWidth  = 34.0;
goalWidth   = 7.0
goalDepth   = 2.0
goalHeight  = 1.80

#
# floor and wall collider
addWall('floor',0.0, 1.0 ,0.0, 0.0); 
addWall('w1', 1.0,  0.0,  0.0, -fieldLength/2.0);
addWall('w2',-1.0,  0.0,  0.0, -fieldLength/2.0);  
addWall('w3', 0.0,  0.0,  1.0, -fieldWidth/2.0);  
addWall('w4', 0.0,  0.0, -1.0, -fieldWidth/2.0);  

#
# mark the soccer field with 4 field flags
addFlag('flag1',-fieldLength/2.0, 0.0, fieldWidth/2.0);
addFlag('flag2', fieldLength/2.0, 0.0, fieldWidth/2.0);
addFlag('flag3', fieldLength/2.0, 0.0,-fieldWidth/2.0);
addFlag('flag4',-fieldLength/2.0, 0.0,-fieldWidth/2.0);

#
# mark the left goal
addFlag('goal1l',-fieldLength/2,0.0,-goalWidth/2.0);
addFlag('goal1r',-fieldLength/2,0.0,+goalWidth/2.0);

#
# mark the right goal
addFlag('goal2l',fieldLength/2,0.0,-goalWidth/2.0);
addFlag('goal2r',fieldLength/2,0.0,+goalWidth/2.0);

# gLeft = new('kerosin/BoxCollider','/usr/scene/goal1');
# gLeft.setPosition(-fieldLength/2,0.0,-goalWidth/2.0);
# gLeft.setBoxLengths(goalDepth,goalHeight,goalWidth);

# gRight = new('kerosin/BoxCollider','/usr/scene/goal1');
# gRight.setPosition(fieldLength/2,0.0,-goalWidth/2.0);
# gRight.setBoxLengths(goalDepth,goalHeight,goalWidth);

#
# game Setup
#

# agent parameters
# Agent.Mass = 75.0
# Agent.Radius = 1.0
# Agent.MaxSpeed = 10.0

#
# put a ball on the soccer field
ball = new('Ball','/usr/scene/ball');
ball.setLocalPos(0.0,0.0,0.0);

#
# queue agents for startup
spadesServer.queueAgents('default', 3);

