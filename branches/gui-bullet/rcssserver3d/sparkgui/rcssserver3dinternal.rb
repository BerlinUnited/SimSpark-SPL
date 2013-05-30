#
# rcssserver3d.rb
#

# toggle log file recording
$recordLogfile = false

# toggle the internal monitor
$enableInternalMonitor = true

# toggle the real time mode
$enableRealTimeMode = true

sparkSetupServer()
if ($enableInternalMonitor)
  sparkSetupRendering()
end
sparkSetupInput()

if ($enableRealTimeMode)
  sparkSetupTimer()
end

# let spark create a default camera
if ($enableInternalMonitor)
  sparkAddFPSCamera(
                    $scenePath+'camera',
                    x = 0,
                    y = -4,
                    z = 0.6,
                    vAngle = 10.0,
                    hAngle = 10.0,
                    maxSpeed = 1.5,
                    accel = 40.0,
                    drag = 4,
                    addCollider = false,
                    colliderRadius = 2.0
                    )
end

# setup default input bindings
run "scripts/bindings.rb"

# setup default materials common to all simulations
run "scripts/materials.rb"

#
# uncomment for logging setup (see spark.rb for reference)
#

sparkResetLogging()
sparkLogErrorToCerr()
#sparkLogAllToCerr()
#sparkLogAllToFile('spark.txt')
#sparkLogDebugToCerr()

#
# uncomment any of the following to run a simulation
#

# create an arena with a bunch of boxes and spheres
# scene = get($scenePath)
# scene.importScene('rsg/boxspheres/simspark.rsg')

# create an arena to test various joint bodies
# scene = get($scenePath)
# scene.importScene('rsg/jointtest/simspark.rsg')

run "naosoccersim.rb"

# setup the integrated agent
#sparkSetupTrain()
#addIntegratedAgent('SoccerbotBehavior',1)

# setup game state info rendering for the internal monitor
# source soccersim.rb to run the soccer simulation
if ($enableInternalMonitor)
  run "internalsoccermonitor.rb"
end
