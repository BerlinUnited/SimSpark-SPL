#
# rcssmonitor3D-kerosin.rb
#

# import required plugins
importBundle 'filesystemstd';
importBundle 'inputsdl';
importBundle 'sexpparser'

# scene and server path
$scenePath = '/usr/scene/'
$serverPath = '/sys/server/'

# setup the PhysicsServer
new('oxygen/PhysicsServer', $serverPath+'physics')

# setup the SceneServer
sceneServer = new('oxygen/SceneServer', $serverPath+'scene')
sceneServer.createScene($scenePath)

# setup the OpenGLServer
new('kerosin/OpenGLServer', $serverPath+'opengl');

# setup the MaterialServer
new('kerosin/MaterialServer', $serverPath+'material');
material = new('kerosin/MaterialSolid', $serverPath+'material/matAgentN');
material.setColor(0.8,0.8,0.2)

material = new('kerosin/MaterialSolid', $serverPath+'material/matAgentL');
material.setColor(1.0,0.2,0.2)

material = new('kerosin/MaterialSolid', $serverPath+'material/matAgentR');
material.setColor(0.2,0.2,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matBall');
material.setColor(1.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matFlag');
material.setColor(1.0,0.0,0.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGround');
material.setColor(0.1,0.5,0.1)

# setup the RenderServer
new('kerosin/RenderServer', $serverPath+'render');

# setup the InputServer
inputServer = new('kerosin/InputServer', $serverPath+'input')
inputServer.init('InputSystemSDL')
inputServer.createDevice('Timer')
inputServer.createDevice('Keyboard')
inputServer.createDevice('Mouse')

# setup the FontServer
new('kerosin/FontServer', $serverPath+'font');

# setup the SceneServer
sceneServer = new('oxygen/SceneServer', $serverPath+'scene')
sceneServer.createScene($scenePath)

# create world and space aspects
world = new('oxygen/World', $scenePath+'world')
world.setGravity(0.0, 0.0, -9.81)
new('oxygen/Space', $scenePath+'space')

# add a camera. The camera movement is controlled using an
# FPSController.
cameraTransform = new('oxygen/Transform',$scenePath+'camera0')
cameraTransform.setLocalPos(0.0,10.0,0.0)
new('oxygen/Camera',$scenePath+'camera0/camera')

# the camera is not affected by gravity but restricted to a maximum
# speed
body = new('oxygen/Body',$scenePath+'camera0/physics')
body.useGravity(false);
body.setMaxSpeed(15.0)

# add an FPSController to move the camera and set the applied
# acceleration
fpsController = new('oxygen/FPSController',$scenePath+'camera0/physics/controller')
fpsController.setAcceleration(40.0)

# setup the CommServer
new('rcssmonitor3d/CommServer', $serverPath+'comm')

# setup the MonitorParser
new('rcssmonitor3d/MonitorParser', $serverPath+'parser')

#
# collection of callbacks and helper functions
#

# create a transform node and a node of class <className>
def addVisual(className, nodeName, x, y, z)
  transform = new('oxygen/Transform', $scenePath+nodeName)
  transform.setLocalPos(x,y,z)
  new('kerosin/'+className,$scenePath+nodeName+'/'+'visual')
end

def addSphere(nodeName, radius, material)
  transform = new('oxygen/Transform', $scenePath+nodeName)
  sphere = new('kerosin/Sphere',$scenePath+nodeName+'/'+'visual')
  sphere.setRadius(radius)
  sphere.setMaterial(material)
end  

# called from the monitor to create an agent named <nodeName>
def addAgentN(nodeName)
  addSphere(nodeName,Soccer.AgentRadius,'matAgentN')
end

def addAgentL(nodeName)
  addSphere(nodeName,Soccer.AgentRadius,'matAgentL')
end

def addAgentR(nodeName)
  addSphere(nodeName,Soccer.AgentRadius,'matAgentR')
end

# called from the monitor to create a ball
def addBall(nodeName)
  addSphere(nodeName,Soccer.BallRadius,'matBall')
end

# called from the monitor to create a flag
def addFlag(nodeName)
  addSphere(nodeName,1.0,'matFlag')
end

# called from the monitor to create the playing field after the
# dimensions are received
def addField()
  # create the green
  fieldThickness = 0.5

  transform = new('oxygen/Transform', $scenePath+'field')
  transform.setLocalPos(0.0,-fieldThickness/2.0,0.0)

  field = new('kerosin/Box',$scenePath+'field/visual')
  field.setMaterial('matGround')

  field.setExtents(110.0,0.2,73.0)
end  

# add an axis
addVisual('Axis','myAxis',0.0,0.0,0.0)







