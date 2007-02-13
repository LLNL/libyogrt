Summary: Your One Get Remaining Time library.
Name: See META file
Version: See META file
Release: See META file
License: Proprietary
Group: System Environment/Base
#URL: 
Packager: Christopher J. Morrone <morrone2@llnl.gov>
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root


%prep
%setup -q

%build
%configure
make

%install slurm
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
DESTDIR="$RPM_BUILD_ROOT" make install

%clean
rm -rf $RPM_BUILD_ROOT

%package none
Summary: libyogrt none implementation
Group: System Environment/Base

%files none
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt-none*

%package slurm
Summary: libyogrt SLURM implementation
Group: System Environment/Base

%files slurm
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt-slurm*

%description
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.

%description none
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.

%description slurm
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.

%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

