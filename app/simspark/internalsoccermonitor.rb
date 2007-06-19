# load a font for the HUD
fontServer = get($serverPath+'font')
fontServer.getFont('VeraMono.ttf')

# register the soccer render plugin to draw the game state on the
# screen
sparkRegisterCustomRender 'InternalSoccerRender'
