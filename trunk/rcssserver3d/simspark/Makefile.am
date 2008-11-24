bin_PROGRAMS = simspark

LDADD = -lpthread

if DEBUG
agentspark_CXXFLAGS = -O -g -W -Wall
else
agentspark_CXXFLAGS = -O2
endif

AM_CPPFLAGS = @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@ @SALT_CPPFLAGS@ \
                  @SPARK_CPPFLAGS@ @ZEITGEIST_CPPFLAGS@ \
                  @OXYGEN_CPPFLAGS@ @KEROSIN_CPPFLAGS@

if NO_RPATH
AM_LDFLAGS = @RUBY_LDFLAGS@ @SALT_LDFLAGS@ \
                  @SPARK_LDFLAGS@ @ZEITGEIST_LDFLAGS@ \
                  @OXYGEN_LDFLAGS@ @KEROSIN_LDFLAGS@ @BOOST_THREADS_LIB@
else
AM_LDFLAGS  = -Wl,--rpath,$(DESTDIR)$(pkglibdir) @RUBY_LDFLAGS@ \
                  @SALT_LDFLAGS@ @SPARK_LDFLAGS@ @ZEITGEIST_LDFLAGS@ \
                  @OXYGEN_LDFLAGS@ @KEROSIN_LDFLAGS@ @BOOST_THREADS_LIB@
endif



simspark_SOURCES = \
	main.cpp 

dist_pkgdata_DATA = \
	simspark.rb \
	soccersim.rb \
	naosoccersim.rb \
	internalsoccermonitor.rb \
	internalsoccerbindings.rb
