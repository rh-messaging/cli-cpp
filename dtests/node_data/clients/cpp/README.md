QPID proton dependencies:
-> RHEL >= 6
 yum install swig patch libuuid-devel
-> RHEL 5.11
 yum install swig patch e2fsprogs-devel java-1.7.0-openjdk
     
QPID Proton
git clone git@github.com:cliffjansen/qpid-proton.git -b cpp-work
cd qpid-proton
curl "http://pastebin.test.redhat.com/pastebin.php?dl=281175" -o default-cpp-on.patch
patch -p1 < default-cpp-on.patch
cmake -DSYSINSTALL_BINDINGS=ON -DCMAKE_INSTALL_PREFIX=/opt/devel/qpid-proton -DDEFAULT_CPP=ON -DBUILD_TESTING=ON .
make all test
sudo make install
     
Clients:
RHEL >= 6:
cmake -DENABLE_PROTON=ON -DPROTON_DIR=/opt/devel/qpid-proton -DENABLE_MODERN=ON -DCMAKE_CXX_COMPILER=/usr/bin/c++ .
     
RHEL 5.11:
-> without low level clients
cmake -DENABLE_PROTON=ON -DPROTON_DIR=/opt/devel/qpid-proton -DENABLE_MODERN=ON -DCMAKE_CXX_COMPILER=`which c++` -DCMAKE_VERBOSE_MAKEFILE=OFF -DBUILD_WITH_UNIT_TESTS=OFF -DQPID_WITH_GET_CONTENT_OBJECT=ON -DBOOST_INCLUDE_DIR=/usr/include/boost141/ -DBOOST_LIBRARY_DIR=/usr/lib/boost141/ .

-> with low level clients
cmake -DENABLE_PROTON=ON -DPROTON_DIR=/opt/devel/qpid-proton -DENABLE_MODERN=ON -DCMAKE_CXX_COMPILER=`which c++` -DCMAKE_VERBOSE_MAKEFILE=OFF -DBUILD_WITH_UNIT_TESTS=OFF -DQPID_WITH_GET_CONTENT_OBJECT=ON -DBOOST_INCLUDE_DIR=/usr/include/boost141/ -DBOOST_LIBRARY_DIR=/usr/lib/boost141/ -DENABLE_QPID_LL=ON .

------------- Build details --------------
Enable/Disable Flags:
-DEENABLE_QPID_PROTON -> Enable qpid proton support (messaging/reactor clients)
-DENABLE_MODERN -> Enable new and/or refactored code. It is a requirement for -DENABLE_PROTON
-DENABLE_QPID_MESSAGING -> Enable qpid messaging clients
-DENABLE_QPID_CLIENT -> Enable qpid clients. It requires qpid source code on the build host
-DBUILD_WITH_UNIT_TESTS -> Builds the unit tests for the client and enables a new target ('make test')

With flags
-DQPID_WITH_GET_CONTENT_OBJECT -> Whether qpid code contains getContentObject (defines NGETCONTENTOBJECT)
-DQPID_SOURCE_WITH_SYS_EPOCH -> Defines QPIDSYSEPOCH if set [ default ] or QPID_SOURCE_WITH_SYS_EPOCH if unset