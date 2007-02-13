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
mkdir -p $RPM_BUILD_ROOT
DESTDIR="$RPM_BUILD_ROOT" make install

%clean
rm -rf $RPM_BUILD_ROOT

######################################################################
# none subpackage 
%package none
Summary: libyogrt none implementation
Group: System Environment/Base
Provides: libyogrt

%description none
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the "none" libyogurt wrapper.

%files none
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt-none*

%post none
sub=none
if [ -e %{_libdir}/libyogrt-${sub}.so ]; then
	lib=%{_libdir}/libyogrt-${sub}.so
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/libyogrt.so
	else
		ln -sf $lib %{_libdir}/libyogrt.so
	fi
elif [ -e %{_libdir}/libyogrt-${sub}.a ]; then
	lib=%{_libdir}/libyogrt-${sub}.a
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/libyogrt.a
	else
		ln -sf $lib %{_libdir}/libyogrt.a
	fi
fi
	
######################################################################
# slurm subpackage 
%package slurm
Summary: libyogrt SLURM implementation
Group: System Environment/Base
Provides: libyogrt

%description slurm
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the SLURM libyogurt wrapper.

%files slurm
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt-slurm*

%post slurm
sub=slurm
if [ -e %{_libdir}/libyogrt-${sub}.so ]; then
	lib=%{_libdir}/libyogrt-${sub}.so
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/libyogrt.so
	else
		ln -sf $lib %{_libdir}/libyogrt.so
	fi
elif [ -e %{_libdir}/libyogrt-${sub}.a ]; then
	lib=%{_libdir}/libyogrt-${sub}.a
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/libyogrt.a
	else
		ln -sf $lib %{_libdir}/libyogrt.a
	fi
fi

%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

