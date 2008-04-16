Name:           rcssserver3d
Version:        0.5.7
Release:        2%{?dist}
Summary:        Robocup 3D Soccer Simulation Server

Group:          Applications/Engineering
License:        GPLv2
URL:            http://sourceforge.net/projects/sserver/
Source0:        rcssserver3d-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%if 0%{?fedora_version} || 0%{?rhel_version} || 0%{?centos_version}
BuildRequires:  gcc-c++ boost-devel slang-devel ruby ruby-devel
BuildRequires:  ode-devel mesa-libGL-devel DevIL-devel SDL-devel
BuildRequires:  freetype-devel mesa-libGLU-devel

Requires:       boost slang ruby ode mesa-libGL DevIL SDL freetype
Requires:       mesa-libGLU
%endif

%if 0%{?suse_version} || 0%{?sles_version}
BuildRequires:  gcc-c++ boost-devel slang-devel ruby ruby-devel
BuildRequires:  libode-devel Mesa-devel libdevil-devel SDL-devel
BuildRequires:  freetype2-devel

Requires:       boost slang ruby libode Mesa libdevil SDL freetype2
%endif

%description
This is the simulation server used in Robocup 3D Soccer Simulation contests.

%if 0%{?_with_wxWidgets: 1}
%package        rsgedit
Summary:        RsgEditor and wxWidgets related libraries
Group:          Applications/Engineering
Requires:       %{name} = %{version}-%{release}
Requires:       wxGTK wxGTK-gl
BuildRequires:  wxGTK-devel

%description    rsgedit
This package contains rsgedit and plugins which use wxWidgets libraries.
%endif

%prep
%setup -q

%build
#./bootstrap
%configure --enable-debug=no %{!?_with_wxWidgets: --without-wxWidgets}
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_sysconfdir}/ld.so.conf.d/
echo %{_libdir}/%{name} > $RPM_BUILD_ROOT/%{_sysconfdir}/ld.so.conf.d/%{name}.conf
%if 0%{!?_with_wxWidgets: 1}
rm -rf $RPM_BUILD_ROOT/%{_includedir}/%{name}/wxutil
%endif

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc AUTHORS INSTALL NEWS ChangeLog COPYING README README.MacOSX README-soccer THANKS TODO doc/TEXT_INSTEAD_OF_A_MANUAL.txt
%{_bindir}/[^r]*
%{_bindir}/rc*
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/[^ilo]*
%{_libdir}/%{name}/inputsdl*
%{_libdir}/%{name}/openglsyssdl*
%{_libdir}/%{name}/ob*
%{_libdir}/%{name}/lib[^w]*
%{_datadir}/%{name}
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/[^w]*
%{_sysconfdir}/ld.so.conf.d/%{name}.conf

%if 0%{?_with_wxWidgets: 1}
%files rsgedit
%defattr(-,root,root,-)
%{_bindir}/rsgedit
%{_libdir}/%{name}/*wx*
#%{_datadir}/%{name}/rsgedit.rb
%{_includedir}/%{name}/wxutil
%endif

%changelog
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
