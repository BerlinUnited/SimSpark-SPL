# the GLExtension class is used just for convenience. It manages a list of
# functions which are associated with a particular extension.
# Note: Not all extension have functions!!

module GLExtGen
	class GLExtension
		attr_accessor :functionList, :name
		def initialize (newName)
			@functionList = []
			@name = newName
		end
	
		def addFunction (function)
			@functionList.push(function)
			@functionList.uniq
		end

		def removePrefix
			i = @name.index('_')
			if i == 2
				# we have a GL_ extension, watch out for the pesky 3DFX extension
				if @name[3] != "3"[0]
					@name[3..-1]
				else
					@name[2..-1]
				end		
			else
				# we have a WGL_ or GLX_ extension
				@name[0..(i-1)].downcase + @name[i..-1]
			end
		end
	end
end
