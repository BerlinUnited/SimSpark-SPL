# Default Settings
#
# This script creates a BUNCH of variables. It basically sets up the
# default settings for all engine namespaces/variables. It also serves
# as the documentation for all the settings.

####
# Application
#
# This section contains information about the application being executed.
#

createVariable ('Application.title', 'Kerosin');


####
# System
#
# This section contains information about the environment the engine is
# executed in, such as font paths.
#

createVariable ('System.fontPath', 'C:/Windows/Fonts/');


####
# Viewport
#
# The Viewport namespace is used to describe all the variables related
# to the application window.
#

createVariable ('Viewport.xRes', 640);	# width
createVariable ('Viewport.yRes', 480);	# height
createVariable ('Viewport.fullScreen', false);	# run in fullscreen?
createVariable ('Viewport.doubleBuffer', true);# use double buffering?
# Set up bits per pixel
createVariable ('Viewport.redBits', 8);	# red bits
createVariable ('Viewport.greenBits', 8);	# green bits
createVariable ('Viewport.blueBits', 8);	# blue bits
createVariable ('Viewport.alphaBits', 8);	# alpha bits
# stencil and depth buffers
createVariable ('Viewport.depthBits', 24);	# depth bits
createVariable ('Viewport.stencilBits', 8);# stencil bits
# dpi for font rendering
createVariable ('Viewport.dpiHRes', 120);	# horizontal resolution (72, 96, 120, 300)
createVariable ('Viewport.dpiVRes', 120);	# vertical resolution (72, 96, 120, 300)
