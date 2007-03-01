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
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3

# Build all of the libraries twice: 32bit versions, and then 64bit versions
TOP="`pwd`"
TMP="$TOP/aix"
rm -rf "$TMP"
for bits in 64 32; do
	OBJECT_MODE=$bits
	export OBJECT_MODE
	%configure -C
	mkdir -p $TMP/orig/$bits
	DESTDIR=$TMP/orig/$bits make install
	make clean
done
# Now merge the 32bit and 64bit versions of the libraries together into
# one composite libyogrt.a library.
for subpackage in none slurm lcrm moab; do
	if [ ! -d $TMP/orig/32%{_libdir}/libyogrt/$subpackage ]; then
		continue
	fi
	mkdir -p $TMP/$subpackage/32
	cd $TMP/$subpackage/32
	ar -X32 x $TMP/orig/32%{_libdir}/libyogrt/$subpackage/libyogrt.a

	mkdir -p $TMP/$subpackage/64
	cd $TMP/$subpackage/64
	ar -X64 x $TMP/orig/64%{_libdir}/libyogrt/$subpackage/libyogrt.a

	cd $TMP/$subpackage
	ar -Xany cr libyogrt.a $TMP/$subpackage/*/*
	cd $TOP
done
rm -rf $TMP/orig
%else
%configure
make
%endif

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
DESTDIR="$RPM_BUILD_ROOT" make install
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
if [ -d aix ] ; then
	for subpackage in none slurm lcrm moab; do
		if [ ! -d aix/$subpackage ] ; then continue; fi
		cp aix/$subpackage/libyogrt.a \
			"$RPM_BUILD_ROOT"%{_libdir}/libyogrt/$subpackage
	done
fi
%endif


# Now determine file lists for each subpackage
for subpackage in none slurm lcrm moab; do
	touch ${subpackage}.files
	if [ -d $RPM_BUILD_ROOT%{_libdir}/libyogrt/${subpackage} ]; then
		cat > ${subpackage}.files << ENDOFLIST
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
%{_libdir}/libyogrt/${subpackage}/libyogrt.a
%else
%{_libdir}/libyogrt/${subpackage}/*
%endif
%{_mandir}/*/*
ENDOFLIST
	fi
done

%clean
rm -rf $RPM_BUILD_ROOT

######################################################################
# none subpackage 
%package none
Summary: libyogrt none implementation
Group: System Environment/Base
Provides: libyogrt
Conflicts: libyogrt-slurm libyogrt-lcrm libyogrt-moab

%description none
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the "none" libyogurt wrapper.

%files -f none.files none

%post none
# Create the symlinks to the library
subpackage=none
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subpackage}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun none
# Remove the symlinks to the library
subpackage=none
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

######################################################################
# slurm subpackage 
%package slurm
Summary: libyogrt SLURM implementation
Group: System Environment/Base
Provides: libyogrt
Conflicts: libyogrt-none libyogrt-lcrm libyogrt-moab

%description slurm
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the SLURM libyogurt wrapper.

%files -f slurm.files slurm

%post slurm
# Create the symlinks to the library
subpackage=slurm
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subpackage}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun slurm
# Remove the symlinks to the library
subpackage=slurm
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

######################################################################
# lcrm subpackage 
%package lcrm
Summary: libyogrt LCRM implementation
Group: System Environment/Base
Provides: libyogrt
Conflicts: libyogrt-none libyogrt-slurm libyogrt-moab

%description lcrm
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the LCRM libyogurt wrapper.

%files -f lcrm.files lcrm

%post lcrm
# Create the symlinks to the library
subpackage=lcrm
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subpackage}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun lcrm
# Remove the symlinks to the library
subpackage=lcrm
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

######################################################################
# moab subpackage 
%package moab
Summary: libyogrt MOAB implementation
Group: System Environment/Base
Provides: libyogrt
Conflicts: libyogrt-none libyogrt-slurm libyogrt-moab

%description moab
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.  This package
provides the MOAB libyogurt wrapper.

%files -f moab.files moab

%post moab
# Create the symlinks to the library
subpackage=moab
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	lib=%{_libdir}/libyogrt/${subpackage}/${name}
	if [ -L $lib ]; then
		ln -sf `readlink $lib` %{_libdir}/${name}
	elif [ -e $lib ]; then
		ln -sf $lib %{_libdir}/${name}
	fi
done

%preun moab
# Remove the symlinks to the library
subpackage=moab
lib_names=$(. %{_libdir}/libyogrt/${subpackage}/libyogrt.la; echo -n $library_names)
for name in $lib_names; do
	rm -f %{_libdir}/${name}
done

%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

