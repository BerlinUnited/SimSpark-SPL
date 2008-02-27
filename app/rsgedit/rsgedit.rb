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

  # setup common materials
  run "scripts/materials.rb"

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



