%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            DTest C++ client code and runtime (for writing custom unit tests)
Name:               dtests-cpp-runtime
Version:            20150930
Release:            1%{?dist}
License:            GPLv2+
Group:              System Environment/Tools
Source:             dtests-%{version}.tar.gz
URL:                http://ooo.englab.brq.redhat.com/c/dtests/
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc-c++
BuildRequires:      qpid-proton-c-devel

%description
Crawls Brew, Koji, etc.

%prep
%autosetup -n dtests/dtests/node_data/clients/cpp

%build
make distclean
cmake -DENABLE_MODERN=ON -DENABLE_QPID_CLIENT=OFF -DENABLE_QPID_MESSAGING=ON -DENABLE_QPID_PROTON=ON -DCMAKE_INSTALL_PREFIX=%{buildroot}/opt
make

%install
make install

%files
/opt

%changelog
* Tue Feb 08 2011 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 2.1.5-21
- Rebuilt for https://fedoraproject.org/wiki/Fedora_15_Mass_Rebuild

* Fri Jul 02 2010 Kamil Dudka <kdudka@redhat.com> 2.1.5-20
- handle multi-partition devices with spaces in mount points properly (#608502)
