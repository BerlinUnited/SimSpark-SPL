#
# rcssserver3d.rb
#

# toggle log file recording
$recordLogfile = false

# set rendering mode
$enableInternalMonitor = false
$enableGuiRendering = false

# toggle the real time mode
$enableRealTimeMode = true

# ----------------------------------------------------

filename = "rcssserver3dgui.rb"
prefix = "("+filename+") "

logNormal(prefix + "Started script" + filename +".\n")

# External rendering mode is activated by a parameter from the gui
if ($gui_enableExternalRendering)
	logNormal(prefix + "Using external (Gui) rendering.\n")
	$enableGuiRendering = true
	$enableInternalMonitor = false
	
	$inputSystem = 'InputSystemQt'	
	$inputSystemBundle = 'inputqt'	
	
	$openGLSystem = 'OpenGLSystemQT'
	$openGLBundle = 'openglsystemqt'
else
	logNormal(prefix + "Using default rendering.\n")
	$enableGuiRendering = false
	$enableInternalMonitor = true
	
	$inputSystem = 'InputSystemSDL'	
	$inputSystemBundle = 'inputsdl'	
	
	$openGLSystem = 'OpenGLSystemSDL'
	$openGLBundle = 'openglsyssdl'	
end

# setup server
logNormal(prefix + "Setting up server.\n")
sparkSetupServer()

# setup rendering
if ($enableInternalMonitor or $enableGuiRendering)
  logNormal(prefix + "Setup rendering.\n")
  importBundle($openGLBundle)
  sparkSetupRendering($openGLSystem, !$enableGuiRendering) # if a gui plugin renders, deactivate the rendering by simspark
end

# setup input
logNormal(prefix + "setup input.\n")
importBundle($inputSystemBundle)
sparkSetupInput($inputSystem)

if ($enableRealTimeMode)
  sparkSetupTimer()
end

# let spark create a default camera
if ($enableInternalMonitor or $enableGuiRendering)
  logNormal(prefix + "Setup camera.\n")
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
end

# setup default input bindings
logNormal(prefix + "Setup bindings.\n")
run "scripts/bindings.rb"

# setup default materials common to all simulations
logNormal(prefix + "Setup materials.\n")
run "scripts/materials.rb"

#
# uncomment for logging setup (see spark.rb for reference)
#

sparkResetLogging()
sparkLogErrorToCerr()
sparkLogNormalToCout(true)
#sparkLogAllToCerr()
#sparkLogAllToFile('spark.txt')
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

logNormal(prefix + "Setup naosoccer simulation.\n")
run "naosoccersimgui.rb"

# setup the integrated agent
#sparkSetupTrain()
#addIntegratedAgent('SoccerbotBehavior',1)

# setup game state info rendering for the internal monitor
# source soccersim.rb to run the soccer simulation
if ($enableInternalMonitor or $enableGuiRendering)
  logNormal(prefix + "Setup internal monitor.\n")
  run "internalsoccermonitor.rb"
end
