# GLBackEnd
#
# This backend generates CPP code

require 'script/glextension.rb'
require 'script/glfrontend.rb'

module GLExtGen
	class GLBackEnd
		def initialize
		end

		# generate output
		def generate (frontEnd, glArray, wglArray, glXArray)
			print "\n"
			generateHFile (frontEnd, glArray, wglArray, glXArray)
			generateCPPFile (frontEnd, glArray, wglArray, glXArray)
		end
		
		def writeMembers (frontEnd, file, array)
			array.each { |item|
				file.print "	bool m", item[1].name, ";\n"
			}
		end

		def writeFunctions (frontEnd, file, array)
			array.each { |item|
				file.print "	bool Has_", item[1].name, "() const { return mExtensions.m", item[1].name, "; }\n"
			}
		end
		
		def writeExternFunctionPointers (frontEnd, file, array)
			array.each { |item|
				if item[1].functionList.size != 0
					file.print "#ifdef ", item[0], "\n"
					item[1].functionList.each { |function|
						file.print "extern ", frontEnd.procize(function), " ", function, ";\n"
					}
					file.print "#endif //", item[0], "\n\n"
				end
			}
		end
		
		def	writeStructFunctionPointers (frontEnd, file, array)
			array.each { |item|
				if item[1].functionList.size != 0
					file.print "#ifdef ", item[0], "\n"
					item[1].functionList.each { |function|
						file.print "	", frontEnd.procize(function), " ", function, ";\n"
					}
					file.print "#endif //", item[0], "\n\n"
				end
			}
		end
		
		def writeStructExtensions (frontEnd, file, array)
			array.each { |item|
				file.print "	bool m", item[1].name, ";\n"
			}
		end
				
		def copyFile (destFile, srcName)
			File.open (srcName, "r") { |srcFile|
				srcFile.each_line	{ |srcLine|
					destFile.print srcLine
				}
			}
		end
		
		def winGuardOn
			"#if defined(_WIN32)\n"
		end

		def winGuardOff
			"#endif //_WIN32\n"
		end

		def generateHFile (frontEnd, glArray, wglArray, glXArray)
			print "Generating 'glextensionreg.h'\n"
			File.open("glextensionreg.h", "w")	{ |f|
				copyFile (f, "tocopy/header_begin.txt")
				
				### here we write out a struct with all possible function pointers
				f.print "struct GLExtGenFunctionPointers\n{\n"
				writeStructFunctionPointers (frontEnd, f, glArray)
				f.print "	", winGuardOn
				writeStructFunctionPointers (frontEnd, f, wglArray)
				f.print "	", winGuardOff
				f.print "};\n"
				
				### here we write out all possible extensions
				f.print "struct GLExtGenExtensions\n{\n"
				writeStructExtensions (frontEnd, f, glArray)
				f.print winGuardOn
				writeStructExtensions (frontEnd, f, wglArray)
				f.print winGuardOff
				f.print "};\n"
				
				f.print "\n"
				f.print "class GLExtensionReg\n"
				f.print "{\n"
				f.print "private:\n"
				
				f.print winGuardOn
				f.print "	HDC	mHDC;\n"
				f.print winGuardOff
				### write out extension definitions for gl
				f.print "	GLExtGenExtensions	mExtensions;\n\n"
			
				f.print "public:\n"
				### loop through all extensions
				writeFunctions (frontEnd, f, glArray)

				f.print winGuardOn
				writeFunctions (frontEnd, f, wglArray)
				f.print winGuardOff
				
				f.print "	GLExtensionReg();\n"
				f.print "	~GLExtensionReg();\n"
				
				f.print "	void Init(GLExtGenFunctionPointers *funPtr = 0, GLExtGenExtensions *ext = 0);\n"
				f.printf"	bool 	QueryGLExtension(const char *name);\n"
				f.print winGuardOn
				f.printf"	bool 	QueryWGLExtension(const char *name);\n"
				f.print "	HDC		GetHDC() const {	return mHDC;	}\n"
				f.print "	void	SetHDC(HDC hdc){	mHDC = hdc;		}\n"
				f.print winGuardOff
				f.print "};\n\n"
			
				### write out extern function pointer definitions
				writeExternFunctionPointers (frontEnd, f, glArray)
				
				### write out extern function pointer definitions for wgl with define-guard
				f.print winGuardOn
				writeExternFunctionPointers (frontEnd, f, wglArray)
				f.print winGuardOff
		
				copyFile (f, "tocopy/header_end.txt")
			}
		end

		def writeFunctionPointers(frontEnd, file, array)
			array.each { |item|
				if item[1].functionList.size != 0
					file.print "#ifdef ", item[0], "\n"
					item[1].functionList.each { |function|
						file.print frontEnd.procize(function), " ", function, " = NULL;\n"
					}
					file.print "#endif //", item[0], "\n\n"
				end
			}
		end

		def writeInitMembers(frontEnd, file, array)
			array.each { |item|
				file.print "	mExtensions.m", item[0], " = false;\n"
			}
		end
		
		def writeLoadFunctions(frontEnd, file, array)
			array.each { |item|
				if item[0].index("extensions_string") == nil
					file.print "static bool Load_", item[0], "(GLExtensionReg *reg)\n{\n"
					file.print "#ifdef ", item[0], "\n"
					# query for the extension, unless we are a GL_VERSION
					if item[0].index("VERSION") == nil
						if item[0].index("WGL") == nil
							file.print "	if(reg->QueryGLExtension(\"", item[0], "\") == 0)\n"
						else
							if item[0].index("WGL_NV_vertex_array_range") == nil
								file.print "	if(reg->QueryWGLExtension(\"", item[0], "\") == 0)\n"
							else
								file.print "	if(reg->QueryGLExtension(\"GL_NV_vertex_array_range\") == 0)\n"
							end
						end
						file.print "		return false;\n"
					end
					item[1].functionList.each { |function|
						file.print "	GET_PROC_ADDRESS(", frontEnd.procize(function), ", ", function, ");\n"
					}
					file.print "	return true;\n#endif //", item[0], "\n	return false;\n}\n\n"
				end
			}
		end
		
		def writeCallLoadFunctions (frontEnd, file, array)
			array.each { |item|
				if item[0].index("extensions_string") == nil
					if item[0].index("VERSION") != nil
						file.print "	if (mExtensions.m", item[0], ") Load_", item[0], "(this);\n"
					else
						file.print "	mExtensions.m", item[0], " = Load_", item[0], "(this);\n"
					end
				end
			}
		end
		
		def writeCopyFunctionPointers (frontEnd, file, array)
			array.each { |item|
				if item[1].functionList.size != 0
					file.print "#ifdef ", item[0], "\n"
					item[1].functionList.each { |function|
						file.print "		funPtr->", function, " = ", function, ";\n"
					}
					file.print "#endif //", item[0], "\n\n"
				end
			}
		end


		def generateCPPFile (frontEnd, glArray, wglArray, glXArray)
			print "Generating 'glextensionreg.cpp'\n"
			File.open("glextensionreg.cpp", "w")	{ |f|
				copyFile (f, "tocopy/cpp_begin.txt")
				
				### create global function pointers
				writeFunctionPointers (frontEnd, f, glArray)
				f.print winGuardOn
				writeFunctionPointers (frontEnd, f, wglArray)
				f.print winGuardOff
			
				### create load functions
				writeLoadFunctions (frontEnd, f, glArray)
				f.print winGuardOn
				writeLoadFunctions (frontEnd, f, wglArray)
				f.print winGuardOff + "\n"
		
				f.print "GLExtensionReg::GLExtensionReg()\n"
				f.print "{\n"
				f.print "	memset(&mExtensions, 0, sizeof(GLExtGenExtensions));\n"
				f.print	winGuardOn
				f.print "	mHDC = NULL;\n"
				f.print	winGuardOff
				f.print "}\n\n"
		
				f.print "GLExtensionReg::~GLExtensionReg()\n"
				f.print "{\n"
				f.print "}\n\n"
		
				f.print "void GLExtensionReg::Init(GLExtGenFunctionPointers *funPtr, GLExtGenExtensions *ext)\n"
				f.print "{\n"
					f.print "	char *s = (char*) glGetString(GL_VERSION);\n\n"
		
					f.print "	if (!s)\n"
					f.print "		return;\n"
					f.print "	s = strstr(s, \"1.\");\n"
					f.print "	if (s != NULL)\n"
					f.print "	{\n"
					f.print "		if (s[2] >= '4')\n"
					f.print "			mExtensions.mGL_VERSION_1_4 = true;\n"
					f.print "		if (s[2] >= '3')\n"
					f.print "			mExtensions.mGL_VERSION_1_3 = true;\n"
					f.print "		if (s[2] >= '2')\n"
					f.print "			mExtensions.mGL_VERSION_1_2 = true;\n"    
					f.print "	}\n\n"
			
					### call load functions
					writeCallLoadFunctions(frontEnd, f, glArray)
					f.print winGuardOn
					# here we do some special case processing for the wgl extension stuff
					f.print "    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) GetProcedure(\"wglGetExtensionsStringARB\");\n"
					f.print "    wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) GetProcedure(\"wglGetExtensionsStringEXT\");\n"
					f.print "    mExtensions.mWGL_ARB_extensions_string = wglGetExtensionsStringARB != NULL;\n"
					f.print "    mExtensions.mWGL_EXT_extensions_string = wglGetExtensionsStringEXT != NULL;\n\n"
					writeCallLoadFunctions(frontEnd, f, wglArray)
					f.print winGuardOff + "\n"
					
					f.print "	if (ext)\n"
					f.print "		memcpy(ext, &mExtensions, sizeof(GLExtGenExtensions));\n\n"
					
					f.print "	if (funPtr)\n"
					f.print "	{\n"
						writeCopyFunctionPointers (frontEnd, f, glArray)
						f.print winGuardOn
						writeCopyFunctionPointers (frontEnd, f, wglArray)
						f.print winGuardOff + "\n"
					f.print "	}\n"
				f.print "}\n\n"
			}
		end
	end
end