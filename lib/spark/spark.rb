#
# spark.rb, setup kerosin application framework
#

#
# import required plugins
importBundle 'filesystemstd';

#
# scene and server path
$scenePath = '/usr/scene/'
$serverPath = '/sys/server/'

#
# setup the PhysicsServer
new('oxygen/PhysicsServer', $serverPath+'physics')

#
# setup the SceneServer

sceneServer = new('oxygen/SceneServer', $serverPath+'scene')
sceneServer.createScene($scenePath)

# use the ruby scene importer to import scenes
importBundle 'rubysceneimporter'
sceneServer.initSceneImporter("RubySceneImporter");

#
# setup the kerosin render framework
new('kerosin/OpenGLServer', $serverPath+'opengl');
new('kerosin/MaterialServer', $serverPath+'material');
new('kerosin/RenderServer', $serverPath+'render');

#
# setup the InputServer

# use the SDL input system and use a german keyboard layout
importBundle 'inputsdl';
inputServer = new('kerosin/InputServer', $serverPath+'input')
inputServer.setScanCodeMapping('german.scan.rb');
inputServer.init('InputSystemSDL')

# add devices
inputServer.createDevice('Timer')
inputServer.createDevice('Keyboard')
inputServer.createDevice('Mouse')

# setup the FontServer
new('kerosin/FontServer', $serverPath+'font');

# create world and space aspects
world = new('oxygen/World', $scenePath+'world')
world.setGravity(0.0, 0.0, -9.81)
new('oxygen/Space', $scenePath+'space')

#
# below is a set of utility functions for the user app
#

# add a camera with an FPS Controller to the scene at path
def sparkAddFPSCamera(
		      path,
		      x = 0.0, y = 0.0, z = 4,
		      maxSpeed = 15.0,
		      accel = 40.0,
		      drag = 4,
		      addCollider = false,
		      colliderRadius = 2.0
		      )

  # add a camera. The camera movement is controlled using an
  # FPSController.
  cameraTransform = new('oxygen/Transform',path)
  cameraTransform.setLocalPos(x,y,z)
  new('oxygen/Camera',path+'/camera')

  # the camera is not affected by gravity but restricted to a maximum
  # speed
  body = new('oxygen/Body',path+'/physics')
  body.useGravity(false);
  velCtr = new('oxygen/VelocityController',path+'/physics/velctr')
  velCtr.setMaxVelocity(maxSpeed)

  # add an FPSController to move the camera and set the applied
  # acceleration
  fpsController = new('oxygen/FPSController',path+'/physics/controller')
  fpsController.setAcceleration(accel)

  # add an DragController to work against the camera acceleration
  dragController = new('oxygen/DragController',path+'/physics/drag')
  dragController.setLinearDrag(drag)

  if addCollider
    # add an Collider that allows the camera to push objects
    collider = new('oxygen/SphereCollider', path+'/geometry')
    collider.setRadius(colliderRadius)    

    handler = new('oxygen/ContactJointHandler', path+'geometry/contact')
    handler.setContactBounceMode true
    handler.setMinBounceVel 2
  end
end











