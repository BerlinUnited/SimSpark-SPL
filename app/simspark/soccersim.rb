#
# prepare soccer simulation
#

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
randomServer.seed(0)

# the soccer field dimensions in meters
addSoccerVar('FieldLength', randomServer.uniformRND(100.0,110.9).floor().to_f())
addSoccerVar('FieldWidth', randomServer.uniformRND(64.0,75.9).floor().to_f())
addSoccerVar('FieldHeight', 40.0)
addSoccerVar('GoalWidth', 7.32)
addSoccerVar('GoalDepth', 2.0)
addSoccerVar('GoalHeight', 0.75) # FIFA: 2.44 
addSoccerVar('BorderSize', 10.0)
addSoccerVar('FreeKickDistance', 9.15)
addSoccerVar('AutomaticKickOff', false)
addSoccerVar('WaitBeforeKickOff', 2.0)

# agent parameters
addSoccerVar('AgentMass', 75.0)
addSoccerVar('AgentRadius',  0.22)
addSoccerVar('AgentMaxSpeed', 10.0)

# ball parameters
addSoccerVar('BallRadius', 1.0)
addSoccerVar('BallDensity',2)

# soccer rule parameters
addSoccerVar('RuleGoalPauseTime',3.0)
addSoccerVar('RuleKickInPauseTime',1.0)
addSoccerVar('RuleHalfTime',5.0 * 60)
addSoccerVar('RuleDropBallTime',30)

scene = get($scenePath)
scene.importScene('rsg/agent/soccer.rsg')

# setup the GameControlServer
gameControlServer = get($serverPath+'gamecontrol')
gameControlServer.initControlAspect('GameStateAspect')
gameControlServer.initControlAspect('BallStateAspect')
# gameControlServer.initControlAspect('SoccerRuleAspect')

# init monitorItems to transmit game state information
monitorServer = get($serverPath+'monitor')
monitorServer.registerMonitorItem('GameStateItem')

# install the TrainerCommandParser to parse commands received from a
# monitor client
sparkRegisterMonitorCmdParser 'TrainerCommandParser'
