%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            DTests C++ client code and runtime (for writing custom unit tests)
Name:               cli-cpp
Version:            20170612
Release:            1%{?dist}
License:            ASL 2.0
Group:              Development/Tools
Source:             dtests-%{version}.tar.gz
URL:                https://github.com/rh-messaging/cli-cpp
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc-c++
BuildRequires:      boost-devel
BuildRequires:      qpid-proton-cpp-devel
Requires:      boost-filesystem
Requires:      boost-regex
Requires:      boost-system
Requires:      qpid-proton-cpp

%ifarch x86_64
BuildRequires:      qpid-proton-c-devel
BuildRequires:      qpid-cpp-client-devel
Requires:      qpid-proton-c
Requires:      qpid-cpp-client
%endif

%description
C++ clients, tools and runtime libraries

%prep
%setup

%build

%ifarch x86_64
cmake -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_QPID_CLIENT=OFF -DENABLE_QPID_MESSAGING=ON -DENABLE_QPID_PROTON=ON -DBUILD_WITH_UNIT_TESTS=ON -DCMAKE_INSTALL_PREFIX=%{buildroot}/opt/dtests
%else
cmake -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_QPID_CLIENT=OFF -DENABLE_QPID_MESSAGING=OFF -DENABLE_QPID_PROTON=ON -DBUILD_WITH_UNIT_TESTS=ON -DCMAKE_INSTALL_PREFIX=%{buildroot}/opt/dtests
%endif
make

%install
make install

%files
/opt/dtests

%changelog
* Sun Oct  4 2020 Jiri Danek <jdanek@redhat.com> - 20201004
- Fix build requirements, rename package, change license from GPL to ASL
* Wed Sep 30 2015 Middleware Messaging <opiske@redhat.com> - 20150930
- Initial release
