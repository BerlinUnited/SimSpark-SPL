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

# soccer namespace
$soccerNameSpace = "Soccer"

# scene and server path
$scenePath = '/usr/scene/'
$serverPath = '/sys/server/'

#
# declare some helper methods
#

# add an agent. this function is called from the CreateEffector with
# the full path of the AgentAspect

# the start positions of the agents
$agentX = -2.5
$agentY = 0.0
$agentZ = 0.5

# register a variable in the soccer namespace
def addSoccerVar(name, value)
   createVariable($soccerNameSpace, name, value)
end

# helper to get the value of a variable in the soccer namespace
def getSoccerVar(name)
  eval <<-EOS 
    #{$soccerNameSpace}.#{name} 
  EOS
end

def addAgent(aspectPath)
  # move different agents away from each other
  aspect = get(aspectPath)
  aspect.setLocalPos($agentX,$agentZ,$agentY)
  $agentX += 5.0
  $agentY += 0.0
  $agentZ += 0.1

  # geometry and physics setup
  physics = new('kerosin/Body', aspectPath+'physics')
  physics.setMass(getSoccerVar('AgentMass'))
  physics.setMaxSpeed(getSoccerVar('AgentMaxSpeed'))

  geometry = new('kerosin/SphereCollider', aspectPath+'geometry')
  geometry.setRadius(getSoccerVar('AgentRadius'))

  # effector setup
  new('InitEffector', aspectPath+'InitEffector')
  dashEffector = new('DashEffector', aspectPath+'DashEffector')
  dashEffector.setForceFactor(450.0);
  dashEffector.setSigma(0.5);
  kickEffector = new('KickEffector', aspectPath+'KickEffector')
  kickEffector.setForceFactor(4.0)
  kickEffector.setNoiseParams(0.4,0.02,0.025)
  kickEffector.setSteps(3,75)
  kickEffector.setMaxPower(100.0)

  # perceptor setup
  new('VisionPerceptor', aspectPath+'VisionPerceptor')
  new('GameStatePerceptor', aspectPath+'GameStatePerceptor')

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
  physics.setMass(getSoccerVar('BallMass'))

  geomPath = path+'geometry/'

  geometry = new('kerosin/SphereCollider', geomPath)
  geometry.setRadius(getSoccerVar('BallRadius'))

  new('kerosin/RecorderHandler', geomPath+'recorder')
  new('kerosin/ContactJointHandler', geomPath+'contact')
end

def addField()
  #
  # setup all arena colliders

  #
  # floor and wall collider- infinite planes used as the the hard
  # barrier
  halfLength = getSoccerVar('FieldLength')/2.0
  halfWidth = getSoccerVar('FieldWidth')/2.0
  halfHeight = getSoccerVar('FieldHeight')/2.0
  halfGoalWidth = getSoccerVar('GoalWidth')/2.0
  borderSize = getSoccerVar('BorderSize')/2.0

  addWall('Floor',0.0, 1.0 ,0.0, 0.0) 
  addWall('W1', 1.0,  0.0,  0.0, -halfLength-borderSize)
  addWall('W2',-1.0,  0.0,  0.0, -halfLength-borderSize)  
  addWall('W3', 0.0,  0.0,  1.0, -halfWidth-borderSize)  
  addWall('W4', 0.0,  0.0, -1.0, -halfWidth-borderSize)  

  #
  # mark the soccer field with 4 field flags
  addFlag('Flag1',-halfLength, 0.0, halfWidth)
  addFlag('Flag2', halfLength, 0.0, halfWidth)
  addFlag('Flag3', halfLength, 0.0,-halfWidth)
  addFlag('Flag4',-halfLength, 0.0,-halfWidth)

  #
  # mark the left goal
  addFlag('Goal1l',-halfLength,0.0,-halfGoalWidth)
  addFlag('Goal1r',-halfLength,0.0,+halfGoalWidth)

  #
  # mark the right goal
  addFlag('Goal2l',halfLength,0.0,-halfGoalWidth)
  addFlag('Goal2r',halfLength,0.0,+halfGoalWidth)

  #
  # box collider around the playing field
  fieldBoxPath = $scenePath+'FieldBox/'
  fieldBox = new('kerosin/BoxCollider',fieldBoxPath)
  fieldBox.setBoxLengths(
			 getSoccerVar('FieldLength'),
			 getSoccerVar('FieldHeight'),
			 getSoccerVar('FieldWidth')
			 )
  fieldBox.setPosition(0.0,halfHeight,0.0)
  new('kerosin/RecorderHandler', fieldBoxPath+'recorder')

  #
  # box collider around the goals
  halfGoalDepth  = getSoccerVar('GoalDepth')/2.0
  halfGoalHeight = getSoccerVar('GoalHeight')/2.0

  # left goal
  goalBoxLPath = $scenePath+'GoalBoxL/';
  goalBoxL = new('kerosin/BoxCollider',goalBoxLPath)
  goalBoxL.setBoxLengths(
			 getSoccerVar('GoalDepth'),
			 getSoccerVar('GoalHeight'),
			 getSoccerVar('GoalWidth')
			 )
  goalBoxL.setPosition(
		       -halfLength-halfGoalDepth,
		       halfGoalHeight,
		       0.0
		       )
  new('kerosin/RecorderHandler', goalBoxLPath+'recorder')

  # right goal
  goalBoxRPath = $scenePath+'GoalBoxR/';
  goalBoxR = new('kerosin/BoxCollider',goalBoxRPath)
  goalBoxR.setBoxLengths(
			 getSoccerVar('GoalDepth'),
			 getSoccerVar('GoalHeight'),
			 getSoccerVar('GoalWidth')
			 )
  goalBoxR.setPosition(
		       +halfLength+halfGoalDepth,
		       halfGoalHeight,
		       0.0
		       )
  new('kerosin/RecorderHandler', goalBoxRPath+'recorder')
end

#
# init
#

# the soccer field dimensions in meters
addSoccerVar('FieldLength', 105.0)
addSoccerVar('FieldWidth', 68.0)
addSoccerVar('FieldHeight', 40.0)
addSoccerVar('GoalWidth', 7.32)
addSoccerVar('GoalDepth', 2.0)
addSoccerVar('GoalHeight', 2.44)
addSoccerVar('BorderSize', 4.0)

# agent parameters
addSoccerVar('AgentMass', 75.0)
addSoccerVar('AgentRadius',  0.22)
addSoccerVar('AgentMaxSpeed', 10.0)

# ball parameters
addSoccerVar('BallRadius', 0.111)
addSoccerVar('BallMass',0.4)

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
addBall(-36.0,0.5,-10.0)

#
# register game control aspects
gameControlServer.initControlAspect('GameStateAspect')
gameControlServer.initControlAspect('BallStateAspect')
gameControlServer.initControlAspect('SoccerRuleAspect')

#
# register the SexpMonitorSystem
monitorServer.registerMonitorSystem('SexpMonitor')

#
# queue agents for startup
spadesServer.queueAgents('foo', 2)
spadesServer.queueAgents('bar', 2)
