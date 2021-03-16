# DTest: C/C++ QPID client
============

# Introduction


# Building
----

## Dependencies


QPID proton dependencies

* RHEL >= 6

```yum install swig patch libuuid-devel```

* RHEL 5

```yum install swig patch e2fsprogs-devel java-1.7.0-openjdk```

* QPID Proton with experimental CPP Reactor support

```git clone git@github.com:cliffjansen/qpid-proton.git -b cpp-work ;
cd qpid-proton ;
curl "http://pastebin.test.redhat.com/pastebin.php?dl=281175" -o default-cpp-on.patch ;
patch -p1 < default-cpp-on.patch ;
cmake -DSYSINSTALL_BINDINGS=ON -DCMAKE_INSTALL_PREFIX=/opt/devel/qpid-proton -DDEFAULT_CPP=ON -DBUILD_TESTING=ON . ;
make all test ;
sudo make install ;
```


## Build variables

### Global Variables

These variables apply to all code base and may affect what code is built with 
along with the project. For example, they may enable refactored code.

* ENABLE_MODERN: Enable new and/or refactored code. It is a requirement for -DENABLE_QPID_PROTON. 
Default: off


#### Boost libraries

Control where required boost libraries can be found.

* Boost_INCLUDE_DIRS: use to inform the Boost include directory.
* Boost_LIBRARY_DIRS: use to inform the Boost library directory.


#### Unit tests

Control whether or not to enable the unit tests for the client code.

* BUILD_WITH_UNIT_TESTS: Builds the unit tests for the client and enables a new target ('make test').


### Client-specific Variables

These variables control client-specific compilation details.

#### QPID Messaging Build Variables

These variables affect only the QPID Messaging clients.

* ENABLE_QPID_MESSAGING: Enable qpid messaging clients (eg.: aac0, some qc2 and 
some qmf2 binaries). Default: on.


With that flag enabled, the following other variables may also be set:

* QPID_INCLUDE_DIR: QPID Messaging include directory.
* QPID_LIBRARY_DIR: QPID Messaging library directory.
* QPID_WITH_GET_CONTENT_OBJECT: Whether qpid code contains getContentObjectQPID
 'getContentObject'.
* QPID_VERSION: QPID Version.
* QPID_SOURCE: QPID source code directory (required for building QPID's client)
* QPID_SOURCE_INCLUDE_DIR: QPID source code include directory (required for building QPID's client).

##### QPID Clients

* ENABLE_QPID_CLIENT: Enable qpid clients (qpid-send, qpid-receive and its 
qc2-* counterpars). Note: It requires QPID source code on the build host. 
Default: off.
* QPID_SOURCE_WITH_SYS_EPOCH: Defines QPIDSYSEPOCH if set or 
QPID_SOURCE_WITH_SYS_EPOCH if unset. Default: true.
* QPID_WITH_GET_CONTENT_OBJECT: This flags tells the build system whether qpid 
code contains getContentObject (defines NGETCONTENTOBJECT). It may be required 
for some older RHEL/qpid versions. Default: false.

#### QPID Proton

These variables affect only the QPID Proton clients.

* ENABLE_QPID_PROTON: Enable qpid proton messaging/reactor clients (eg.: aac3 
and aac4). This variable requires ENABLE_MODERN.
* PROTON_DIR: QPID Proton base directory.


### Build examples
     
Clients
---

RHEL >= 6, with QPID Proton clients, using a non-standard QPID Proton directory

```
cmake -DENABLE_QPID_PROTON=ON -DPROTON_DIR=/opt/devel/qpid-proton \
    -DENABLE_MODERN=ON -DCMAKE_CXX_COMPILER=`which c++` .
```

RHEL 5.11 without QPID Proton and disabled unit tests

```
cmake -DENABLE_QPID_PROTON=ON -DPROTON_DIR=/opt/devel/qpid-proton \ 
    -DENABLE_MODERN=ON -DCMAKE_CXX_COMPILER=`which c++` \
    -DCMAKE_VERBOSE_MAKEFILE=OFF -DBUILD_WITH_UNIT_TESTS=OFF \
    -DQPID_WITH_GET_CONTENT_OBJECT=ON \
    -DBoost_INCLUDE_DIRS=/usr/include/boost141/ \
    -DBoost_LIBRARY_DIRS=/usr/lib/boost141/ .
```

RHEL 5.11 with QPID Proton and disabled unit tests

```
cmake -DENABLE_QPID_PROTON=ON -DPROTON_DIR=/opt/devel/qpid-proton \
    -DENABLE_MODERN=ON -DCMAKE_CXX_COMPILER=`which c++` `
    -DCMAKE_VERBOSE_MAKEFILE=OFF -DBUILD_WITH_UNIT_TESTS=OFF `
    -DQPID_WITH_GET_CONTENT_OBJECT=ON `
    -DBoost_INCLUDE_DIRS=/usr/include/boost141/ `
    -DBoost_LIBRARY_DIRS=/usr/lib/boost141/ -DENABLE_QPID_CLIENT=ON .
```



## Compiling and other targets


### Compile

To compile the code:

```make -j2 all```

To run the unit tests:

```make test```

### Other targets

To generate the documentation  (the files will be in the html directory within 
the target directory):

```make documentation```

To clean all code, including the build script cache:

```make distclean```


To run quality checks on the code:

```make check```


# Qpid Proton: QE building tips

This document contains some hacking tips for Qpid clients.

Building Qpid Proton with C++ reactive API bindings

Current Version (0.11-SNAPSHOT)

Users with access to the source code should look at INSTALL.md file in the root directory of the project for latest details.

```shell
git clone https://github.com/apache/qpid-proton.git
mkdir build
cd build
mkdir -p /opt/devel/qpid-proton-0.11
cmake -DCMAKE_INSTALL_PREFIX=/opt/devel/qpid-proton-0.11-SNAPSHOT -DSYSINSTALL_BINDINGS=ON DBUILD_PYTHON=ON -DBUILD_PERL=OFF -DSWIG_EXECUTABLE=/usr/bin/swig ..
make && sudo make install
ln -sf /opt/devel/qpid-proton-0.11 /opt/devel/qpid-proton
```

Relevant: Swig executable must be informed to bypass PROTON-949.

Older Versions (< 0.11-SNAPSHOT)

Note: this is, probably, outdated since it is based on a older C++ work branch.

```shell
git clone git@github.com:cliffjansen/qpid-proton.git -b cpp-work
cd qpid-proton
curl "http://pastebin.test.redhat.com/pastebin.php?dl=281175" -o default-cpp-on.patch
patch -p1 < default-cpp-on.patch
cmake -DSYSINSTALL_BINDINGS=ON -DCMAKE_INSTALL_PREFIX=/opt/devel/qpid-proton -DDEFAULT_CPP=ON -DBUILD_TESTING=ON .
make all test
sudo make install
```

Building Qpid Proton from source RPM with tests

Note: the rpm name, version and architecture may vary. Please adjust accordingly.

```shell
curl http://download.devel.redhat.com/brewroot/packages/qpid-proton/0.9/3.el6/src/qpid-proton-0.9-3.el6.src.rpm -O
rpm -ivh qpid-proton-0.9-3.el6.src.rpm
cd ~/rpmbuild/SPECS
curl "http://pastebin.test.redhat.com/pastebin.php?dl=284041" -o make-test.patch
patch -p0 < make-test.patch
cd ..
rpmbuild -ba SPECS/qpid-proton.spec
```

Running Qpid JMS unit tests from RPM

```shell
rpm -ivh qpid-jms-0.1.0-1.el6.src.rpm
cd ~/rpmbuild/SOURCES
unzip qpid-jms-parent-0.1.0-redhat-1-scm-sources.zip
cd rh-qpid-jms
# Download a patch to ignore long-running / timing out tests
curl "http://pastebin.test.redhat.com/pastebin.php?dl=284210" -o ignore-long-running.patch
patch -p1 < ignore-long-running.patch
mvn -Dgeronimo-jms-1-1-spec-version=1.1.1 clean package
```

Running Qpid JMS unit tests from upstream

```shell
git clone https://github.com/apache/qpid-jms.git
cd qpid-jms
curl "http://pastebin.test.redhat.com/pastebin.php?dl=284210" -o ignore-long-running.patch
patch -p1 < ignore-long-running.patch
mvn clean package
```
