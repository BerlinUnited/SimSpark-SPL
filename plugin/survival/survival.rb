# Import classes of this simulation description
importBundle('survival/survival');

# create the control aspect
cd '/usr/scene'

controlAspect = new ('SurvivalControlAspect', '_control');


cd ('/usr/scene');

# create world and space aspects
world = new ('kerosin/World', '_world');
world.setGravity(0.0, -9.81, 0.0);
new ('kerosin/Space', '../_space');

# setup camera
trans = new ('kerosin/Transform', '../camera');
trans.setLocalPos(0.0, 0.0, 10.0);
new ('kerosin/Camera', 'camera');
body = new ('kerosin/Body', '../_physics');
body.useGravity(false);
new ('kerosin/FPSController', 'fps');
light = new ('kerosin/Light', '../../_light');
light.setRadius(50.0);
light.setDiffuseColor(1.0, 1.0, 1.0);
collider = new ('kerosin/SphereCollider', '../_geometry');
collider.setRadius(2.0);

# add arena
cd ('/usr/scene');
trans = new ('kerosin/Transform', 'arena');
trans.setLocalPos(0.0, 0.0, 0.0);
mesh = new ('kerosin/StaticMesh', '_visual');
mesh.load('model/arena.void');
# floor
pc = new ('kerosin/PlaneCollider', '../pc');
pc.setParams(0.0, 1.0 ,0.0, 0.0); 
#lights
trans = new ('kerosin/Transform', '../../light0');
trans.setLocalPos(0.0, 15.0, 0.0);
light = new ('kerosin/Light', '_light');
light.setRadius(80.0);
light.setDiffuseColor(1.0, 1.0, 1.0);



# add slow
cd ('/usr/scene');
trans = new ('kerosin/Transform', 'slow');
trans.setLocalPos(-12.5, 1.0, -12.5);
visual = new ('kerosin/StaticMesh', '_visual');
visual.load('model/slow.void');
physics = new ('kerosin/Body', '../_physics');
physics.setSphere(1.0, 1.0);
physics.setMass(1.0);
physics.setMaxSpeed(3.0);
geometry = new ('kerosin/SphereCollider', '../_geometry');
geometry.setRadius(1.0);
#light = new ('kerosin/Light', '../_light');
#light.setRadius(5.0);
#light.setDiffuseColor(0.0, 1.0, 0.0);

agent = new ('SurvivalAgentAspect', '../_agent');
#agent.setWanderingStrength(5.0);
#agent.setWanderingRate(2.0);

# add fast
cd ('/usr/scene');
trans = new ('kerosin/Transform', 'fast');
trans.setLocalPos(12.5, 1.0, 12.5);
visual = new ('kerosin/StaticMesh', '_visual');
visual.load('model/fast.void');
physics = new ('kerosin/Body', '../_physics');
physics.setSphere(1.0, 1.0);
physics.setMass(1.0);
physics.setMaxSpeed(10.0);
geometry = new ('kerosin/SphereCollider', '../_geometry');
geometry.setRadius(1.0);
#light = new ('kerosin/Light', '../_light');
#light.setRadius(5.0);
#light.setDiffuseColor(1.0, 0.0, 0.0);

agent = new ('SurvivalAgentAspect', '../_agent');
#agent.setWanderingStrength(10.0);
#agent.setWanderingRate(1.0);

# add food
cd ('/usr/scene');
trans = new ('kerosin/Transform', 'food');
trans.setLocalPos(0.0, 1.0, 0.0);
visual = new ('kerosin/StaticMesh', '_visual');
visual.load('model/food.void');
