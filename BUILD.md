# Build instructions

## Requirements

This project depends on Qpid Cpp and Qpid Proton messaging libraries.
These have their own dependencies in turn.
Here is (possibly incomplete) list for Fedora/RHEL to install.
(In other words, the build is not hermetic/self-contained, not even with Bazel.)

Cli Cpp

* boost_regex

Proton Cpp

## Two ways to build

This project can be built either with CMake, or with CMake executed by Bazel.

### CMake build

   cmake \
        -DProton_DIR=...
        -DProtonCpp_DIR=...
        -DENABLE_QPID_PROTON=ON
        -DCMAKE_USER_C_FLAGS='-std=c++11'

### Bazel command line

Bazel is used to also build the dependencies, qpid-cpp and qpid-proton.

    bazelisk build //:cli-cpp --enable_bzlmod --//:deps=upstream

You can tell Bazel not to build these dependencies, and they will be then picked by CMake from system location.

    bazelisk build //:cli-cpp --enable_bzlmod --//:deps=rpms

When building with ccache, add the following

    --sandbox_writable_path=$HOME/.ccache

To run the built binaries, use

    LD_LIBRARY_PATH=bazel-bin/copy_qpid-proton/qpid-proton/lib64 bazel-bin/copy_cli-cpp/cli-cpp/bin/aac3_sender --help

#### Troubleshooting

Preserve sandbox with `--sandbox_debug` flag.
Get more output with `--verbose_failures` flag.

Disable sandbox with `--spawn_strategy=standalone` flag, use values of `processwrapper-sandbox`, `linux-sandbox`, ...

If Proton fails to link, complaining about missing `main`,  try adding bazel option `--linkopt="-fuse-ld=gold"`.
See https://github.com/bazelbuild/rules_foreign_cc/issues/863.

### CMake build

If you want to use the dependencies built by Bazel in your CMake build, do this.

    mkdir ext_build_deps
    ln -s `pwd`/bazel-bin/copy_*/*/lib64/cmake/*/*.cmake ext_build_deps/
    ln -s `pwd`/bazel-bin/copy_*/* ext_build_deps/
    cmake -DCMAKE_PREFIX_PATH=`pwd`/cmake_module_path -DEXT_BUILD_DEPS=`pwd`/cmake_module_path

### Flags for libraries

See `BUILD.bazel` for qpid-cpp and qpid-proton flags.
The idea is to build as little as possible.

## Design considerations

It makes sense to build cli-cpp with libs from RPM, libs from upstream, or local checkout.

Further, it makes sense to mix and match tools such as ninja, make, ccache.

Next, the Debug/Release build should be selected as needed.

Bazel can provide caching of build actions, and it can be used both in CI or locally.

## Rebuild images

```shell
podman build -f Dockerfile.fedora37_epel -t quay.io/jdanek/cli-cpp_fedora37_epel
podman build -f Dockerfile.ubi8_epel -t quay.io/jdanek/cli-cpp_ubi8_epel
podman build -f Dockerfile.ubi8_upstream -t quay.io/jdanek/cli-cpp_ubi8_upstream

podman push quay.io/jdanek/cli-cpp_fedora37_epel
podman push quay.io/jdanek/cli-cpp_ubi8_epel
podman push quay.io/jdanek/cli-cpp_ubi8_upstream
```
