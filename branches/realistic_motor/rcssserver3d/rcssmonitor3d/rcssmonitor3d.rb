#
# rcssmonitor3d.rb
#

sparkSetupRendering()
sparkSetupInput()
sparkSetupTimer()

if ($logPlayerMode == true)
  sparkSetupMonitorLogPlayer()
else
  sparkSetupMonitor()
end

# let spark create a default camera
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

# setup default input bindings
run "scripts/bindings.rb"

# setup default materials common to all simulations
run "scripts/materials.rb"

# source soccersim-monitor.rb to build a soccer monitor
run "soccersim-monitor.rb"

#
# uncomment for logging setup (see spark.rb for reference)
#

sparkResetLogging()
sparkLogErrorToCerr()
# sparkLogAllToFile('./spark.txt')
# sparkLogDebugToCerr()

