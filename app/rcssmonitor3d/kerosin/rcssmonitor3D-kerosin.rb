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
cameraTransform.setLocalPos(0.0,0.0,10.0)
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

def addSphere(nodeName, radius)
  transform = new('oxygen/Transform', $scenePath+nodeName)
  sphere = new('kerosin/Sphere',$scenePath+nodeName+'/'+'visual')
  sphere.setRadius(radius)
end  

# called from the monitor to create an agent named <nodeName>
def addAgent(nodeName)
  addSphere(nodeName,Soccer.AgentRadius)
end

# called from the monitor to create a ball
def addBall(nodeName)
  addSphere(nodeName,Soccer.BallRadius)
end

# add an axis
addVisual('Axis','myAxis',0.0,0.0,0.0)






