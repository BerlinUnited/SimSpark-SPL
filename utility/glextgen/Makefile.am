EXTRA_DIST = README glextgen.rb.in ${rubysources} ${copyfiles}

rubysources = script/glbackend.rb script/glextension.rb script/glextgen.rb \
              script/glfrontend.rb script/glmunger.rb
copyfiles =   tocopy/cpp_begin.txt tocopy/cpp_query.txt \
	      tocopy/header_begin.txt tocopy/header_end.txt

all-local: glextensionreg.cpp glextensionreg.h

clean-local:
	rm -f glextensionreg.cpp glextensionreg.h

glextensionreg.cpp glextensionreg.h: glextgen.rb ${rubysources} ${copyfiles}
	ruby glextgen.rb kerosin
