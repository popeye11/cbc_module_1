set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

find_program(CMAKE_SIZE NAMES ${_CMAKE_TOOLCHAIN_PREFIX}size HINTS ${_CMAKE_TOOLCHAIN_LOCATION})
mark_as_advanced(CMAKE_SIZE)
