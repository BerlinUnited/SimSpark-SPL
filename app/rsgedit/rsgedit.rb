#
# rsgedit.rb
#

def rsgeditResetScene
  sparkSetupServer()
  sparkSetupInput('InputSystemWX')

  # setup rendering using no OpenGLSystem- the wxWidgets GL canvas takes
  # care of OpenGL setup
  sparkSetupRendering('')

  sparkResetScene()

  # log from which rsg file nodes were created
  rubySceneImporter = get($serverPath+'scene/RubySceneImporter')
  rubySceneImporter.enableSceneDictionary(true);

  # let spark create a default camera
  sparkAddFPSCamera(
		    $scenePath+'camera', 
		    x = -5, 
		    y = -40, 
		    z = 15,
		    maxSpeed = 15.0,
		    accel = 400.0,
		    drag = 4,
		    addCollider = false
		    )
end

# import required bundles
importBundle('inputwx')

# reset any all logging (output via gui)
sparkResetLogging()

# prepare a default scene
rsgeditResetScene()

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

material = new('kerosin/MaterialSolid', $serverPath+'material/matSelected');
material.setDiffuse(0.9,0.1,0.9,0.9)
