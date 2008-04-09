Name:           rcssserver3d
Version:        0.5.7
Release:        1%{?dist}
Summary:        Robocup 3D Soccer Simulation Server

Group:          Applications/Engineering
License:        GPLv2
URL:            http://sourceforge.net/projects/sserver/
Source0:        rcssserver3d-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  ruby-devel ode-devel mesa-libGL-devel DevIL-devel SDL-devel
BuildRequires:  freetype-devel mesa-libGLU-devel
Requires:       ruby ode mesa-libGL DevIL SDL freetype mesa-libGLU

%description
This is the simulation server used in Robocup 3D Soccer Simulation contests.

%prep
%setup -q

%build
#./bootstrap
%configure --prefix=%{_prefix} --enable-debug=no %{?_without_wxWidgets}
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc AUTHORS INSTALL NEWS ChangeLog COPYING README README.MacOSX README-soccer RELEASE THANKS TODO doc/TEXT_INSTEAD_OF_A_MANUAL.txt
%{_prefix}/bin/*
%{_prefix}/include/*
%{_prefix}/lib/r*
%{_prefix}/share/*



%changelog
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
