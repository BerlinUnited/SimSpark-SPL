# GLFrontEnd    -*- mode: ruby; ruby-indent-level: 4 -*-
#
# The front end is responsible for parsing the glext-header files

require 'script/glextension.rb'

module GLExtGen
    class GLFrontEnd
	attr_accessor :files
	
	def initialize
	    @files = []
	end

	# insert a tuple of scan arguments
	def addFile (fileName, extensionExp, functionExp, extensionHash)
	    files.concat([[fileName, Regexp.new(extensionExp), 
				 Regexp.new(functionExp), extensionHash]])
	end

	# scan the files registered
	def scanAll
	    files.each { |item|
		print "Digesting '" + item[0] + "'...\n"
		scan(item[0], item[1], item[2], item[3]) 
	    }
	end

	def scan (fileName, extensionExp, functionExp, extensionHash)
            predefinedExp = Regexp.new("(.*)APIENTRY gl(.*) \\((.*)");
	    currentExtension = nil
	    File.open(fileName).each { |line|
		if line =~ /^(\s*)(\/+)(.*) (gl\w+)/
		    print "removing misleading line ", line
		elsif md = predefinedExp.match(line)
		    print "predefined fct gl", md[2], ":\n"
		elsif md = extensionExp.match(line)
		    extensionName = md[2]
		    currentExtension = extensionHash[extensionName]
		    # if we don't know the extension, we have to add it
		    if currentExtension == nil
			currentExtension = GLExtension.new(extensionName)
			extensionHash[extensionName] = currentExtension
		    end
		    # at this point currentExtension has a valid 
		    # GLExtension object ... always!
		elsif md = functionExp.match(line)
		    # add the function name to the current Extension
		    functionName = md[2]
		    if currentExtension != nil
			currentExtension.addFunction(functionName)
		    end
		end
	    }
	end
	
	def procize (functionName)
	    "PFN"+functionName.upcase+"PROC"
	end
    end
end
