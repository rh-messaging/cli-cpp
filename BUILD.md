# Build instructions

## Two ways to build

This project can be built either with CMake, or with CMake executed by Bazel.

### CMake build

   cmake \
        -DQpid_DIR=...
        -DProton_DIR=...
        -DProtonCpp_DIR=...
        -DENABLE_QPID_MESSAGING=OFF
        -DENABLE_QPID_PROTON=ON
        -DCMAKE_USER_C_FLAGS='-std=c++11'

### Bazel command line

Bazel is used to also build the dependencies, qpid-cpp and qpid-proton.

    bazelisk build //:cli-cpp --//:deps=upstream

You can tell Bazel not to build these dependencies, and they will be then picked by CMake from system location.

    bazelisk build //:cli-cpp --//:deps=rpms

When building with ccache, add the following

    --sandbox_writable_path=$HOME/.ccache

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

