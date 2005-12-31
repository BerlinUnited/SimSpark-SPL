#
# simspark.rb
#

sparkSetupServer()
#sparkSetupRendering()
#sparkSetupInput()


# let spark create a default camera
sparkAddFPSCamera(
		  $scenePath+'camera', 
		  x = -5, 
		  y = -40, 
		  z = 2,
		  maxSpeed = 15.0,
		  accel = 400.0,
		  drag = 4,
		  addCollider = false
		  )

# setup default input bindings
run "bindings.rb"

# create custom materials
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

material = new('kerosin/MaterialSolid', $serverPath+'material/matRedGlow');
material.setDiffuse(1.0,0.0,0.0,1.0)
material.setEmission(0.5,0.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGreen');
material.setDiffuse(0.1,0.6,0.1,1.0)
material.setAmbient(0.1,0.3,0.1,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGrey');
material.setDiffuse(0.1,0.1,0.1,1.0)

#
# uncomment for logging setup (see spark.rb for reference)
#

#sparkLogAllToFile('./spark.txt')
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

# source soccersim.rb to run the soccer simulation
run "soccersim.rb"




