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
material.setColor(0.8,0.8,0.2)

material = new('kerosin/MaterialSolid', $serverPath+'material/matBlue');
material.setColor(0.2,0.2,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matWhite');
material.setColor(1.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matRed');
material.setColor(1.0,0.0,0.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGreen');
material.setColor(0.1,0.5,0.1)







