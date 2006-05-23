
$useCars = 'no'
#
# import needed bundles
#

#
# generic plugins
importBundle "filesystemstd"
importBundle "sexpparser"
if ($useCars == 'yes')
  importBundle "rubysceneimporter"
end

#
# bundle of soccer plugins
importBundle "soccer"
if ($useCars == 'yes')
  importBundle "sparkagent"
end

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
$agentY = 10.0
$agentZ = 2.5

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

def addAgent(path)
    # move different agents away from each other
    aspect = get(path)
    aspect.setLocalPos($agentX,$agentY,$agentZ)
    $agentX -= 4.0
    $agentY += 1.0
    $agentZ += 1.1

  if ($useCars == 'yes')
    scene = get(path)
    scene.importScene('rsg/agent/soccerplayer.rsg')
  else
    # physics setup
    physics = new('oxygen/Body', path+'physics')
    physics.setSphereTotal(getSoccerVar('AgentMass'),
	  		 getSoccerVar('AgentRadius'))

    maxVel = new('oxygen/VelocityController', path+'physics/maxVelCtr')
    maxVel.setMaxVelocity(getSoccerVar('AgentMaxSpeed'))

    drag = new('oxygen/DragController',path+'physics/drag')
    drag.setAngularDrag(12.0)
    drag.setLinearDrag(12.0)

    # geometry setup

    geomPath = path+'geometry/'
    geometry = new('oxygen/SphereCollider', geomPath)
    geometry.setRadius(getSoccerVar('AgentRadius'))

    contact = new('oxygen/ContactJointHandler', geomPath+'contact')

    # agent state (needs to be set up before perceptors)
    new('AgentState', path+'AgentState')

    # effector setup
    new('InitEffector', path+'InitEffector')

    # driveeffector setup
    driveEffector = new('DriveEffector', path+'DriveEffector')
    # this sets the acceleration of the robot.
    # (values are still experimental :)
    driveEffector.setForceFactor(35.0 * 12.0 / 100.0);
    driveEffector.setSigma(0.005);
    # drive consumption. (higher value means lower consumption) 
    # untested if this is enough or too much 
    driveEffector.setConsumption(120 * 75.0 * 12.0 * 4);

    kickEffector = new('KickEffector', path+'KickEffector')
    kickEffector.setKickMargin(0.07)
    kickEffector.setForceFactor(0.7)
    kickEffector.setTorqueFactor(0.1)
    kickEffector.setNoiseParams(0.4,0.02,0.9,4.5)
    kickEffector.setSteps(10)
    kickEffector.setMaxPower(100.0)
    kickEffector.setAngleRange(0.0,50.0)

    new('BeamEffector', path+'BeamEffector')
    new('SayEffector', path+'SayEffector')

    # setup catch effector
    catchEffector = new('CatchEffector', path+'CatchEffector')
    catchEffector.setCatchMargin(0.8);


    # setup pan/tilt effector
    pantiltEffector = new('PanTiltEffector', path+'PanTiltEffector')
    pantiltEffector.setMaxPanAngleDelta(90);
    pantiltEffector.setMaxTiltAngleDelta(10);

    # perceptor setup
#    visionPerceptor = new('VisionPerceptor', path+'VisionPerceptor')
    # set to true for debugging. will be set to false during competition
#    visionPerceptor.setSenseMyPos(false);
#    visionPerceptor.addNoise(true);
#    visionPerceptor.useRandomNoise(true);

#     pvisionPerceptor = new('VisionPerceptor', path+'PerfectVisionPerceptor')
#     # set to true for debugging. will be set to false during competition
#     pvisionPerceptor.setSenseMyPos(false);
#     pvisionPerceptor.addNoise(false);
#     pvisionPerceptor.setPredicateName('PerfectVision')


    # perceptor setup: restricted vision perceptor
    visionPerceptor = new('RestrictedVisionPerceptor', path+'VisionPerceptor')
    visionPerceptor.setSenseMyPos(false);
    visionPerceptor.addNoise(true);
    visionPerceptor.setStaticSenseAxis(true);
    visionPerceptor.setViewCones(90,90);
    visionPerceptor.setPanRange(-180,180);
    visionPerceptor.setTiltRange(-10,10);


    new('GameStatePerceptor', path+'GameStatePerceptor')
    new('AgentStatePerceptor', path+'AgentStatePerceptor')
    new('HearPerceptor', path+'HearPerceptor')

  end 
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

  contact = new('oxygen/ContactJointHandler', $scenePath+name+'/contact')
end

# construct a ball at (x,y,z)
def addBall(x,y,z)
  path = $scenePath+'Ball/'

  ball = new('Ball',path)
  ball.setLocalPos(x,y,z)

  physics = new('oxygen/Body', path+'physics')
  physics.setSphereTotal(getSoccerVar('BallMass'),
			 getSoccerVar('BallRadius'))

  drag = new('oxygen/DragController',path+'physics/drag')
  drag.setAngularDrag(0.0)
  drag.setLinearDrag(0.3)

  geomPath = path+'geometry/'

  geometry = new('oxygen/SphereCollider', geomPath)
  geometry.setRadius(getSoccerVar('BallRadius'))

  new('oxygen/RecorderHandler', geomPath+'recorder')

  contact = new('oxygen/ContactJointHandler', geomPath+'contact')
  
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
  halfGoalWidth  = getSoccerVar('GoalWidth')/2.0
  halfGoalHeight = getSoccerVar('GoalHeight')/2.0
  barDiameter = getSoccerVar('LineWidth')
  barRadius = barDiameter/2.0

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

  # create the goal bar on top of the goal (left goal)
  barL = new('oxygen/Transform', goalBoxLPath+'bar')
  barL.setLocalPos(-halfLength-halfGoalDepth+getSoccerVar('LineWidth'), 
		   0.0,
		   getSoccerVar('GoalHeight') + barRadius)
  boxBarL = new('oxygen/BoxCollider', goalBoxLPath+'bar/geometry')
  # create small space between bar and posts to avoid creating contact joints
  boxBarL.setBoxLengths(getSoccerVar('GoalDepth'),
			getSoccerVar('GoalWidth') - 0.1,
			barDiameter)

  # create the left goal post (left goal)
  leftPostL = new('oxygen/Transform', goalBoxLPath+'leftPost')
  leftPostL.setLocalPos(-halfLength-halfGoalDepth+getSoccerVar('LineWidth'), 
			-halfGoalWidth-barRadius,
			halfGoalHeight+barRadius )
  boxLeftPostL = new('oxygen/BoxCollider', goalBoxLPath+'leftPost/geometry')
  boxLeftPostL.setBoxLengths(getSoccerVar('GoalDepth'),
			     barDiameter,
			     getSoccerVar('GoalHeight'))

  # create the right goal post (left goal)
  rightPostL = new('oxygen/Transform', goalBoxLPath+'rightPost')
  rightPostL.setLocalPos(-halfLength-halfGoalDepth+getSoccerVar('LineWidth'), 
			halfGoalWidth+barRadius,
			halfGoalHeight+barRadius)
  boxRightPostL = new('oxygen/BoxCollider', goalBoxLPath+'rightPost/geometry')
  boxRightPostL.setBoxLengths(getSoccerVar('GoalDepth'),
			      barDiameter,
			      getSoccerVar('GoalHeight'))

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

  # create the goal bar on top of the goal (right goal)
  barR = new('oxygen/Transform', goalBoxRPath+'bar')
  barR.setLocalPos(halfLength+halfGoalDepth-getSoccerVar('LineWidth'), 
		   0.0,
		   getSoccerVar('GoalHeight') + barRadius)
  boxBarR = new('oxygen/BoxCollider', goalBoxRPath+'bar/geometry')
  # create small space between bar and posts to avoid creating contact joints
  boxBarR.setBoxLengths(getSoccerVar('GoalDepth'),
			getSoccerVar('GoalWidth') - 0.1,
			barDiameter)

  # create the left goal post (right goal)
  leftPostR = new('oxygen/Transform', goalBoxRPath+'leftPost')
  leftPostR.setLocalPos(halfLength+halfGoalDepth-getSoccerVar('LineWidth'), 
			-halfGoalWidth-barRadius,
			halfGoalHeight+barRadius)
  boxLeftPostR = new('oxygen/BoxCollider', goalBoxRPath+'leftPost/geometry')
  boxLeftPostR.setBoxLengths(getSoccerVar('GoalDepth'),
			     barDiameter,
			     getSoccerVar('GoalHeight'))

  # create the right goal post (right goal)
  rightPostR = new('oxygen/Transform', goalBoxRPath+'rightPost')
  rightPostR.setLocalPos(halfLength+halfGoalDepth-getSoccerVar('LineWidth'), 
			 halfGoalWidth+barRadius,
			 halfGoalHeight+barRadius)
  boxRightPostR = new('oxygen/BoxCollider', goalBoxRPath+'rightPost/geometry')
  boxRightPostR.setBoxLengths(getSoccerVar('GoalDepth'),
			      barDiameter,
			      getSoccerVar('GoalHeight'))
end

#
# init
#

# set up logging
logServer = get($serverPath+'log')
logServer.addStream(':cerr', 'eError')
#logServer.addStream(':cout', 'eDebug')


# set a random seed (a seed of 0 means: use a random random seed)
randomServer = get($serverPath+'random')
randomServer.seed(0)

# the soccer field dimensions in meters
addSoccerVar('FieldLength', randomServer.uniformRND(100.0,110.9).floor().to_f())
addSoccerVar('FieldWidth', randomServer.uniformRND(64.0,75.9).floor().to_f())
addSoccerVar('FieldHeight', 40.0)
addSoccerVar('GoalWidth', 7.32)
addSoccerVar('GoalDepth', 2.0)
addSoccerVar('GoalHeight', 1.6) # FIFA: 2.44 
addSoccerVar('BorderSize', 10.0)
addSoccerVar('LineWidth', 0.08) # this isn't sent to the monitor yet
addSoccerVar('FreeKickDistance', 9.15)
addSoccerVar('AutomaticKickOff', false)
addSoccerVar('WaitBeforeKickOff', 2.0)

# agent parameters
addSoccerVar('AgentMass', 75.0)
addSoccerVar('AgentRadius',  0.22)
addSoccerVar('AgentMaxSpeed', 10.0)

# ball parameters
addSoccerVar('BallRadius', 0.111)
addSoccerVar('BallMass',randomServer.uniformRND(0.41,0.45))

# soccer rule parameters
addSoccerVar('RuleGoalPauseTime',3.0)
addSoccerVar('RuleKickInPauseTime',1.0)
addSoccerVar('RuleHalfTime',5.0 * 60)
addSoccerVar('RuleDropBallTime',30)
addSoccerVar('SingleHalfTime',false)
addSoccerVar('UseOffside', true)

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

if ($useCars == 'yes')
  sceneServer.initSceneImporter("RubySceneImporter")
end

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
Spades.MonitorInterval = 15

# start  the integrated comm server
Spades.RunIntegratedCommserver = true

# the number of commServers to wait for before the simulation is
# initially unpaused
Spades.CommServersWanted = 1

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

# init monitorItems to transmit game state information
monitorServer.registerMonitorItem('GameStateItem')

#
# queue agents for startup
spadesServer.queueAgents('foo', 11)
spadesServer.queueAgents('bar', 11)

