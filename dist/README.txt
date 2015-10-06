
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

4. Generate the source code tarball
cd ${BUILD_DIR}
git clone git://ooo.englab.brq.redhat.com/~qacommon/pub/git/dtests.git

cd dtests
git archive --format=tar --prefix=dtests/ HEAD | gzip >~/rpmbuild/SOURCES/dtests-`date +%Y%m%d`.tar.gz

6. Copy the RPM specs
cd /tmp/build/dtests/dtests/node_data/clients/cpp/dist 

7. Build the RPM:

- RHEL 6 / 7
rpmbuild -ba dtest-rhel6-cpp.spec

- Fedora 22
rpmbuild -ba dtest-cpp.spec