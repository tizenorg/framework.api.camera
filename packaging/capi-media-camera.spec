Name:       capi-media-camera
Summary:    A Camera library in Tizen C API
Version:    0.1.3
Release:    1
Group:      libdevel
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-camcorder)
BuildRequires:  pkgconfig(capi-base-common)
Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A Camera library in Tizen C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q


%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest capi-media-camera.manifest
%{_libdir}/libcapi-media-camera.so.*
%{_datadir}/license/%{name}


%files devel
%{_includedir}/media/camera.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-media-camera.so

