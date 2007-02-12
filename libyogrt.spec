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

%description
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.

%prep
%setup -q

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc


%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

