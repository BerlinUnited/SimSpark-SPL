#
# spark.rb, setup kerosin application framework
#
 
#
# define constants used to setup spark
#

# (MonitorControl) constants
# 

# define the monitor update interval in cylcles
$monitorInterval = 10;
$serverType = 'tcp'
$serverPort = 3200

# (SparkMonitorClient) constants
# 
$monitorServer = '127.0.0.1'
$monitorPort = 3200

# socket type ('tcp' or 'udp')
$monitorType = 'tcp'

#
# below is a set of utility functions for the user app
#
def sparkSetupMonitor
  print "(spark.rb) sparkSetupMonitor\n"

  # add the agent control node
  simulationServer = get($serverPath+'simulation');

  monitorClient = new('SparkMonitorClient', 
		      $serverPath+'simulation/SparkMonitorClient')
  monitorClient.setServer($monitorServer)
  monitorClient.setPort($monitorPort)
  
  if ($monitorType == 'udp')
    monitorClient.setClientTypeUDP()
  else if ($monitorType == 'tcp')
	 monitorClient.setClientTypeTCP()
       else
	 print "(spark.rb) unknown monitor socket type "
	 print $monitorType
	 print "\n"
       end
  end

  rubySceneImporter = get($serverPath+'scene/RubySceneImporter')
  rubySceneImporter.setUnlinkOnCompleteScenes(true);
end

#
# install a class below the SparkMonitorClient that implementes the
# simulation specific monitor processing
# 
def sparkRegisterCustomMonitor(className)
  print "(spark.rb) sparkRegisterCustomMonitor " + className + "\n"
  new(className, $serverPath+'simulation/SparkMonitorClient/'+className)
end

#
# install a class below the Spark RenderControl node that implements
# application specific render logic
#
def sparkRegisterCustomRender(className)
  print "(spark.rb) sparkRegisterCustomRender " + className + "\n"
  new(className, $serverPath+'simulation/RenderControl/'+className)
end

#
# install a class below the Spark InputControl node that implements
# application specific input processing
#
def sparkRegisterCustomInput(className)
  print "(spark.rb) sparkRegisterCustomInput " + className + "\n"
  new(className, $serverPath+'simulation/InputControl/'+className)
end

#
# install a class below the Spark MonitorSystem node that parses
# commands received from a monitor client
#
def sparkRegisterMonitorCmdParser(className)
  print "(spark.rb) sparkRegisterMonitorCmdParser " + className + "\n"
  new(className, $serverPath+'monitor/SparkMonitor/'+className)
end
  
def sparkSetupServer
  print "(spark.rb) sparkSetupServer\n"

  # add the agent control node
  simulationServer = get($serverPath+'simulation');
  simulationServer.initControlNode('oxygen/AgentControl','AgentControl')  

  monitorControl = new('oxygen/MonitorControl',
		       $serverPath+'simulation/MonitorControl')
  monitorControl.setMonitorInterval($monitorInterval)
  monitorControl.setServerPort($serverPort)

  if ($serverType == 'udp')
    monitorControl.setServerTypeUDP()
  else if ($serverType == 'tcp')
	 monitorControl.setServerTypeTCP()
       else
	 print "(spark.rb) unknown monitor socket type "
	 print $serverType
	 print "\n"
       end
  end
end

def sparkSetupRendering
  print "(spark.rb) sparkSetupRendering\n"
  #
  # setup the GeometryServer
  geometryServer = new('oxygen/GeometryServer', $serverPath+'geometry')
  importBundle 'voidmeshimporter'
  geometryServer.initMeshImporter("VoidMeshImporter");
  
  #
  # setup the kerosin render framework
  new('kerosin/OpenGLServer', $serverPath+'opengl');
  new('kerosin/RenderServer', $serverPath+'render');
  new('kerosin/ImageServer', $serverPath+'image');
  new('kerosin/TextureServer', $serverPath+'texture');

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

  # 
  # register render control node to the simulation server
  simulationServer = get($serverPath+'simulation');
  simulationServer.initControlNode('kerosin/RenderControl','RenderControl')
end

def sparkSetupInput()
  print "(spark.rb) sparkSetupInput\n"

  # 
  # register input control node to the simulation server
  simulationServer = get($serverPath+'simulation');

  # add the input control node
  simulationServer.initControlNode('kerosin/InputControl','InputControl')
end

# add a camera with an FPS Controller to the scene at path
def sparkAddFPSCamera(
		      path,
		      x = 0.0, y = 0.0, z = 4,
                      vAngle = 45.0,
                      hAngle = 10.0,
		      maxSpeed = 15.0,
		      accel = 30.0,
		      drag = 4,
		      addCollider = false,
		      colliderRadius = 2.0
		      )

  print "(spark.rb) sparkAddFPSCamera\n"

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
  fpsController.setVAngle(vAngle)
  fpsController.setHAngle(hAngle)
  inputControl = get($serverPath+'simulation/InputControl')
  inputControl.setFPSController(path+'/physics/controller')

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

#
# registers a new logTarget to the LogServer
#
# valid logTarget value are ':cout' and ':cerr' for stdout and stderr
# output, as well as any other file name, e.g. 'log.txt'
#
# logType denotes the debug to be attached to the
# logtarget. Valid streams are 'eNone', 'eDebug' and 'eWarning'
#
def sparkEnableLog(logTarget, logType)
  print "(spark.rb) sparkEnableLog logTarget="+logTarget+" logType="+logType+"\n"

  logServer = get($serverPath+'log')
  logServer.addStream(logTarget, logType)
end

# logs all error output to cerr
def sparkLogErrorToCerr()
  sparkEnableLog(':cerr', 'eError')
end

# logs all debug output to cerr
def sparkLogDebugToCerr()
  sparkEnableLog(':cerr', 'eDebug')
end

# logs all output to cerr
def sparkLogAllToCerr()
  sparkEnableLog(':cerr', 'eAll')
end

# logs all error output to 'fileName'
def sparkLogErrorToFile(fileName)
  sparkEnableLog(fileName, 'eError')
end

# logs all debug output to 'fileName'
def sparkLogDebugToFile(fileName)
  sparkEnableLog(fileName, 'eDebug')
end

# logs all output to 'fileName'
def sparkLogAllToFile(fileName)
  sparkEnableLog(fileName, 'eAll')
end

#
# setup spark
#

#
# scene and server path
$scenePath = '/usr/scene/'
$serverPath = '/sys/server/'

#
# set up logging
sparkLogErrorToCerr()
#sparkLogAllToCerr()

#
# setup the PhysicsServer
new('oxygen/PhysicsServer', $serverPath+'physics')

#
# setup the SceneServer
sceneServer = new('oxygen/SceneServer', $serverPath+'scene')
sceneServer.createScene($scenePath)

#
# setup the GeometryServer
geometryServer = new('oxygen/GeometryServer', $serverPath+'geometry')
importBundle 'voidmeshimporter'
geometryServer.initMeshImporter("VoidMeshImporter");

# use the ruby scene importer to import scenes
importBundle 'rubysceneimporter'
sceneServer.initSceneImporter("RubySceneImporter");

# create world and space aspects
world = new('oxygen/World', $scenePath+'world')
world.setGravity(0.0, 0.0, -9.81)
world.setCFM(0.001)
world.setAutoDisableFlag(true)
world.setContactSurfaceLayer(0.001)

new('oxygen/Space', $scenePath+'space')

#
# setup the MaterialServer
new('kerosin/MaterialServer', $serverPath+'material');

#
# setup the MonitorServer
monitorServer = new('oxygen/MonitorServer', $serverPath+'monitor')

# register the SparkMonitor system
importBundle 'sparkmonitor'
monitorServer.registerMonitorSystem('SparkMonitor')

#
# setup the GameControlServer
gameControlServer = new('oxygen/GameControlServer', $serverPath+'gamecontrol')

# register the s-expression parser to the GameControlServer
importBundle "sexpparser"
gameControlServer.initParser('SexpParser')

# register the SceneEffector to the GameControlServer
importBundle "sceneeffector"
gameControlServer.initEffector('SceneEffector')

#
# setup the SimulationServer
simulationServer = new('oxygen/SimulationServer', $serverPath+'simulation')

#
# import the spark perceptors and effector set
importBundle "sparkagent"











