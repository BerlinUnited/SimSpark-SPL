bin_PROGRAMS = rcssmonitor3d

LDADD = -lpthread

if DEBUG
rcssmonitor3d_CXXFLAGS = -O -g -W -Wall
else
rcssmonitor3d_CXXFLAGS = -O2
endif

AM_CPPFLAGS = @SALT_CPPFLAGS@ \
                  @SPARK_CPPFLAGS@ @ZEITGEIST_CPPFLAGS@ \
                  @OXYGEN_CPPFLAGS@ @KEROSIN_CPPFLAGS@

AM_LDFLAGS  = @SALT_LDFLAGS@ \
                  @SPARK_LDFLAGS@ @ZEITGEIST_LDFLAGS@ \
                  @OXYGEN_LDFLAGS@ @KEROSIN_LDFLAGS@

rcssmonitor3d_SOURCES = \
	main.cpp

dist_pkgdata_DATA = \
	soccersim-monitor.rb \
	rcssmonitor3d.rb \
	soccerbindings.rb
	
