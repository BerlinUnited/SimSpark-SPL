# GLExtGen:
#
# GLExtGen is a Ruby script which is capable of extracting OpenGL extension
# data from header files and generate files for inclusion in OpenGL projects.
#
# GLExtGen works using a front-end and a back-end. The front-end is responsible
# for analyzing the header files and building an intermediate representation.
# The backend is then responsible for writing out that information in a way that
# the target language can understand it. This design allows different backends
# (say for PASCAL/Delphi) to be generated without having to rewrite lots of
# code. Unfortunately, this separation is not as good as it could be. For example,
# GLBackEnd is really not designed to be derived from. It should be more of an
# interface, than an actual implementation. Nevertheless, it works quite well for
# C++. If you want something else, I suggest just changing the code in GLBackEnd

# perform error checking on config parameters
begin

if $target != "win32" && $target != "x"
	raise "Incorrect target specification"
end

if $path[-1] != '/'[0] && $path[-1] != '\\'[0]
	$path += '/'
	print "WARNING: No trailing slash in path ... adjusting\n"
	print "WARNING: New path is '", $path, "'\n"
end

require 'script/glextension.rb'
require 'script/glfrontend.rb'
require 'script/glmunger.rb'
require 'script/glbackend.rb'

# here we choose the classes we will use
frontEndClass 	= GLExtGen::GLFrontEnd
mungerClass		= GLExtGen::GLMunger
backEndClass 	= GLExtGen::GLBackEnd

# we will use these hash tables to hold string => GLExtension pairs
glExtensionHash = {}
wglExtensionHash = {}
glXExtensionHash = {}

# create our front end
frontEnd = frontEndClass.new

# queue up some jobs to scan
frontEnd.addFile ($path+"glext.h", '(#ifndef) (GL_\w+)', '(.+) (gl\w+)', glExtensionHash)
if $target == "win32"
	frontEnd.addFile ($path+"wglext.h", '(#ifndef) (WGL_\w+)', '(.+) (wgl\w+)', wglExtensionHash)
end
if $target == "x"
	frontEnd.addFile ($path+"glxext.h", '(#ifndef) (GLX_\w+)', '(.+) (glX\w+)', glXExtensionHash)
end

# let's work it
frontEnd.scanAll

# now, for the MS-Windows world, this is fine, but other OpenGL implementations (such as Mesa)
# already define some of the symbols, as their GL/gl.h implements version 1.2 and not 1.1 (as
# the Windows header). So, what we do is, we analyze the regular gl-header file.

	extensionExp = Regexp.new('(#ifndef) (GL_\w+)')
	versionExp = Regexp.new('(#define) (GL_VERSION\w+)')
	glNativeSupportArray = []

	print "\nSearching for native extensions in '"+$path+"gl.h\n"

	File.open($path+"gl.h").each { |line|
		if md = extensionExp.match(line)
			glNativeSupportArray.push(md[2]);
			#print "Found extension: " + md[2] + "\n"
		elsif md = versionExp.match(line)
			glNativeSupportArray.push(md[2]);
			#print "Found version: " + md[2] + "\n"
		end
	}

	# before we pass this data to the backend we get a chance to transform it.
	munger = mungerClass.new(glNativeSupportArray)
	munger.processGL(glExtensionHash)
	munger.processWGL(wglExtensionHash)
	munger.processGLX(glXExtensionHash)

	# at this point the extension hashes contain valid data (let's cross our fingers)

	# now we have done all our processing and sort the extensions
	glExtensionArray	= glExtensionHash.sort
	wglExtensionArray	= wglExtensionHash.sort
	glXExtensionArray	= glXExtensionHash.sort

	# create back end
	backEnd = backEndClass.new

	# now, we just hand our extension arrays over to the backend
	#
	# Note: we are also passing in the frontEnd, because it should contain information
	#		on how to 'procize' a function (generate function pointer typedef given a
	#		function name)
	backEnd.generate (frontEnd, glExtensionArray, wglExtensionArray, glXExtensionArray)
rescue StandardError => bang
	print "ERROR: ", bang, "\n"
end
