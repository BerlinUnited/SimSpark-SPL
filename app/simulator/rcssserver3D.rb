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
  aspect.setLocalPos($agentX,$agentY,$agentZ)
  $agentX += 5.0
  $agentY += 0.0
  $agentZ += 0.1

  # geometry and physics setup
  physics = new('oxygen/Body', aspectPath+'physics')
  physics.setMass(getSoccerVar('AgentMass'))
  physics.setMaxSpeed(getSoccerVar('AgentMaxSpeed'))

  geometry = new('oxygen/SphereCollider', aspectPath+'geometry')
  geometry.setRadius(getSoccerVar('AgentRadius'))

  # agent state (needs to be set up before perceptors)
  new('AgentState', aspectPath+'AgentState')

  # effector setup
  new('InitEffector', aspectPath+'InitEffector')

  # driveeffector setup
  driveEffector = new('DriveEffector', aspectPath+'DriveEffector')
  # the driveeffector is good for acceleration up to 12m/s² of a 75kg robot 
  # (if there was no friction. but there is. :)
  driveEffector.setForceFactor(75.0 * 12.0 / 100.0);
  driveEffector.setSigma(0.005);
  # drive consumption. (higher value means lower consumption) 
  # untested if this is enough or too much 
  driveEffector.setConsumption(120 * 75.0 * 12.0 * 4);

  kickEffector = new('KickEffector', aspectPath+'KickEffector')
  kickEffector.setForceFactor(22.0)
  kickEffector.setNoiseParams(0.4,0.02,0.9,4.5)
  kickEffector.setSteps(10)
  kickEffector.setMaxPower(100.0)
  kickEffector.setAngleRange(0.0,50.0)

  new('BeamEffector', aspectPath+'BeamEffector')

  # perceptor setup
  visionPerceptor = new('VisionPerceptor', aspectPath+'VisionPerceptor')
  visionPerceptor.setSenseMyPos(true);

  new('GameStatePerceptor', aspectPath+'GameStatePerceptor')
  new('AgentStatePerceptor', aspectPath+'AgentStatePerceptor')

end

# add a field flag to (x,y,z)
def addFlag(name,id,x,y,z)
  fullname = name+'_'+id
  flag = new('FieldFlag',$scenePath+fullname)
  flag.setLocalPos(x,y,z)
  # object state
  state = new('ObjectState', $scenePath+fullname+'/ObjectState')
  state.setPerceptName(name)
  state.setID(id)
end

# add a plane collider with ax+by+cz=d
def addWall(name,a,b,c,d)
  wall = new('oxygen/PlaneCollider', $scenePath+name)
  wall.setParams(a,b,c,d)
end

# construct a ball at (x,y,z)
def addBall(x,y,z)
  path = $scenePath+'Ball/'

  ball = new('Ball',path)
  ball.setLocalPos(x,y,z)

  physics = new('oxygen/Body', path+'physics')
  physics.setMass(getSoccerVar('BallMass'))

  geomPath = path+'geometry/'

  geometry = new('oxygen/SphereCollider', geomPath)
  geometry.setRadius(getSoccerVar('BallRadius'))

  new('oxygen/RecorderHandler', geomPath+'recorder')
  new('oxygen/ContactJointHandler', geomPath+'contact')

  # object state
  state = new('ObjectState', path+'ObjectState')
  state.setPerceptName('Ball')
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

  addWall('Floor',0.0, 0.0, 1.0, 0.0) 
  addWall('W1', 1.0,  0.0,  0.0, -halfLength-borderSize)
  addWall('W2',-1.0,  0.0,  0.0, -halfLength-borderSize)  
  addWall('W3', 0.0,  1.0,  0.0, -halfWidth-borderSize)  
  addWall('W4', 0.0, -1.0,  0.0, -halfWidth-borderSize)  

  #
  # mark the soccer field with 4 field flags
  addFlag('Flag', '1_l',-halfLength,-halfWidth, 0.0)
  addFlag('Flag', '2_l',-halfLength, halfWidth, 0.0)
  addFlag('Flag', '1_r', halfLength,-halfWidth, 0.0)
  addFlag('Flag', '2_r', halfLength, halfWidth, 0.0)

  #
  # mark the left goal
  addFlag('Goal', '1_l',-halfLength,-halfGoalWidth, 0.0)
  addFlag('Goal', '2_l',-halfLength, halfGoalWidth, 0.0)

  #
  # mark the right goal
  addFlag('Goal', '1_r', halfLength,-halfGoalWidth, 0.0)
  addFlag('Goal', '2_r', halfLength, halfGoalWidth, 0.0)

  #
  # box collider around the playing field
  fieldBoxPath = $scenePath+'FieldBox/'
  fieldBox = new('oxygen/BoxCollider',fieldBoxPath)
  fieldBox.setBoxLengths(
			 getSoccerVar('FieldLength'),
			 getSoccerVar('FieldWidth'),
			 getSoccerVar('FieldHeight')
			 )
  fieldBox.setPosition(0.0,0.0,halfHeight)
  new('oxygen/RecorderHandler', fieldBoxPath+'recorder')

  #
  # box collider around the goals
  halfGoalDepth  = getSoccerVar('GoalDepth')/2.0
  halfGoalHeight = getSoccerVar('GoalHeight')/2.0

  # left goal
  goalBoxLPath = $scenePath+'GoalBoxL/';
  goalBoxL = new('oxygen/BoxCollider',goalBoxLPath)
  goalBoxL.setBoxLengths(
			 getSoccerVar('GoalDepth'),
			 getSoccerVar('GoalWidth'),
			 getSoccerVar('GoalHeight')
			 )
  goalBoxL.setPosition(
		       -halfLength-halfGoalDepth,
		       0.0,
		       halfGoalHeight
		       )
  new('oxygen/RecorderHandler', goalBoxLPath+'recorder')

  # right goal
  goalBoxRPath = $scenePath+'GoalBoxR/';
  goalBoxR = new('oxygen/BoxCollider',goalBoxRPath)
  goalBoxR.setBoxLengths(
			 getSoccerVar('GoalDepth'),
			 getSoccerVar('GoalWidth'),
			 getSoccerVar('GoalHeight')
			 )
  goalBoxR.setPosition(
		       +halfLength+halfGoalDepth,
		       0.0,
		       halfGoalHeight
		       )
  new('oxygen/RecorderHandler', goalBoxRPath+'recorder')
end

#
# init
#

# set a random seed (a seed of 0 means: use a random random seed)
randomServer = get($serverPath+'random')
randomServer.seed(0)

# the soccer field dimensions in meters
addSoccerVar('FieldLength', randomServer.uniformRND(100.0,110.9).floor().to_f())
addSoccerVar('FieldWidth', randomServer.uniformRND(64.0,75.9).floor().to_f())
addSoccerVar('FieldHeight', 40.0)
addSoccerVar('GoalWidth', 7.32)
addSoccerVar('GoalDepth', 2.0)
addSoccerVar('GoalHeight', 2.44)
addSoccerVar('BorderSize', 10.0)

# agent parameters
addSoccerVar('AgentMass', 75.0)
addSoccerVar('AgentRadius',  0.22)
addSoccerVar('AgentMaxSpeed', 10.0)

# ball parameters
addSoccerVar('BallRadius', 0.111)
addSoccerVar('BallMass',randomServer.uniformRND(0.41,0.45))

# soccer rule parameters
addSoccerVar('RuleGoalPauseTime',3.0)
addSoccerVar('RuleKickingPauseTime',1.0)
addSoccerVar('RuleHalfTime',2.25 * 60)

#
# mount a standard file system
fileServer = get($serverPath+'file')
fileServer.mount('FileSystemSTD', 'data/')

#
# setup the PhysicsServer
new('oxygen/PhysicsServer', $serverPath+'physics')

#
# setup the SceneServer
sceneServer = new('oxygen/SceneServer', $serverPath+'scene')
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
world = new('oxygen/World', $scenePath+'world')
world.setGravity(0.0, 0.0, -9.81)
new('oxygen/Space', $scenePath+'space')

# construct the playing field
addField()

#
# game Setup
#

# set the time of a single simulation step
Spades.TimePerStep = 0.01
# set up the number of simsteps per monitor message
Spades.MonitorInterval = 4
# set up the integrated comm server
Spades.RunIntegratedCommserver = true
# flag if we want to send agent think times
Spades.SendAgentThinkTimes = false

#
# put a ball on the soccer field
addBall(0.0,0.0,10.0)

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
spadesServer.queueAgents('foo', 11)
spadesServer.queueAgents('bar', 11)


