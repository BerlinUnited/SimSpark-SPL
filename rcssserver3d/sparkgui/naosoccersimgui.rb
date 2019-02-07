#
# internalsoccermonitor.rb
#
nfilename = "internalsoccermonitor.rb"
nprefix = "("+nfilename+") "

logNormal(nprefix + "Setting up Nao soccer simulation.")

# setup soccer specific materials
# run "scripts/rcs-materials.rb"
if ($enableInternalMonitor or $enableGuiRendering)
    run "scripts/rcs-materials-textures.rb"
    
    material = sparkCreate('kerosin/Material2DTexture', $serverPath+'material/matGrass');
    material.setDiffuse(1.0,1.0,1.0,1.0)
    material.setAmbient(0.5,0.5,0.5,1.0)
    material.setDiffuseTexture('textures/rcs-naofield.png')
end

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
addSoccerVar('FieldLength', 21.0)
addSoccerVar('FieldWidth', 14.0)
addSoccerVar('FieldHeight', 40.0)
addSoccerVar('GoalWidth', 2.1)
addSoccerVar('GoalDepth', 0.6)
addSoccerVar('GoalHeight', 0.8)
addSoccerVar('PenaltyLength',1.8)
addSoccerVar('PenaltyWidth',3.9)
addSoccerVar('FreeKickDistance', 1.3)
addSoccerVar('FreeKickMoveDist', 1.5)
addSoccerVar('GoalKickDist', 1.0)
addSoccerVar('AutomaticKickOff', false)
addSoccerVar('WaitBeforeKickOff', 2.0)
addSoccerVar('BorderSize', 0.0) # prevent complaining about missing variable

# agent parameters
addSoccerVar('AgentRadius',  0.4)

# ball parameters
addSoccerVar('BallRadius', 0.042)
addSoccerVar('BallMass',0.026)

# soccer rule parameters
addSoccerVar('RuleGoalPauseTime', 3.0)
addSoccerVar('RuleKickInPauseTime', 1.0)
addSoccerVar('RuleHalfTime', 5.0 * 60)
addSoccerVar('RuleDropBallTime', 15)   
addSoccerVar('SingleHalfTime', false)
addSoccerVar('UseOffside', false)
addSoccerVar('MaxTouchGroupSize', 2)

# auto ref parameters  FCP 2010
addSoccerVar('NotStandingMaxTime',30)       
addSoccerVar('GoalieNotStandingMaxTime',60)
addSoccerVar('GroundMaxTime',15)    
addSoccerVar('GoalieGroundMaxTime', 30)     
addSoccerVar('MaxPlayersInsideOwnArea',3)
addSoccerVar('MinOppDistance',0.8)       
addSoccerVar('Min2PlDistance',0.4)       
addSoccerVar('Min3PlDistance',1.0)      

# auto ref parameters for testing (not for use in competition...)
#addSoccerVar('NotStandingMaxTime',10)       
#addSoccerVar('GoalieNotStandingMaxTime',30)
#addSoccerVar('GroundMaxTime', 5)    
#addSoccerVar('GoalieGroundMaxTime', 5)     
#addSoccerVar('MaxPlayersInsideOwnArea',2)
#addSoccerVar('MinOppDistance',1.0)       
#addSoccerVar('Min2PlDistance',0.6)       
#addSoccerVar('Min3PlDistance',1.5)      

# recorders
addSoccerVar('BallRecorder',"Ball/geometry/recorder")
addSoccerVar('LeftGoalRecorder',"leftgoal/GoalBox/BoxCollider/recorder")
addSoccerVar('RightGoalRecorder',"rightgoal/GoalBox/BoxCollider/recorder")

# textures
createVariable('Nao', 'UseTexture', 'true')

scene = get($scenePath)
if (scene != nil)
  scene.importScene('rsg/agent/nao/soccer.rsg')
end

# setup the GameControlServer
gameControlServer = get($serverPath+'gamecontrol')
if (gameControlServer != nil)
  gameControlServer.initControlAspect('GameStateAspect')
  gameControlServer.initControlAspect('BallStateAspect')
  gameControlServer.initControlAspect('SoccerRuleAspect')
  obj = get('/sys/server/gamecontrol/GameStateAspect')
  if (obj != nil)
	  obj.setTime(0)
	  obj.setScores(0,0)
  end
end

# init monitorItems to transmit game state information
monitorServer = get($serverPath+'monitor')
if (monitorServer != nil)
  monitorServer.registerMonitorItem('GameStateItem')
end

# install the TrainerCommandParser to parse commands received from a
# monitor client
sparkRegisterMonitorCmdParser 'TrainerCommandParser'
