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
rm -rf $TMP/orig
%else
%configure
make
%endif

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
DESTDIR="$RPM_BUILD_ROOT" make install
mv $RPM_BUILD_ROOT%{_sysconfdir}/yogrt.conf.example $RPM_BUILD_ROOT%{_sysconfdir}/yogrt.conf
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
if [ -d aix ]; then
	cp aix/libyogrt* "$RPM_BUILD_ROOT"%{_libdir}/libyogrt
fi
%endif

%files
%defattr(-,root,root,-)
%doc
%{_includedir}/yogrt.h
%ifos aix5.3 aix5.2 aix5.1 aix5.0 aix4.3
%{_libdir}/*.a
%{_libdir}/libyogrt/*.a
%else
%{_libdir}/*
%{_libdir}/libyogrt/*
%endif
%{_mandir}/*/*
%config %{_sysconfdir}/yogrt.conf

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Mon Feb 12 2007 Christopher J. Morrone <morrone@conon.llnl.gov> - 
- Initial build.

