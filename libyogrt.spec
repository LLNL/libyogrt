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
Conflicts: libyogrt-slurm libyogrt-lcrm

%description none
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the "none" libyogurt wrapper.

%files none
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt/none/*

%post none
# Create the symlinks to the library
subdir=none
lib_names=$(. %{_libdir}/libyogrt/${subdir}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subdir}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun none
# Remove the symlinks to the library
subdir=none
lib_names=$(. %{_libdir}/libyogrt/${subdir}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

######################################################################
# slurm subpackage 
%package slurm
Summary: libyogrt SLURM implementation
Group: System Environment/Base
Provides: libyogrt
Conflicts: libyogrt-none libyogrt-lcrm

%description slurm
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the SLURM libyogurt wrapper.

%files slurm
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt/slurm/*

%post slurm
# Create the symlinks to the library
subdir=slurm
lib_names=$(. %{_libdir}/libyogrt/${subdir}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subdir}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun slurm
# Remove the symlinks to the library
subdir=slurm
lib_names=$(. %{_libdir}/libyogrt/${subdir}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

######################################################################
# lcrm subpackage 
%package lcrm
Summary: libyogrt LCRM implementation
Group: System Environment/Base
Provides: libyogrt
Conflicts: libyogrt-none libyogrt-slurm

%description lcrm
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the LCRM libyogurt wrapper.

%files lcrm
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%{_libdir}/libyogrt/lcrm/*

%post lcrm
# Create the symlinks to the library
subdir=lcrm
lib_names=$(. %{_libdir}/libyogrt/${subdir}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subdir}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun lcrm
# Remove the symlinks to the library
subdir=lcrm
lib_names=$(. %{_libdir}/libyogrt/${subdir}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

