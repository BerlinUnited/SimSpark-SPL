#
# import needed bundles
#

#
# generic plugins
importBundle "filesystemstd"
importBundle "sexpparser"
importBundle "perfectvisionperceptor"

#
# bundle of soccer plugins
importBundle "soccer"

#
# setup gloabal constants
#

# the soccer field dimensions in meters
$fieldLength = 105.0
$fieldWidth  = 68.0
$fieldHeight = 20.0
$goalWidth   = 7.32
$goalDepth   = 2.0
$goalHeight  = 2.44
$borderSize  = 4.0

# scene and server path
$scenePath = '/usr/scene/'
$serverPath = '/sys/server/'

# agent parameters
$agentMass     = 75.0
$agentRadius   = 0.3
$agentMaxSpeed = 10.0

#
# declare some helper methods
#

# add an agent. this function is called from the CreateEffector with
# the full path of the AgentAspect

# the start positions of the agents
$agentX = -12.0
$agentY = 1.0
$agentZ = -7.0

def addAgent(aspectPath)
  # move different agents away from each other
  aspect = get(aspectPath)
  aspect.setLocalPos($agentX,$agentY,$agentZ)
  $agentX += 2.5
  $agentY += 1.0
  $agentZ += 3.0

  # geometry and physics setup
  physics = new('kerosin/Body', aspectPath+'physics')
  physics.setMass($agentMass)
  physics.setMaxSpeed($agentMaxSpeed)

  geometry = new('kerosin/SphereCollider', aspectPath+'geometry')
  geometry.setRadius($agentRadius)

  # effector setup
  new('InitEffector', aspectPath+'InitEffector')
  new('DashEffector', aspectPath+'DashEffector')
  new('KickEffector', aspectPath+'KickEffector')

  # perceptor setup
  new('PerfectVisionPerceptor', aspectPath+'PerfectVisionPerceptor')
  new('VisionPerceptor', aspectPath+'VisionPerceptor')

  # agent state
  new('AgentState', aspectPath+'AgentState')
end

# add a field flag to (x,y,z)
def addFlag(name,x,y,z)
  flag = new('FieldFlag',$scenePath+name)
  flag.setLocalPos(x,y,z)
end

# add a plane collider with ax+by+cz=d
def addWall(name,a,b,c,d)
  wall = new('kerosin/PlaneCollider', $scenePath+name)
  wall.setParams(a,b,c,d)
end

# construct a ball at (x,y,z)
def addBall(x,y,z)
  path = $scenePath+'Ball/'

  ball = new('Ball',path)
  ball.setLocalPos(x,y,z)

  physics = new('kerosin/Body', path+'physics')
  physics.setMass(0.4)

  geomPath = path+'geometry/'

  geometry = new('kerosin/SphereCollider', geomPath)
  geometry.setRadius(0.111)

  new('kerosin/RecorderHandler', geomPath+'recorder')
  new('kerosin/ContactJointHandler', geomPath+'contact')
end

def addField()
  #
  # setup all arena colliders

  #
  # floor and wall collider- infinite planes used as the the hard
  # barrier
  addWall('Floor',0.0, 1.0 ,0.0, 0.0) 
  addWall('W1', 1.0,  0.0,  0.0, -$fieldLength/2.0 - $borderSize)
  addWall('W2',-1.0,  0.0,  0.0, -$fieldLength/2.0 - $borderSize)  
  addWall('W3', 0.0,  0.0,  1.0, -$fieldWidth/2.0  - $borderSize)  
  addWall('W4', 0.0,  0.0, -1.0, -$fieldWidth/2.0  - $borderSize)  

  #
  # mark the soccer field with 4 field flags
  addFlag('Flag1',-$fieldLength/2.0, 0.0, $fieldWidth/2.0)
  addFlag('Flag2', $fieldLength/2.0, 0.0, $fieldWidth/2.0)
  addFlag('Flag3', $fieldLength/2.0, 0.0,-$fieldWidth/2.0)
  addFlag('Flag4',-$fieldLength/2.0, 0.0,-$fieldWidth/2.0)

  #
  # mark the left goal
  addFlag('Goal1l',-$fieldLength/2,0.0,-$goalWidth/2.0)
  addFlag('Goal1r',-$fieldLength/2,0.0,+$goalWidth/2.0)

  #
  # mark the right goal
  addFlag('Goal2l',$fieldLength/2,0.0,-$goalWidth/2.0)
  addFlag('Goal2r',$fieldLength/2,0.0,+$goalWidth/2.0)

  #
  # box collider around the playing field
  fieldBox = new('kerosin/BoxCollider',$scenePath+'fieldBox')
  fieldBox.setPosition(-$fieldLength/2.0,0.0,-$fieldWidth/2.0)
  fieldBox.setBoxLengths($fieldLength,$fieldHeight,$fieldWidth)

  # gLeft = new('kerosin/BoxCollider','/usr/scene/goal1')
  # gLeft.setPosition(-$fieldLength/2,0.0,-goalWidth/2.0)
  # gLeft.setBoxLengths(goalDepth,goalHeight,goalWidth)

  # gRight = new('kerosin/BoxCollider','/usr/scene/goal1')
  # gRight.setPosition($fieldLength/2,0.0,-goalWidth/2.0)
  # gRight.setBoxLengths(goalDepth,goalHeight,goalWidth)
end

#
# init
#

#
# mount a standard file system
fileServer = get($serverPath+'file')
fileServer.mount('FileSystemSTD', 'data/')

#
# setup the PhysicsServer
new('kerosin/PhysicsServer', $serverPath+'physics')

#
# setup the SceneServer
sceneServer = new('kerosin/SceneServer', $serverPath+'scene')
sceneServer.createScene($scenePath)

#
# setup the MonitorServer and a simple MonitorSystem
monitorServer = new('oxygen/MonitorServer', $serverPath+'monitor')
monitorServer.registerMonitorSystem('SexpMonitor')

#
# setup the GameControlServer
gameControlServer = new('oxygen/GameControlServer', $serverPath+'gamecontrol')
gameControlServer.initParser('SexpParser')
gameControlServer.initEffector('CreateEffector')

#
# setup the SpadesServer
spadesServer = new('oxygen/SpadesServer', $serverPath+'spades')

# 
# scene setup
#

#
# create world and space aspects
world = new('kerosin/World', $scenePath+'world')
world.setGravity(0.0, -9.81, 0.0)
new('kerosin/Space', $scenePath+'space')

# construct the playing field
addField()

#
# game Setup
#

# set the time of a single simulation step
Spades.TimePerStep = 0.01

#
# put a ball on the soccer field
addBall(0.0,1.0,0.0)

#
# register game control aspects
gameControlServer.initControlAspect('GameStateAspect')
gameControlServer.initControlAspect('BallStateAspect')

#
# queue agents for startup
spadesServer.queueAgents('default', 3)
