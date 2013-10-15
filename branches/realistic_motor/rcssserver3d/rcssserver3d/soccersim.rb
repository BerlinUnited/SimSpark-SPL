#
# prepare soccer simulation
#

# setup soccer specific materials
# run "scripts/rcs-materials.rb"
run "scripts/rcs-materials-textures.rb"

importBundle "soccer"

# soccer namespace
$soccerNameSpace = "Soccer"

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

# set a random seed (a seed of 0 means: use a random random seed)
randomServer = get($serverPath+'random')
if (randomServer != nil)
  randomServer.seed(0)
end

# the soccer field dimensions in meters
#addSoccerVar('FieldLength', randomServer.uniformRND(100.0,110.9).floor().to_f())
#addSoccerVar('FieldWidth', randomServer.uniformRND(64.0,75.9).floor().to_f())
# Joschka: settings for Atlanta
#addSoccerVar('FieldLength', 45.0)
#addSoccerVar('FieldWidth', 30.0)

#Jan: alternative settings for Atlanta
addSoccerVar('FieldLength', 50.0)
addSoccerVar('FieldWidth', 32.0)
addSoccerVar('FieldHeight', 40.0)
#addSoccerVar('GoalWidth', 7.32)
addSoccerVar('GoalWidth', 9.0)
addSoccerVar('GoalDepth', 2.0)
#addSoccerVar('GoalHeight', 2.44) # FIFA: 2.44 
addSoccerVar('GoalHeight', 4.0)
addSoccerVar('PenaltyLength',8)
addSoccerVar('PenaltyWidth',10.5)
addSoccerVar('BorderSize', 10.0)
addSoccerVar('FreeKickDistance', 4.5)
addSoccerVar('FreeKickMoveDist', 9.0)
addSoccerVar('AutomaticKickOff', false)
addSoccerVar('WaitBeforeKickOff', 2.0)

# agent parameters
addSoccerVar('AgentMass', 75.0)
addSoccerVar('AgentRadius',  3.5)
addSoccerVar('AgentMaxSpeed', 10.0)

# ball parameters
addSoccerVar('BallRadius', 0.4)
#addSoccerVar('BallDensity',2)
addSoccerVar('BallMass',0.43) # FIFA: 410-450 g

# soccer rule parameters
addSoccerVar('RuleGoalPauseTime',3.0)
addSoccerVar('RuleKickInPauseTime',1.0)
addSoccerVar('RuleHalfTime',8.0 * 60)
addSoccerVar('RuleDropBallTime',30)
addSoccerVar('SingleHalfTime', true)
addSoccerVar('UseOffside',false)

# recorders
addSoccerVar('BallRecorder',"Ball/geometry/recorder")
addSoccerVar('LeftGoalRecorder',"LeftGoalSpace/GoalBoxL/GoalColliderL/recorder")
addSoccerVar('RightGoalRecorder',"RightGoalSpace/GoalBoxR/GoalColliderR/recorder")


scene = get($scenePath)
if (scene != nil)
  scene.importScene('rsg/agent/soccer.rsg')
end

# setup the GameControlServer
gameControlServer = get($serverPath+'gamecontrol')
if (gameControlServer != nil)
  gameControlServer.initControlAspect('GameStateAspect')
  gameControlServer.initControlAspect('BallStateAspect')
  gameControlServer.initControlAspect('SoccerRuleAspect')
end
  
# init monitorItems to transmit game state information
monitorServer = get($serverPath+'monitor')
if (monitorServer != nil)
  monitorServer.registerMonitorItem('GameStateItem')
end

# install the TrainerCommandParser to parse commands received from a
# monitor client
sparkRegisterMonitorCmdParser 'TrainerCommandParser'
