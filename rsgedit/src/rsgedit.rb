#
# rsgedit.rb
#

# toggle the real time mode
$enableRealTimeMode = false

def rsgeditResetScene
  sparkSetupServer()
  sparkSetupInput('InputSystemWX')
  sparkSetupRendering('OpenGLSystemWX')
  sparkResetScene()

  if ($enableRealTimeMode)
    sparkSetupTimer()
  end

  # setup common materials
  run "scripts/materials.rb"

  # setup rsgedit specific materials
  run "scripts/rsgedit-materials.rb"

  # log from which rsg file nodes were created
  rubySceneImporter = get($serverPath+'scene/RubySceneImporter')
  rubySceneImporter.enableSceneDictionary(true);

  # let spark create a default camera
  sparkAddFPSCamera(
		    $scenePath+'camera', 
		    x = 0, 
		    y = -4, 
		    z = 0.6,
		    maxSpeed = 15.0,
		    accel = 40.0,
		    drag = 4,
		    addCollider = false
		    )
end

# import required bundles
importBundle('inputwx')
importBundle('openglsyswx')

# reset any all logging (output via gui)
sparkResetLogging()

# prepare a default scene
rsgeditResetScene()

# setup default input bindings
run "bindings.rb"




