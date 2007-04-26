#
# prepare the soccer monitor
importBundle 'soccermonitor'

# load a font for the HUD
fontServer = get($serverPath+'font')
fontServer.getFont('VeraMono.ttf')

# register the soccer monitor plugin to parse the soccer game
# state
sparkRegisterCustomMonitor 'SoccerMonitor'

# register the soccer render plugin to draw the game state on the
# screen
sparkRegisterCustomRender  'SoccerRender'

if ($logPlayerMode == false)
  # register the soccer input plugin to process soccer specific key
  # presses
  sparkRegisterCustomInput 'SoccerInput'

  # bind keys to soccer commands
  run "soccerbindings.rb"
end
