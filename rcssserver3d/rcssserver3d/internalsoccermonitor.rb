# load a font for the HUD
fontServer = sparkGetFontServer()

if (fontServer != nil)
  fontServer.getFont('fonts/VeraMono.ttf')
end

# register the internal soccer render plugin to draw the game state on the
# screen
sparkRegisterCustomRender 'InternalSoccerRender'

# register the internal soccer input plugin to process soccer specific key
# presses
sparkRegisterCustomInput 'InternalSoccerInput'

# bind keys to soccer commands
run "internalsoccerbindings.rb"
