importBundle "sexpparser"
importBundle "filesystemstd"
importBundle "monitortest"
importBundle "spadestest"

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
monitorServer.registerMonitorSystem('MonitorTest');

# setup the GameControlServer
gameControlServer = new('oxygen/GameControlServer', '/sys/server/gamecontrol');
gameControlServer.initParser('SexpParser');

# setup the SpadesServer
spadesServer = new('oxygen/SpadesServer', '/sys/server/spades');

# test perceptor
print "\n##### Creating a Gadgetron PerfectVision 360 perceptor #####\n\n";
vision360 = new('oxygen/PerfectVisionPerceptor', '/v360');

# set the time of a single simulation step
Spades.TimePerStep = 0.01

# 
# Scene setup
#

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
w1.setParams(-1.0, 0.0, 0.0, -25.0);

w2 = new('kerosin/PlaneCollider', '/usr/scene/w2');
w2.setParams(1.0, 0.0, 0.0, -25.0);

w3 = new('kerosin/PlaneCollider', '/usr/scene/w3');
w3.setParams(0.0, 0.0, -1.0, -25.0);

w4 = new('kerosin/PlaneCollider', '/usr/scene/w4');
w4.setParams(0.0, 0.0, 1.0, -25.0);

# add a sphere collider
trans = new('kerosin/Transform', '/usr/scene/sphere');
trans.setLocalPos(-12.5, 50.0, -12.5);
physics = new('kerosin/Body', '/usr/scene/sphere/_physics');
physics.setSphere(1.0, 1.0);
physics.setMass(10.0);
physics.setMaxSpeed(13.0);
geometry = new('kerosin/SphereCollider', '/usr/scene/sphere/_geometry');
geometry.setRadius(1.0);
agent = new('TestAgentAspect', '/usr/scene/sphere/_agent');

# a second collider
trans = new('kerosin/Transform', '/usr/scene/sphere2');
trans.setLocalPos(-12.5, 9.0, -12.4);
physics = new('kerosin/Body', '/usr/scene/sphere2/_physics');
physics.setSphere(1.0, 1.0);
physics.setMass(1.0);
physics.setMaxSpeed(4.0);
geometry = new('kerosin/SphereCollider', '/usr/scene/sphere2/_geometry');
geometry.setRadius(1.0);
 
