cmake_minimum_required(VERSION 3.10)

project(TSP_Matlab CXX)

include(../version.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXECUTABLE_SUFFIX .mexw64)

set(3rdParty ../3rdParty)

add_subdirectory(${3rdParty} 3rdParty)
add_subdirectory(../lib lib)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/version.txt.in
    version.txt)

add_compile_definitions(
    MATLAB_DEFAULT_RELEASE=R2018a
    MATLAB_MEX_FILE
)

include_directories(
    ${3rdParty}/MATLAB/R2020b/extern/include
    ${3rdParty}/MATLAB/R2020b/simulink/include
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
add_compile_options(
    -fexceptions
    -fPIC
    -fno-omit-frame-pointer
    -pthread
)

add_link_options(
    -pthread
    -shared
    -Wl,--no-undefined
    -Wl,--as-needed
)

link_directories(
    ${3rdParty}/MATLAB/R2020b/extern/lib/win64/mingw64
)

link_libraries(
    lib::tsp
    mx
    mex
    mat
    m
)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
add_compile_options(
    /Zp8
    /GR
    /W3
    /EHs
    /nologo
    /MD
    /D_CRT_SECURE_NO_DEPRECATE
    /D_SCL_SECURE_NO_DEPRECATE
    /D_SECURE_SCL=0
)

add_link_options(
    /nologo
    /manifest
    /DLL
    /EXPORT:mexFunction
    /EXPORT:mexfilerequiredapiversion
)

link_directories(
    ${3rdParty}/MATLAB/R2020b/extern/lib/win64/microsoft
)

link_libraries(
    lib::tsp
    libmx
    libmex
    libmat
)
endif()

install(
    FILES
        tsp_iir.cpp
        tsp_pid.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/version.txt
    DESTINATION
        tsp
)

add_executable(
    tsp_pid
    tsp_pid.cpp
    ${3rdParty}/MATLAB/R2020b/extern/version/cpp_mexapi_version.cpp
)

add_executable(
    tsp_iir
    tsp_iir.cpp
    ${3rdParty}/MATLAB/R2020b/extern/version/cpp_mexapi_version.cpp
)

install(
    TARGETS
        tsp_iir
        tsp_pid
    RUNTIME
    DESTINATION
        tsp
)
