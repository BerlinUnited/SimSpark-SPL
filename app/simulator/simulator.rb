importBundle "filesystemstd"
importBundle "sexpparser"

importBundle "perfectvisionperceptor"
importBundle "forceeffector"

# import plugins specific to the soccer simulation
importBundle "soccer"

#
# Init
#

# mount a standard file system
fileServer = get('/sys/server/file');
fileServer.mount('FileSystemSTD', 'data/');

# setup the PhysicsServer
new('kerosin/PhysicsServer', '/sys/server/physics');

# setup the SceneServer
sceneServer = new('kerosin/SceneServer', '/sys/server/scene');
sceneServer.createScene('/usr/scene');

# setup the MonitorServer and a simple MonitorSystem
monitorServer = new('oxygen/MonitorServer', '/sys/server/monitor');
monitorServer.registerMonitorSystem('SexpMonitor');

# setup the GameControlServer
gameControlServer = new('oxygen/GameControlServer', '/sys/server/gamecontrol');
gameControlServer.initParser('SexpParser');
gameControlServer.initEffector('CreateEffector');
gameControlServer.initControlAspect('GameStateAspect');

# setup the SpadesServer
spadesServer = new('oxygen/SpadesServer', '/sys/server/spades');

# set the time of a single simulation step
Spades.TimePerStep = 0.01

# 
# Scene setup
#

# agent parameters
# Agent.Mass = 75.0
# Agent.Radius = 1.0
# Agent.MaxSpeed = 10.0

# create world and space aspects
world = new('kerosin/World', '/usr/scene/_world');
world.setGravity(0.0, -9.81, 0.0);
new('kerosin/Space', '/usr/scene/_space');

# setup all arena colliders

# floor collider
pc = new('kerosin/PlaneCollider', '/usr/scene/pc');
pc.setParams(0.0, 1.0 ,0.0, 0.0); 

# wall collider
w1 = new('kerosin/PlaneCollider', '/usr/scene/w1');
w1.setParams(-1.0, 0.0, 0.0, -52.5);

w2 = new('kerosin/PlaneCollider', '/usr/scene/w2');
w2.setParams(1.0, 0.0, 0.0, -52.5);

w3 = new('kerosin/PlaneCollider', '/usr/scene/w3');
w3.setParams(0.0, 0.0, -1.0, -34.0);

w4 = new('kerosin/PlaneCollider', '/usr/scene/w4');
w4.setParams(0.0, 0.0, 1.0, -34.0);

# mark the soccer field with 4 field flags
fx = 52.5;
fz = 34.0;
fy = 1.0;

flag = new('FieldFlag','/usr/scene/flag1');
flag.setLocalPos(-fx,fy,fz);

flag = new('FieldFlag','/usr/scene/flag2');
flag.setLocalPos(fx,fy,fz);

flag = new('FieldFlag','/usr/scene/flag3');
flag.setLocalPos(fx,fy,-fz);

flag = new('FieldFlag','/usr/scene/flag4');
flag.setLocalPos(-fx,fy,-fz);

spadesServer.queueAgents('default', 3);
