#
# simspark.rb
#

# import required plugins
importBundle 'sexpparser'

# let spark create a default camera
sparkAddFPSCamera(
		  $scenePath+'camera', 
		  x = 0, 
		  y = 5, 
		  z = 40,
		  maxSpeed = 15.0,
		  accel = 40.0,
		  drag = 4
		  )

#
# collection of callbacks and helper functions
#

# create custom materials
material = new('kerosin/MaterialSolid', $serverPath+'material/matYellow');
material.setDiffuse(1.0,1.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matBlue');
material.setDiffuse(0.0,0.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matWhite');
material.setDiffuse(1.0,1.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matRed');
material.setDiffuse(1.0,0.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matRedGlow');
material.setDiffuse(1.0,0.0,0.0,1.0)
marerial.setEmission(0.5,0.0,0.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGreen');
material.setDiffuse(0.1,0.5,0.1,1.0)







