# GLMunger
#
# The munger does some processing on the extension hashes before they go to the
# backend.
# This fixes:
#	- adds GL_VERSION_1_4 if it's not there
#	- adds GL_VERSION_1_3 if it's not there
#	- adds GL_VERSION_1_2 if it's not there
#	- fix ARB_imaging (move most functions from GL_VERSION_1_2 to ARB_imaging)
#	  See http://www.opengl.org/developers/documentation/OpenGL12.html for more explanation
#	- throws away extensions which are natively supported

require 'script/glextension.rb'

module GLExtGen
	class GLMunger
		attr_accessor :nativeSupport
		def initialize (newNativeSupport)
			@nativeSupport = newNativeSupport
		end

		def processGL (extensionHash)
			# fix GL_VERSION_1_4
			if extensionHash["GL_VERSION_1_4"] == nil
				extensionHash["GL_VERSION_1_4"] = GLExtension.new("GL_VERSION_1_4")
			end

			# fix GL_VERSION_1_3
			if extensionHash["GL_VERSION_1_3"] == nil
				extensionHash["GL_VERSION_1_3"] = GLExtension.new("GL_VERSION_1_3")
			end

			# fix GL_VERSION_1_2
			if extensionHash["GL_VERSION_1_2"] == nil
				extensionHash["GL_VERSION_1_2"] = GLExtension.new("GL_VERSION_1_2")
			end
			
			# fix ARB_imaging
			gl12 = extensionHash["GL_VERSION_1_2"]
			arbImaging = extensionHash["GL_ARB_imaging"]
			if gl12 != nil && arbImaging != nil
				# we move all functions from gl12 to arbImaging
				arbImaging.functionList.concat(gl12.functionList)
				gl12.functionList.clear
				arbImaging.functionList.delete("glDrawRangeElements")
				arbImaging.functionList.delete("glTexImage3D")
				arbImaging.functionList.delete("glTexSubImage3D")
				arbImaging.functionList.delete("glCopyTexSubImage3D")
				gl12.functionList.push("glDrawRangeElements")
				gl12.functionList.push("glTexImage3D")
				gl12.functionList.push("glTexSubImage3D")
				gl12.functionList.push("glCopyTexSubImage3D")
			end
			
			if nativeSupport != nil
				nativeSupport.each { |extensionName|
					print "Processing " + extensionName + " -> "
					if extensionHash[extensionName] != nil
						if extensionName.index("VERSION") != nil
							print "Clearing Functions\n"
							# for a GL_VERSION definition, we just throw away the functions
							extensionHash[extensionName].functionList.clear
						else
							print "Removing\n"
							# throw away the extension, as it is double defined
							extensionHash.delete(extensionName)
						end
					else
						print "No Action\n"
					end
				}
			end
		end

		def processWGL (extensionHash)
		end

		def processGLX (extensionHash)
		end
	end
end