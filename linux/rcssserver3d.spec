Name:           rcssserver3d
Version:        0.5.7pre
Release:        1%{?dist}
Summary:        Robocup 3D Soccer Simulation Server

Group:          Applications/Engineering
License:        GPLv2
URL:            http://sourceforge.net/projects/sserver/
Source0:        rcssserver3d-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  ruby-devel ode-devel freeglut-devel mesa-libGL-devel DevIL-devel SDL-devel freetype-devel
Requires:       ruby ode freeglut mesa-libGL DevIL SDL freetype rcssserver3d-data >= 0.5.6

%description
This is the simulation server used in Robocup 3D Soccer Simulation contests.

%prep
%setup -q

%build
#./bootstrap
%configure --prefix=%{_prefix} --enable-debug=no
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
* Thu Feb 14 2008 - Hedayat Vatankhah <hedayat@grad.com>
  -Initial version
