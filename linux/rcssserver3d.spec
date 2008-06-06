Name:           rcssserver3d
Version:        0.5.9
Release:        1%{?dist}
Summary:        Robocup 3D Soccer Simulation Server

Group:          Applications/System
License:        GPLv2
URL:            http://sourceforge.net/projects/sserver/
Source0:        http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz
Source1:        %{name}.png
Source2:        %{name}.desktop
Source3:        %{name}-rsgedit.desktop
Patch0:         rcssserver3d-0.5.9-gcc43_rpath_fix.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  automake autoconf gcc-c++ boost-devel slang-devel
BuildRequires:  ruby ruby-devel SDL-devel desktop-file-utils

Requires:       boost slang ruby SDL

%if 0%{?suse_version} || 0%{?sles_version}
BuildRequires:  libode-devel Mesa-devel libdevil-devel
BuildRequires:  freetype2-devel

Requires:       libode Mesa libdevil freetype2
%define         vendor suse
%else
BuildRequires:  ode-devel mesa-libGL-devel DevIL-devel
BuildRequires:  freetype-devel mesa-libGLU-devel

Requires:       ode mesa-libGL DevIL freetype mesa-libGLU
%define         vendor fedora
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

%description    devel
This package contains the header files and libraries
for %{name}. If you like to develop programs using %{name},
you will need to install %{name}-devel.

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
%patch0 -p1 -b .gcc43_rpath_fix

%build
autoreconf --install
%configure --enable-debug=no --disable-rpath %{?_without_wxWidgets: --without-wxWidgets}
make %{?_smp_mflags}
chmod a-x app/simspark/rsg/agent/nao/*

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
rm -rf $RPM_BUILD_ROOT/%{_libdir}/%{name}/*.la
mkdir -p $RPM_BUILD_ROOT/%{_sysconfdir}/ld.so.conf.d/
echo %{_libdir}/%{name} > $RPM_BUILD_ROOT/%{_sysconfdir}/ld.so.conf.d/%{name}.conf
%if 0%{?_without_wxWidgets:1}
rm -rf $RPM_BUILD_ROOT/%{_includedir}/%{name}/wxutil
%endif
rm -rf $RPM_BUILD_ROOT/usr/bin/{rcsoccersim3D,rcssmonitor3D-kerosin,rcssserver3D,agenttest}

mkdir $RPM_BUILD_ROOT/%{_datadir}/pixmaps/
cp %{SOURCE1} $RPM_BUILD_ROOT/%{_datadir}/pixmaps/
desktop-file-install --vendor="%{vendor}"                    \
  --dir=${RPM_BUILD_ROOT}%{_datadir}/applications         \
  --add-category X-Red-Hat-Base                           \
  %{SOURCE2} %{SOURCE3}

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc AUTHORS  NEWS ChangeLog COPYING README README-soccer THANKS TODO
%doc doc/TEXT_INSTEAD_OF_A_MANUAL.txt
%{_bindir}/*spark*
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/[^ilo]*.so.*
%{_libdir}/%{name}/inputsdl*.so.*
%{_libdir}/%{name}/openglsyssdl*.so.*
%{_libdir}/%{name}/ob*.so
%{_libdir}/%{name}/lib[^w]*.so.*
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/[^x]*
%{_sysconfdir}/ld.so.conf.d/%{name}.conf
%{_datadir}/applications/%{vendor}-%{name}.desktop
%{_datadir}/pixmaps/*

%files devel
%defattr(-,root,root,-)
%{_bindir}/*-config
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/[^w]*
%{_libdir}/%{name}/[^ilo]*.so
%{_libdir}/%{name}/inputsdl*.so
%{_libdir}/%{name}/openglsyssdl*.so
%{_libdir}/%{name}/lib[^w]*.so


%if 0%{!?_without_wxWidgets:1}
%post rsgedit -p /sbin/ldconfig

%postun rsgedit -p /sbin/ldconfig

%files rsgedit
%defattr(-,root,root,-)
%{_bindir}/rsgedit
%{_libdir}/%{name}/*wx*.so.*
#%{_datadir}/%{name}/rsgedit.rb
%{_datadir}/%{name}/xpm*
%doc doc/rsgedit.txt
%{_datadir}/applications/%{vendor}-%{name}-rsgedit.desktop

%files wxGTK-devel
%defattr(-,root,root,-)
%{_libdir}/%{name}/*wx*.so
%{_includedir}/%{name}/wxutil
%endif

%changelog
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
