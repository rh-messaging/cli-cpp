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
    npm findutils \
    \
    cmake \
    make \
    gcc \
    gcc-c++ \
    \
    opentelemetry-cpp-devel \
    cyrus-sasl-devel \
    openssl-devel \
    python-devel

RUN npm install -g @bazel/bazelisk

COPY . /src
WORKDIR /src

RUN bazelisk build //:cli-cpp --enable_bzlmod --verbose_failures --sandbox_debug --experimental_ui_max_stdouterr_bytes=104857600 --//:deps=upstream

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

COPY --from=build /src/bazel-bin/copy_qpid-proton/qpid-proton/lib64/ /usr/local/lib64
COPY --from=build /src/bazel-bin/copy_cli-cpp/cli-cpp/bin/ /usr/local/bin

RUN mkdir /var/lib/cli-cpp && \
    chown -R 1001:0 /var/lib/cli-cpp  && \
    chmod -R g=u /var/lib/cli-cpp

USER 1001

VOLUME /var/lib/cli-cpp
WORKDIR /var/lib/cli-cpp

CMD ["/bin/bash"]
