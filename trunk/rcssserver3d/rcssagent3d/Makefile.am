bin_PROGRAMS = rcssagent3d

LDADD = -lpthread
	
if DEBUG
rcssagent3d_CXXFLAGS = -O -g -W -Wall
else
rcssagent3d_CXXFLAGS = -O2
endif

AM_CPPFLAGS = @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@ @SALT_CPPFLAGS@ \
                  @SPARK_CPPFLAGS@ @ZEITGEIST_CPPFLAGS@ \
                  @OXYGEN_CPPFLAGS@ @KEROSIN_CPPFLAGS@

AM_LDFLAGS  = @RUBY_LDFLAGS@ @SALT_LDFLAGS@ \
                  @SPARK_LDFLAGS@ @ZEITGEIST_LDFLAGS@ \
                  @OXYGEN_LDFLAGS@ @KEROSIN_LDFLAGS@ @BOOST_THREADS_LIB@


rcssagent3d_SOURCES = main.cpp \
	behavior.h \
	soccerbehavior.h \
	soccerbehavior.cpp \
	soccerbotbehavior.h \
	soccerbotbehavior.cpp \
	hoap2behavior.h \
	hoap2behavior.cpp \
	naobehavior.h \
	naobehavior.cpp

