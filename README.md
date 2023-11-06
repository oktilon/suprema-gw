# Suprema XPass2 device gateway service

[TOC]
## 1. Build requirements

### 1.1. Build system
 - Meson build v.0.53.1 or higher [Getting Meson](https://mesonbuild.com/Getting-meson.html)
 - CMake v. 3.13 or higher [Installing CMake](https://cmake.org/install)


### 1.2. Install build dependencies
Debian/Ubuntu:
``` bash
# gRPC and Protocol Buffers dependencies
# Manual is here https://grpc.io/docs/languages/cpp/quickstart/
sudo apt install build-essential autoconf libtool pkg-config
# This project dependencies
sudo apt install libre2-dev
```

### 1.3. Clone gRPC repo
``` bash
git clone --recurse-submodules -b v1.58.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
```

### 1.4. Build and install gRPC and Protocol Buffers
```bash
cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j 4
make install
popd
```
> Important! It's **strongly** encourage you to install gRPC locally — using an appropriately set ```CMAKE_INSTALL_PREFIX``` — because there is no easy way to uninstall gRPC after you’ve installed it globally.

## 2. Build project
Before configure project, we need to add .local path to PKG_CONFIG_PATH
``` bash
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/share/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig:$HOME/.local/lib/pkgconfig
```

Configure project build:
``` bash
meson setup -D grpc_root=$(echo $HOME/.local) build
```

Build project:
``` bash
ninja -C build
```

