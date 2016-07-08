%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            DTests C++ client code and runtime (for writing custom unit tests)
Name:               dtests-cpp-runtime
Version:            20160704
Release:            1%{?dist}
License:            GPLv2+
Group:              Development/Tools
Source:             dtests-%{version}.tar.gz
URL:                http://ooo.englab.brq.redhat.com/c/dtests/
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc-c++
BuildRequires:      qpid-proton-c-devel
BuildRequires:      qpid-cpp-client-devel
BuildRequires:      qpid-cpp-server-devel
BuildRequires:      boost-devel
BuildRequires:      qpid-proton-cpp-devel
Requires:      qpid-proton-c
Requires:      qpid-cpp-client
Requires:      qpid-cpp-server
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
* Wed Sep 30 2015 Middleware Messaging <opiske@redhat.com> - 20150930
- Initial release
