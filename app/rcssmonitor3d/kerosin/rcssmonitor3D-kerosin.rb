#
# rcssmonitor3D-kerosin.rb
#

# import required plugins
importBundle 'filesystemstd';
importBundle 'inputsdl';

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

# add a camera
cameraTransform = new('oxygen/Transform',$scenePath+'camera0')
cameraTransform.setLocalPos(0.0,0.0,10.0)
new('oxygen/Camera',$scenePath+'camera0/camera')
body = new('oxygen/Body',$scenePath+'camera0/physics')
body.useGravity(false);
new('oxygen/FPSController',$scenePath+'camera0/physics/controller')

#
# setup a dummy scene
#
def addVisual(className, nodeName, x, y, z)
  transform = new('oxygen/Transform', $scenePath+nodeName)
  transform.setLocalPos(x,y,z)
  new('kerosin/'+className,$scenePath+nodeName+'/'+'visual')
end

# add an axis
addVisual('Axis','myAxis',0.0,0.0,0.0)

# add some spheres
addVisual('Sphere','sphere1',0.0,0.0,0.0);

addVisual('Sphere','sphere2',5.0,0.0,0.0);
addVisual('Sphere','sphere3',-5.0,0.0,0.0);

addVisual('Sphere','sphere4',0.0,5.0,0.0);
addVisual('Sphere','sphere5',0.0,-5.0,0.0);

addVisual('Sphere','sphere6',0.0,0.0,5.0);
addVisual('Sphere','sphere7',0.0,0.0,-5.0);






