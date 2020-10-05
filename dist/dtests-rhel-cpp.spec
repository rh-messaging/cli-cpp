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
BuildRequires:      qpid-proton-c-devel
BuildRequires:      qpid-cpp-client-devel
BuildRequires:      boost-devel
BuildRequires:      qpid-proton-cpp-devel
Requires:      qpid-proton-c
Requires:      qpid-cpp-client
Requires:      boost-filesystem
Requires:      boost-regex
Requires:      boost-system
Requires:      qpid-proton-cpp


%description
C++ clients, tools and runtime libraries

%prep
%setup -n dtests/dtests/node_data/clients/cpp

%build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_MODERN=ON -DENABLE_QPID_CLIENT=OFF -DENABLE_QPID_MESSAGING=ON -DENABLE_QPID_PROTON=ON -DBUILD_WITH_UNIT_TESTS=ON -DCMAKE_INSTALL_PREFIX=%{buildroot}/opt/dtests
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
