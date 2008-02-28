#
# this is the init script of the Kerosin library
#

# This script creates a BUNCH of variables. It basically sets up the
# default settings for all engine namespaces/variables. It also serves
# as the documentation for all the settings.

####
# Application
#
# This section contains information about the application being executed.
#

createVariable('Application.Title', 'SimSpark');


####
# System
#
# This section contains information about the environment the engine is
# executed in, such as font paths.
#

createVariable('System.FontPath', 'fonts/');


####
# Viewport
#
# The Viewport namespace is used to describe all the variables related
# to the application window.
#

createVariable('Viewport.XRes', 800);           # width
createVariable('Viewport.YRes', 600);	        # height
createVariable('Viewport.FullScreen',   false);	# run in fullscreen?
createVariable('Viewport.DoubleBuffer', true);  # use double buffering?

# Set up bits per pixel
createVariable('Viewport.RedBits',   5); # red bits
createVariable('Viewport.GreenBits', 6); # green bits
createVariable('Viewport.BlueBits',  5); # blue bits
createVariable('Viewport.AlphaBits', 8); # alpha bits

# stencil and depth buffers
createVariable('Viewport.DepthBits', 16);  # depth bits
createVariable('Viewport.StencilBits', 0); # stencil bits

# dpi for font rendering
createVariable('Viewport.DpiHRes', 120); # horizontal resolution (72, 96, 120, 300)
createVariable('Viewport.DpiVRes', 120); # vertical resolution (72, 96, 120, 300)
