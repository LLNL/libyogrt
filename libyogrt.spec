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
%if 0%ch4
BuildRequires: slurm slurm-devel
%endif

# Disable automatic rpm requirement generation
%define _use_internal_dependency_generator 0
%ifnos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
%define __find_requires %{nil}
%endif

%description
A simple wrapper library that provides a unified get-remaining-time
interface for multiple parallel job scheduling systems.

%prep
%setup -q

%build
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
CC=/usr/bin/gcc
export CC
# Build all of the libraries twice: 32bit versions, and then 64bit versions
TOP="`pwd`"
TMP="$TOP/aix"
rm -rf "$TMP"
for bits in 64 32; do
    OBJECT_MODE=$bits
    export OBJECT_MODE
    %configure -C --program-prefix=%{?_program_prefix:%{_program_prefix}}
    mkdir -p $TMP/orig/$bits
    DESTDIR=$TMP/orig/$bits make install
    make clean
done

# Now merge the 32bit and 64bit versions of the libraries together into
# one composite libyogrt.a library.
for subpackage in none slurm lcrm moab aixslurm; do
    for bits in 32 64; do
        if [ -f $TMP/orig/${bits}%{_libdir}/libyogrt/libyogrt-${subpackage}.a ]; then
            mkdir -p $TMP/$subpackage/${bits}
            cd $TMP/$subpackage/${bits}
            ar -X${bits} x $TMP/orig/${bits}%{_libdir}/libyogrt/libyogrt-${subpackage}.a
        fi
    done

    cd $TMP
    if [ -d $TMP/$subpackage ]; then
        ar -Xany cr libyogrt-${subpackage}.a $TMP/$subpackage/*/*
    fi
    cd $TOP
done
# now merge the 32- and 64-bit versions of the main library
for bits in 32 64; do
	mkdir -p $TMP/${bits}
	cd $TMP/${bits}
	ar -X${bits} x $TMP/orig/${bits}%{_libdir}/libyogrt.a
done
cd $TMP
ar -Xany cr libyogrt.a $TMP/32/* $TMP/64/*
cd $TOP
rm -rf $TMP/orig
%else
%configure
make
%endif

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
CC=/usr/bin/gcc
export CC
%endif
DESTDIR="$RPM_BUILD_ROOT" make install
mv $RPM_BUILD_ROOT%{_sysconfdir}/yogrt.conf.example $RPM_BUILD_ROOT%{_sysconfdir}/yogrt.conf
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
if [ -d aix ]; then
	cp aix/libyogrt-* "$RPM_BUILD_ROOT"%{_libdir}/libyogrt
fi
cp aix/libyogrt.a "$RPM_BUILD_ROOT"%{_libdir}
# slurm plugins must be .so files, not .a, so now we unpack the spank plugin.
cd "$RPM_BUILD_ROOT"%{_libdir}/libyogrt
if [ -f libyogrt-spank-plugin.a ]; then
	ar x ./libyogrt-spank-plugin.a
	mv ./libyogrt-spank-plugin.so.1 ./libyogrt-spank-plugin.so
	rm ./libyogrt-spank-plugin.a
fi
%endif

%files
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
%{_libdir}/*.a
%attr(755, root, root) %dir %{_libdir}/libyogrt
%{_libdir}/libyogrt/*.a
%{_libdir}/libyogrt/libyogrt-spank-plugin.so
%{_libexecdir}/libyogrt_slurm_timed
%else
%{_libdir}/*.*
%{_libdir}/libyogrt/*
%endif
%{_mandir}/*/*
%config(noreplace) %{_sysconfdir}/yogrt.conf

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

