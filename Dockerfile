# Arguments for DEV's (comment static FROM and uncomnnet #DEV ones)
ARG UBI_VERSION=9
ARG UBI_BUILD_TAG=latest
ARG UBI_RUNTIME_TAG=latest
ARG IMAGE_BUILD=registry.access.redhat.com/ubi${UBI_VERSION}/ubi-minimal:${UBI_TAG}
ARG IMAGE_BASE=registry.access.redhat.com/ubi${UBI_VERSION}/ubi-minimal:${UBI_RUNTIME_TAG}

ARG PROTON_VERSION=0.39.0

# Build this with something like buildah bud --arch arm64 --volume=/tmp/ccache:/ccache

#DEV FROM $IMAGE_BUILD
FROM quay.io/centos/centos:stream9 as build

RUN curl -OL https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm
RUN rpm -ivh epel-release-latest-9.noarch.rpm
RUN dnf install -y 'dnf-command(config-manager)'
RUN /usr/bin/crb enable

RUN curl -L https://copr.fedorainfracloud.org/coprs/kpvdr/opentelemetry-cpp-rhel/repo/epel-9/kpvdr-opentelemetry-cpp-rhel-epel-9.repo > /etc/yum.repos.d/kpvdr-opentelemetry-cpp-rhel-epel-9.repo
RUN curl -L https://copr.fedorainfracloud.org/coprs/kpvdr/protobuf/repo/epel-9/kpvdr-protobuf-epel-9.repo > /etc/yum.repos.d/kpvdr-protobuf-epel-9.repo

RUN dnf -y --setopt=install_weak_deps=0 --setopt=tsflags=nodocs install \
    ccache findutils git patchelf \
    \
    cmake ninja-build \
    gcc gcc-c++ \
    \
    cyrus-sasl-devel \
    openssl-devel \
    python-devel

COPY . /src
WORKDIR /src

# can't put $(arch) to CCACHE_DIR, https://github.com/moby/moby/issues/29110
# ENV CCACHE_DIR=
ENV CCACHE_COMPRESS=true
ENV CCACHE_MAXSIZE=400MB

RUN git clone --branch=$PROTON_VERSION --depth=1 https://github.com/apache/qpid-proton.git
RUN CCACHE_DIR=/ccache/$(arch) cmake -S qpid-proton -B cmake-build-qpid-proton -GNinja \
    -DENABLE_WARNING_ERROR=OFF \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_INSTALL_PREFIX=cmake-install \
    -DBUILD_BINDINGS=cpp \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TESTING=OFF \
    -DSASL_IMPL=cyrus \
    -DSSL_IMPL=openssl \
    -DPROACTOR=epoll
RUN CCACHE_DIR=/ccache/$(arch) cmake --build cmake-build-qpid-proton
RUN CCACHE_DIR=/ccache/$(arch) cmake --install cmake-build-qpid-proton --config RelWithDebInfo

RUN CCACHE_DIR=/ccache/$(arch) cmake -S . -B cmake-build-cli-cpp -GNinja \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_INSTALL_PREFIX=cmake-install
    #-DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE \
    #-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE
RUN CCACHE_DIR=/ccache/$(arch) cmake --build cmake-build-cli-cpp
RUN CCACHE_DIR=/ccache/$(arch) cmake --install cmake-build-cli-cpp --config RelWithDebInfo

# fixup finding the proton library, this is something that our build should handle by itself, though
RUN for i in cmake-install/bin/*; do patchelf --add-rpath /usr/local/lib64 $i; done

FROM quay.io/centos/centos:stream9

RUN curl -OL https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm
RUN rpm -ivh epel-release-latest-9.noarch.rpm
RUN dnf install -y 'dnf-command(config-manager)'
RUN /usr/bin/crb enable

RUN curl -L https://copr.fedorainfracloud.org/coprs/kpvdr/opentelemetry-cpp/repo/epel-9/kpvdr-opentelemetry-cpp-epel-9.repo > /etc/yum.repos.d/kpvdr-opentelemetry-cpp-epel-9.repo

RUN dnf -y --setopt=install_weak_deps=0 --setopt=tsflags=nodocs install \
    cyrus-sasl cyrus-sasl-gssapi cyrus-sasl-lib cyrus-sasl-plain \
    openssl

COPY --from=build /src/cmake-install/ /usr/local

RUN mkdir /var/lib/cli-cpp && \
    chown -R 1001:0 /var/lib/cli-cpp  && \
    chmod -R g=u /var/lib/cli-cpp

USER 1001

VOLUME /var/lib/cli-cpp
WORKDIR /var/lib/cli-cpp

CMD ["/bin/bash"]
