#
# simspark.rb
#

sparkSetupRendering()
sparkSetupInput()
sparkSetupMonitor()

# let spark create a default camera
sparkAddFPSCamera(
		  $scenePath+'camera', 
		  x = -5, 
		  y = -40, 
		  z = 2,
		  maxSpeed = 15.0,  
		  accel = 400.0,
		  drag = 4,
		  addCollider = true
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
material.setDiffuse(1.0,0.0,0.0,1.0)
material.setEmission(0.5,0.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGreen');
material.setDiffuse(0.1,0.5,0.1,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGrey');
material.setDiffuse(0.1,0.1,0.1,1.0)

# source soccersim.rb to build a soccer monitor
run "soccersim.rb"



