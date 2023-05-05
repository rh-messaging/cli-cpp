# Arguments for DEV's (comment static FROM and uncomnnet #DEV ones)
ARG UBI_VERSION=9
ARG UBI_BUILD_TAG=latest
ARG UBI_RUNTIME_TAG=latest
ARG IMAGE_BUILD=registry.access.redhat.com/ubi${UBI_VERSION}/ubi-minimal:${UBI_TAG}
ARG IMAGE_BASE=registry.access.redhat.com/ubi${UBI_VERSION}/ubi-minimal:${UBI_RUNTIME_TAG}

#DEV FROM $IMAGE_BUILD
FROM quay.io/centos/centos:stream9 as build

RUN curl -OL https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm
RUN rpm -ivh epel-release-latest-9.noarch.rpm
RUN dnf install -y 'dnf-command(config-manager)'
RUN /usr/bin/crb enable

RUN curl -L https://copr.fedorainfracloud.org/coprs/kpvdr/opentelemetry-cpp/repo/epel-9/kpvdr-opentelemetry-cpp-epel-9.repo > /etc/yum.repos.d/kpvdr-opentelemetry-cpp-epel-9.repo

RUN dnf -y --setopt=install_weak_deps=0 --setopt=tsflags=nodocs install \
    findutils git patchelf \
    \
    cmake ninja-build \
    gcc gcc-c++ \
    \
    opentelemetry-cpp-devel \
    cyrus-sasl-devel \
    openssl-devel \
    python-devel

COPY . /src
WORKDIR /src

RUN git clone --depth=1 https://github.com/apache/qpid-proton.git
RUN cmake -S qpid-proton -B cmake-build-qpid-proton -GNinja \
    -DCMAKE_INSTALL_PREFIX=cmake-install \
    -DBUILD_BINDINGS=cpp \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TESTING=OFF \
    -DSASL_IMPL=cyrus \
    -DSSL_IMPL=openssl \
    -DPROACTOR=epoll
RUN cmake --build cmake-build-qpid-proton
RUN cmake --install cmake-build-qpid-proton --config RelWithDebInfo

RUN cmake -S . -B cmake-build-cli-cpp -GNinja \
    -DCMAKE_INSTALL_PREFIX=cmake-install
    #-DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE \
    #-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE
RUN cmake --build cmake-build-cli-cpp
RUN cmake --install cmake-build-cli-cpp --config RelWithDebInfo

# fixup finding the proton library, this is something that our build should handle by itself, though
RUN for i in cmake-install/bin/*; do patchelf --add-rpath /usr/local/lib64 $i; done

FROM quay.io/centos/centos:stream9

RUN curl -OL https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm
RUN rpm -ivh epel-release-latest-9.noarch.rpm
RUN dnf install -y 'dnf-command(config-manager)'
RUN /usr/bin/crb enable

RUN curl -L https://copr.fedorainfracloud.org/coprs/kpvdr/opentelemetry-cpp/repo/epel-9/kpvdr-opentelemetry-cpp-epel-9.repo > /etc/yum.repos.d/kpvdr-opentelemetry-cpp-epel-9.repo

RUN dnf -y --setopt=install_weak_deps=0 --setopt=tsflags=nodocs install \
    opentelemetry-cpp \
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
