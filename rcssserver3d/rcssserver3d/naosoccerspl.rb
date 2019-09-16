#
# prepare soccer simulation
#

# setup soccer specific materials
# run "scripts/rcs-materials.rb"
if ($enableInternalMonitor)
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
addSoccerVar('FieldLength', 9.0)
addSoccerVar('FieldWidth', 6.0)
addSoccerVar('FieldHeight', 3.0)
addSoccerVar('GoalWidth', 1.5)
addSoccerVar('GoalDepth', 0.4)
addSoccerVar('GoalHeight', 0.8)
addSoccerVar('PenaltyLength',0.6)
addSoccerVar('PenaltyWidth',2.2)
addSoccerVar('FreeKickDistance', 0.75) # radius of the center circle 
addSoccerVar('FreeKickMoveDist', 1.0)
addSoccerVar('KickRepelDist', 0.5)
addSoccerVar('GoalKickDist', 0.6)
addSoccerVar('BorderSize', 0.0) # prevent complaining about missing variable


# game flow parameters
addSoccerVar('ReadyDuration', 45)
addSoccerVar('SetDuration', 5)
addSoccerVar('PenaltyDuration', 30)
addSoccerVar('SayMsgSize', 512)

# soccer game settings
addSoccerVar('AutomaticKickOff', false)
addSoccerVar('WaitBeforeKickOff', 30.0)
addSoccerVar('CoinTossForKickOff', false)
addSoccerVar('PenaltyShootout', false)
addSoccerVar('StartAnyFieldPosition', false)

addSoccerVar('AutomaticQuit', false)
addSoccerVar('ChangeSidesInSecondHalf', true)

# keepaway settings
addSoccerVar('Keepaway', false)
addSoccerVar('KeepawayCenterX', 0.0)
addSoccerVar('KeepawayCenterY', 0.0)
addSoccerVar('KeepawayLength', 20.0)
addSoccerVar('KeepawayWidth', 20.0)
addSoccerVar('KeepawayLengthReductionRate', 4.0)
addSoccerVar('KeepawayWidthReductionRate', 4.0)

# agent parameters
addSoccerVar('AgentRadius',  0.4)
addSoccerVar('MaxRobotTypeCount', 7)
addSoccerVar('MinRobotTypesCount', 3)
addSoccerVar('MaxSumTwoRobotTypes', 9)

# ball parameters
addSoccerVar('BallRadius', 0.05)
addSoccerVar('BallMass',0.05)

# SPL rule parameters
addSoccerVar('RuleGoalPauseTime', 3.0)
addSoccerVar('RuleKickInPauseTime', 1.0)
addSoccerVar('RuleHalfTime', 10.0 * 60)
addSoccerVar('RuleDropBallTime', 15)   
addSoccerVar('SingleHalfTime', false)
addSoccerVar('UseOffside', false)
addSoccerVar('MaxTouchGroupSize', 2)

# charging foul parameters
addSoccerVar('UseCharging', true)
addSoccerVar('ChargingMinSpeed', 0.4)
addSoccerVar('ChargingMinBallSpeedAngle', 30)
addSoccerVar('ChargingMinDeltaDist', 0.05)
addSoccerVar('ChargingMinDeltaAng', 15)
addSoccerVar('ChargingImmunityTime', 1)
addSoccerVar('ChargingCollisionMinTime', 0.2)
addSoccerVar('ChargingMaxBallRulesDist', 1)
addSoccerVar('ChargingMinCollBallDist', 0.1)
addSoccerVar('ChargingMinCollisionSpeed', 0.2)

# self collision parameters
addSoccerVar('SelfCollisionsTolerance', 0.04)
addSoccerVar('PrintSelfCollisions',  true)
addSoccerVar('FoulOnSelfCollisions', true)
addSoccerVar('SelfCollisionJointFrozenTime', 1.0)
addSoccerVar('SelfCollisionJointThawTime', 2.0)
addSoccerVar('SelfCollisionBeamPenalty', false)
addSoccerVar('SelfCollisionBeamCooldownTime',  10.0)
addSoccerVar('WriteSelfCollisionsToFile', false)
addSoccerVar('SelfCollisionRecordFilename', 'SelfCollisions.txt')

# auto ref parameters  FCP 2010
addSoccerVar('NotStandingMaxTime',30)       
addSoccerVar('GoalieNotStandingMaxTime',60)
addSoccerVar('GroundMaxTime',15)    
addSoccerVar('GoalieGroundMaxTime', 30)     
addSoccerVar('MaxPlayersInsideOwnArea',3)
addSoccerVar('MinOppDistance',0.8)       
addSoccerVar('Min2PlDistance',0.4)       
addSoccerVar('Min3PlDistance',1.0)  

addSoccerVar('FoulHoldTime', 0.5)

# 2014 server changes     
addSoccerVar('ReportScore', true)
addSoccerVar('LabelMessages', true)

# Noise added to requested beam positions
addSoccerVar('BeamNoiseXY',0.05)
addSoccerVar('BeamNoiseAngle',10.0)

# auto ref parameters for testing (not for use in competition...)
#addSoccerVar('NotStandingMaxTime',10)       
#addSoccerVar('GoalieNotStandingMaxTime',30)
#addSoccerVar('GroundMaxTime', 5)    
#addSoccerVar('GoalieGroundMaxTime', 5)     
#addSoccerVar('MaxPlayersInsideOwnArea',2)
#addSoccerVar('MinOppDistance',1.0)       
#addSoccerVar('Min2PlDistance',0.6)       
#addSoccerVar('Min3PlDistance',1.5)

addSoccerVar('MaxNumSafeRepositionAttempts', 100)

# Pass mode parameters
addSoccerVar('PassModeMaxBallSpeed',0.05)
addSoccerVar('PassModeMaxBallDist',0.5)
addSoccerVar('PassModeMinOppBallDist',1.0)
addSoccerVar('PassModeDuration',4.0)
addSoccerVar('PassModeScoreWaitTime',10.0)
addSoccerVar('PassModeRetryWaitTime',3.0)


# recorders
addSoccerVar('BallRecorder',"Ball/geometry/recorder")
addSoccerVar('LeftGoalRecorder',"leftgoal/GoalBox/BoxCollider/recorder")
addSoccerVar('RightGoalRecorder',"rightgoal/GoalBox/BoxCollider/recorder")

# textures
createVariable('Nao', 'UseTexture', 'true')

scene = get($scenePath)
if (scene != nil)
  scene.importScene('rsg/agent/naov4/soccer.rsg')
end

# setup the GameControlServer
gameControlServer = get($serverPath+'gamecontrol')
if (gameControlServer != nil)
  gameControlServer.initControlAspect('SPLState')
  o = get('/sys/server/gamecontrol/SPLState') # hack the name
  o.setName('GameStateAspect')
  gameControlServer.initControlAspect('BallStateAspect')
  gameControlServer.initControlAspect('SPLRule')
  o = get('/sys/server/gamecontrol/SPLRule') # hack the name
  o.setName('SoccerRuleAspect')

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

  monitorServer.registerMonitorItem('SoccerRuleItem')

  monitorServer.registerMonitorItem('MonitorMessages')
end

# install the TrainerCommandParser to parse commands received from a
# monitor client
sparkRegisterMonitorCmdParser 'TrainerCommandParser'

# Load parameters for heterogeneous Nao robots
run "naorobottypes.rb"
