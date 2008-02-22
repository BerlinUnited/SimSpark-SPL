Name:           rcssserver3d-data
Version:        0.5.6
Release:        1%{?dist}
Summary:        Robocup 3D Soccer Simulation Server Data Files

Group:          Applications/Engineering
License:        GPLv2
URL:            http://sourceforge.net/projects/sserver/
Source0:        rcssserver3d-%{version}-data.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildArch:      noarch
Requires:       rcssserver3d >= 0.5.6

%description
This package includes the data files for the simulation server used in Robocup 3D Soccer Simulation contests.

%prep
%setup -q -c

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_prefix}/share/rcssserver3d/
cp -r * $RPM_BUILD_ROOT/%{_prefix}/share/rcssserver3d/

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%{_prefix}/share/rcssserver3d/*



%changelog
* Thu Feb 14 2008 - Hedayat Vatankhah <hedayat@grad.com>
  -Initial version
