importBundle "filesystemstd"

#
# Init
#

# mount a standard file system
fileServer = get ('/sys/server/file');
fileServer.mount ('FileSystemSTD', 'data/');

# setup the PhysicsServer
new ('kerosin/PhysicsServer', '/sys/server/physics');

# setup the SceneServer
sceneServer = new ('kerosin/SceneServer', '/sys/server/scene');
sceneServer.createScene('/usr/scene');

# 
# Scene setup
#
cd ('/usr/scene');

# create world and space aspects
world = new ('kerosin/World', '_world');
world.setGravity(0.0, -9.81, 0.0);
new ('kerosin/Space', '../_space');

# setup all arena colliders
cd ('/usr/scene/arena')

# floor collider
pc = new ('kerosin/PlaneCollider', '../pc');
pc.setParams(0.0, 1.0 ,0.0, 0.0); 

# wall collider
w1 = new ('kerosin/PlaneCollider', '../w1');
w1.setParams(-1.0, 0.0, 0.0, -25.0);

cd ('/usr/scene/arena')
w2 = new ('kerosin/PlaneCollider', '../w2');
w2.setParams(1.0, 0.0, 0.0, -25.0);

cd ('/usr/scene/arena')
w3 = new ('kerosin/PlaneCollider', '../w3');
w3.setParams(0.0, 0.0, -1.0, -25.0);

cd ('/usr/scene/arena')
w4 = new ('kerosin/PlaneCollider', '../w4');
w4.setParams(0.0, 0.0, 1.0, -25.0);

# add a sphere collider
cd ('/usr/scene');
trans = new ('kerosin/Transform', 'sphere');
trans.setLocalPos(-12.5, 10.0, -12.5);
physics = new ('kerosin/Body', '/usr/scene/sphere/_physics');
physics.setSphere(1.0, 1.0);
physics.setMass(1.0);
physics.setMaxSpeed(3.0);
geometry = new ('kerosin/SphereCollider', '/usr/scene/sphere/_geometry');
geometry.setRadius(1.0);










