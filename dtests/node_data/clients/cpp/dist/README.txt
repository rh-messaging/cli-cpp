
How to generate the packages (very high level steps, for building them from vagrant)

1. Make sure to subscribe to the correct repositories:

- RHEL 6
sudo subscription-manager repos --disable='*' ; sudo subscription-manager repos --enable=rhel-6-server-rpms --enable=rhel-6-server-extras-rpms --enable=rhel-6-server-optional-rpms --enable=rhel-6-server-mrg-messaging-3-rpms

- RHEL 7
sudo subscription-manager repos --disable='*' ; sudo subscription-manager repos --enable=rhel-7-server-rpms --enable=rhel-7-server-extras-rpms --enable=rhel-7-server-optional-rpms --enable=rhel-7-server-mrg-messaging-3-rpms


2. Update the system
sudo yum update -y && sudo yum install rpm-build @Development cmake qpid-proton-c-devel qpid-cpp-server-devel qpid-cpp-client-devel qpid-qmf-devel -y


3. Create the build directories
mkdir /tmp/build
mkdir -p ~/rpmbuild/SOURCES/

4. Copy the source code tarball
cp dtests-20150930.tar.gz ~/rpmbuild/SOURCES/

5. Extract
tar -xvf dtests-20150930.tar.gz -C /tmp/build

6. Copy the RPM specs
cp *.spec /tmp/build/dtests/dtests/node_data/clients/cpp/dist
cd /tmp/build/dtests/dtests/node_data/clients/cpp/dist 

6. Build the RPM:

- RHEL 6 / 7
rpmbuild -ba dtest-rhel6-cpp.spec

- Fedora 22
rpmbuild -ba dtest-cpp.spec