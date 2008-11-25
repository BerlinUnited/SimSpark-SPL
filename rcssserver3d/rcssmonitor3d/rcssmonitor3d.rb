#
# monitorspark.rb
#

sparkSetupRendering()
sparkSetupInput()

if ($logPlayerMode == true)
  sparkSetupMonitorLogPlayer()
else
  sparkSetupMonitor()
end

# let spark create a default camera
sparkAddFPSCamera(
 		  $scenePath+'camera', 
 		  x = -5, 
 		  y = -40, 
 		  z = 2,
                  vAngle = 10.0,
                  hAngle = 10.0,
                  maxSpeed = 15.0,
 		  accel = 400.0,
 		  drag = 4,
 		  addCollider = false,
                  colliderRadius = 2.0
 		  )

# setup default input bindings
run "bindings.rb"

# setup default materials common to all simulations
run "scripts/materials.rb"

# source soccersim-monitor.rb to build a soccer monitor
run "soccersim-monitor.rb"

#
# uncomment for logging setup (see spark.rb for reference)
#

sparkResetLogging()
sparkLogAllToCerr()
# sparkLogAllToFile('./spark.txt')
# sparkLogDebugToCerr()

