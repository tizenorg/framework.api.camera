Name:       capi-media-camera
Summary:    A Camera library in Tizen C API
%if 0%{?tizen_profile_mobile}
Version:    0.1.5
Release:    1
Group:      libdevel
License:    Apache-2.0
%else
Version:    0.1.28
Release:    1
VCS:        framework/api/camera#capi-media-camera_0.1.7-0-29-g9b4eccc16ec6d0fafbde6d4a9f60a9948d3c8128
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
%endif
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
%if "%{_repository}" == "wearable"
BuildRequires:  pkgconfig(gstreamer-0.10)
%endif
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-camcorder)
BuildRequires:  pkgconfig(audio-session-mgr)
BuildRequires:  pkgconfig(capi-base-common)
%if "%{_repository}" == "wearable"
Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig
%endif

%description


%package devel
Summary:  A Camera library in Tizen C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q


%build
%if 0%{?sec_build_binary_debug_enable}
export CFLAGS+=" -DTIZEN_DEBUG_ENABLE"
%endif
%if 0%{?tizen_profile_wearable}
export CFLAGS+=" -DUSE_ASM_LATEST"
cd ./wearable
%else
cd ./mobile
%endif
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%if 0%{?tizen_profile_wearable}
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
%else
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
%endif


make %{?jobs:-j%jobs}

%install
%if 0%{?tizen_profile_wearable}
cd ./wearable
%else
cd ./mobile
%endif
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%if 0%{?tizen_profile_wearable}
%manifest ./wearable/capi-media-camera.manifest
%else
%manifest ./mobile/capi-media-camera.manifest
%endif
%{_libdir}/libcapi-media-camera.so.*
%{_datadir}/license/%{name}


%files devel
%{_includedir}/media/camera.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-media-camera.so


