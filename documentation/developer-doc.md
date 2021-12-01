@page developer_docs CBC Firmware Developer documentation

# Cloning the repo
```sh
git clone ssh://git@bitbucket.toptica.com:7999/cbc/cbc-firmware.git
git submodule init
git submodule update
```
# Building the Firmware

```sh
mkdir build && cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../gnu-arm-none-eabi.toolchain.cmake ..
ninja firmware
```
