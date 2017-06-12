PREFIX?=/opt
PROJECT_NAME=dtests
BASEDIR=$(PREFIX)/dtests/$(PROJECT_NAME)

USERNAME=repoadm
SERVER=messaging-qe-repo.usersys.redhat.com

VERSION=20170612
DISTRO_NAME?=rhel
REPO_PATH=/home/repoadm/public_html/repos/$(DISTRO_NAME)/$(DISTRO_VERSION)/$(DISTRO_ARCH)

COMMONSRC=

.PHONY: depend clean

# default: install

rpm-clean:
	@rm -f ${HOME}/rpmbuild/SOURCES/*
	@rm -f ${HOME}/rpmbuild/BUILDROOT/*

rpm-prep: rpm-clean
	mkdir -p ${HOME}/rpmbuild/SOURCES/
	tar -cf ${HOME}/rpmbuild/SOURCES/${PROJECT_NAME}-${VERSION}.tar.gz ../../../../../../dtests --gzip

rpm: rpm-prep
	rpmbuild -ba $(PROJECT_NAME)-$(DISTRO_NAME)-cpp.spec

upload: rpm
	scp -o "HostbasedAuthentication no" -o "StrictHostKeyChecking no" -r ${HOME}/rpmbuild/RPMS/$(DISTRO_ARCH)/*.rpm ${USERNAME}@${SERVER}:${REPO_PATH}

uninstall:
	rm -rf $(BASEDIR)
