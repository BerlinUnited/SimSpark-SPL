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

#
# create materials 
# material setup is not sent via the monitor protocol (yet)
#
material = new('kerosin/MaterialSolid', $serverPath+'material/matYellow');
material.setDiffuse(1.0,1.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matOrange');
material.setDiffuse(1.0,0.3,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matBlue');
material.setDiffuse(0.0,0.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matWhite');
material.setDiffuse(1.0,1.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matRed');
material.setDiffuse(1.0,0.0,0.0,1.0)

material = new('kerosin/Material2DTexture', $serverPath+'material/matRedGlow');
material.setDiffuse(0.8,0.0,0.0,1.0)
material.setEmission(0.1,0.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGreen');
material.setDiffuse(0.1,0.5,0.1,1.0)
material.setAmbient(0.1,0.4,0.1,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGrey');
material.setDiffuse(0.2,0.2,0.2,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matDarkGrey');
material.setDiffuse(0.05,0.05,0.05,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matLightBlue');
material.setDiffuse(0.0,0.75,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matMagenta');
material.setDiffuse(1.0,0.0,0.75,1.0)

#
# uncomment for logging setup (see spark.rb for reference)
#

# sparkLogAllToFile('./spark.txt')
# sparkLogDebugToCerr()

# source soccersim.rb to build a soccer monitor
run "soccersim.rb"



