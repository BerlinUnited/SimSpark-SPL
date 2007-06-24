# load a font for the HUD
fontServer = get($serverPath+'font')
fontServer.getFont('VeraMono.ttf')

# register the internal soccer render plugin to draw the game state on the
# screen
sparkRegisterCustomRender 'InternalSoccerRender'

# register the internal soccer input plugin to process soccer specific key
# presses
sparkRegisterCustomInput 'InternalSoccerInput'

# bind keys to soccer commands
run "internalsoccerbindings.rb"
