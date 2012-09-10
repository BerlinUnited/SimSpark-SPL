Name:           rcssserver3d
Version:        0.6
Release:        3%{?dist}
Summary:        Robocup 3D Soccer Simulation Server

Group:          Applications/System
License:        GPLv2
URL:            http://sourceforge.net/projects/sserver/
Source0:        http://downloads.sourceforge.net/sserver/%{name}-%{version}.tar.gz

# Source 2 & 3 are created by myslef. Source1 is created by me using the photo in the user-manual.pdf
Source1:        %{name}.png
Source2:        %{name}.desktop
Source3:        %{name}-rsgedit.desktop
Patch0:         %{name}-0.6-libnamefix.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  automake autoconf gcc-c++ boost-devel slang-devel
BuildRequires:  ruby ruby-devel SDL-devel desktop-file-utils libtool
Requires:       ruby

%if 0%{?suse_version} || 0%{?sles_version}
BuildRequires:  libode-devel Mesa-devel libdevil-devel
BuildRequires:  freetype2-devel

Requires:       libode Mesa libdevil freetype2
%define         linvendor suse
%else
BuildRequires:  ode-devel libGL-devel DevIL-devel
BuildRequires:  freetype-devel libGLU-devel
Requires:       ruby(abi) = 1.8
%define         linvendor fedora
%endif

%description
The RoboCup Soccer Simulator is a research and educational tool for multi-agent
systems and artificial intelligence. It enables for two teams of 11 simulated
autonomous robotic players to play soccer (football).

This package contains the 3D version of the simulator.


%package        devel
Summary:        Header files and libraries for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       boost-devel ruby-devel ode-devel DevIL-devel
Requires:       libGL-devel libGLU-devel

%description    devel
This package contains the header files and libraries
for %{name}. If you like to develop programs using %{name},
you will need to install %{name}-devel.

%package        doc
Summary:        Users manual for %{name}
Group:          Documentation

%description    doc
This package contains the user documentation
for %{name}. If you like to develop agents for %{name},
you will find %{name}-doc package useful.

%define _without_wxWidgets 1

%if 0%{!?_without_wxWidgets:1}
%package        rsgedit
Summary:        RsgEditor and wxWidgets related libraries
Group:          Applications/System
Requires:       %{name} = %{version}-%{release}
Requires:       wxGTK wxGTK-gl
BuildRequires:  wxGTK-devel

%description    rsgedit
This package contains rsgedit and %{name} plugins which use wxWidgets libraries.


%package        wxGTK-devel
Summary:        wxWidgets related header files and libraries for %{name}
Group:          Development/Libraries
Requires:       %{name}-devel = %{version}-%{release}
Requires:       %{name}-rsgedit = %{version}-%{release}
Requires:       wxGTK-devel

%description    wxGTK-devel
This package contains wxWidgets related header files and libraries
for %{name}. If you like to develop programs using wxWidgets plugins of %{name},
you will need to install %{name}-wxGTK-devel.
%endif

%prep
%setup -q
%patch0 -p1 -b .libnamefix
autoreconf --install
chmod a-x app/simspark/rsg/agent/nao/*
find -name "*.cpp" -exec chmod a-x {} \;
find -name "*.h" -exec chmod a-x {} \;
sed -i.stamp -e 's|cp -r|cp -pr|' app/simspark/Makefile.{am,in}

%build
%configure --enable-debug=no %{?_without_wxWidgets: --without-wxWidgets}
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot} INSTALL="install -p" CPPROG="cp -p"

mkdir %{buildroot}/%{_datadir}/pixmaps/
cp -p %{SOURCE1} %{buildroot}/%{_datadir}/pixmaps/

desktop-file-install --vendor="%{linvendor}"                 \
  --dir=%{buildroot}/%{_datadir}/applications %{SOURCE2} %{SOURCE3}

rm -rf %{buildroot}/%{_libdir}/%{name}/*.la
rm -rf %{buildroot}/%{_bindir}/{rcsoccersim3D,rcssmonitor3D-*,rcssserver3D,agenttest}
rm -rf %{buildroot}/%{_datadir}/%{name}/*.h
%if 0%{?_without_wxWidgets:1}
rm -rf %{buildroot}/%{_includedir}/%{name}/wxutil
rm -rf %{buildroot}/%{_datadir}/%{name}/xpm*
rm -rf %{buildroot}/%{_datadir}/applications/%{linvendor}-%{name}-rsgedit.desktop
%endif

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc AUTHORS  NEWS ChangeLog COPYING README README-soccer THANKS TODO
%doc doc/TEXT_INSTEAD_OF_A_MANUAL.txt
%{_bindir}/*spark*
%dir %{_libdir}/%{name}
# Notice: the package needs .so files for running so
# they can't be moved to -devel package
%{_libdir}/%{name}/[^ilo]*.so*
%{_libdir}/%{name}/inputsdl*.so*
%{_libdir}/%{name}/openglsyssdl*.so*
%{_libdir}/%{name}/ob*.so
%{_libdir}/%{name}/lib[^w]*.so.*
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/[^x]*
%{_datadir}/applications/%{linvendor}-%{name}.desktop
%{_datadir}/pixmaps/*

%files devel
%defattr(-,root,root,-)
%{_bindir}/*-config
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/[^w]*
#%{_libdir}/%{name}/[^ilo]*.so
#%{_libdir}/%{name}/inputsdl*.so
#%{_libdir}/%{name}/openglsyssdl*.so
%{_libdir}/%{name}/lib[^w]*.so
%doc TODO doc/devel/howtos doc/devel/manual.pdf

%files doc
%defattr(-,root,root,-)
%doc doc/users/user-manual.pdf doc/manual/manual.pdf


%if 0%{!?_without_wxWidgets:1}
%files rsgedit
%defattr(-,root,root,-)
%{_bindir}/rsgedit
%{_libdir}/%{name}/*wx*.so.*
#%{_datadir}/%{name}/rsgedit.rb
%{_datadir}/%{name}/xpm*
%doc doc/rsgedit.txt
%{_datadir}/applications/%{linvendor}-%{name}-rsgedit.desktop

%files wxGTK-devel
%defattr(-,root,root,-)
%{_libdir}/%{name}/*wx*.so
%{_includedir}/%{name}/wxutil
%endif

%changelog
* Wed Jul 02 2008 Hedayat Vatankhah <hedayat@grad.com> 0.6-3
- Added a patch to rename libtinyxml to libtinyxml_ex since it differs from libtinyxml

* Sat Jun 28 2008 Hedayat Vatankhah <hedayat@grad.com> 0.6-3
- Fixed Source0 URL
- Added a comment about Source1/2/3
- Replaced an incorrect path (/usr/bin) with the correct macro
- Changing "cp -r" commands in simspark makefiles with cp -pr

* Wed Jun 25 2008 Hedayat Vatankhah <hedayat@grad.com> 0.6-2
- Added missing dependencies for -devel subpackage

* Tue Jun 24 2008 Hedayat Vatankhah <hedayat@grad.com> 0.6-1
- fixed according to the Fedora package review process:
  - changed some requirements
  - removed -doc version
  - removed ld.so.conf.d/rcssserver3d.conf file in favour of rpath!!

* Mon Jun 23 2008 Hedayat Vatankhah <hedayat@grad.com> 0.6-1
- updated for 0.6 release

* Fri Jun 20 2008 Hedayat Vatankhah <hedayat@grad.com> 0.6-0.1.20080620cvs
- preparing for 0.6 release

* Thu Jun 12 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.9-1.20080611cvs
- removing rcssmonitor3D-lite
- update the package to use CVS version which contains Fedora packaging fixes
- added -doc subpackage
- added some documentation to -devel package

* Fri Jun 6 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.9-1
- added a patch to fix gcc43 compile errors and add --disable-rpath
  configure option. this patch is created using upstream CVS tree.s

* Thu Jun 5 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.9-1
- updated for 0.5.9 release
- preparing according to Fedora packaging guidelines:
  added -devel packages
  removed .la files from RPMs

* Thu Apr 17 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.7-2
- added some missing dependencies
- some cleanup
- changed to be more general. now it supports OpenSuse too (however it needs
  3rd party DevIL packages)
- removed RELEASE from doc files since it is not available in released versions

* Sun Apr 13 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.7-2
- updated an ugly wildcard! a little better.

* Fri Apr 11 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.7-2
- added ldconfig config file
- run ldconfig after install/uninstall
- some cleanup
- added subpackage rcssserver3d-rsgedit. TODO: file list should be expressed better
  this package will be built only when "--with wxWidgets" option is passed to rpmbuild

* Wed Apr 9 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.7-1
- added without-wxWidgets option to configure if availabe

* Thu Feb 28 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.7-1
- removed rcssserver3d-data as a requirement since the files are in the
  distribution now
- added libGLU stuff as requirements

* Wed Feb 27 2008 Hedayat Vatankhah <hedayat@grad.com> 0.5.7pre-1
- removed freeglut as a requirement since it is needed for rcssmonitor3d-lite only

* Thu Feb 14 2008 - Hedayat Vatankhah <hedayat@grad.com>
- Initial version
