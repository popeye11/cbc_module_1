## Need CMake 3.12 or newer to be able to pass include paths to referencing libraries/executables.
## See https://stackoverflow.com/a/51436025/4631008.
cmake_minimum_required(VERSION 3.12)

project(
    cbc-firmware
    LANGUAGES C CXX ASM
)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Set compiler switches for the whole project.  Note that we later set
## extra compiler options, mostly enabling extra warning messages,
## specific for the firmware code itself, see firmware/CMakeLists.txt.
## Using those options for third-party software would result in a large
## number of warnings in code that we cannot change.
add_definitions(-DSTM32H743xx -DUSE_HAL_DRIVER -DARM_MATH_CM7 -D__FPU_PRESENT)
set(TARGET_FLAGS " -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -specs=${CMAKE_CURRENT_LIST_DIR}/support/standard_c_nano_cpp.specs")

## Note that we always have debugging information in the ELF-file,
## also in release builds.  This does not hurt, as debugging
## information is never flashed to the device anyway.  The options
## "-ffunction-sections" and "-fdata-sections" are only effective if
## the option "--gc-sections" is passed to the linker, see below.
set(COMMON_FLAGS       " -Wall -ffunction-sections -fdata-sections -fstack-usage -ggdb -gdwarf-4 -g3 -fmerge-all-constants -fvar-tracking-assignments -fno-common -fno-builtin-printf -fno-builtin-snprintf")
set(DEBUGGING_FLAGS    " -O0")  # disable optimization for better debugging
set(OPTIMIZATION_FLAGS " -DNDEBUG -Os") # optimize for size (like -O2, but try to keep code size small)

## This works better than the add_compile_options() alternative for the
## following reasons:
## * add_compile_options does not set linker flags while CMAKE_CXX_FLAGS is
##   automatically added to CMAKE_EXE_LINKER_FLAGS,
## * both the variable CMAKE_EXE_LINKER_FLAGS and the target property LINK_FLAGS
##   are treated as strings rather than lists - as in add_compile_options - by
##   CMake. A string to list conversion would hence be required which results in
##   code bloat.
## Also note that we "set" rather than "append" to the flags in order to
## overwrite CMake's default flags.

if (CMAKE_CROSSCOMPILING)
    set(CMAKE_C_FLAGS   "${TARGET_FLAGS} ${COMMON_FLAGS}")
    set(CMAKE_CXX_FLAGS "${TARGET_FLAGS} ${COMMON_FLAGS}")
else(CMAKE_CROSSCOMPILING)
    set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
    set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
endif (CMAKE_CROSSCOMPILING)

## Flags specific to each build type.  These will be appended to the
## flags above.
set(CMAKE_C_FLAGS_DEBUG   ${DEBUGGING_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${DEBUGGING_FLAGS})

set(CMAKE_C_FLAGS_RELEASE   ${OPTIMIZATION_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE ${OPTIMIZATION_FLAGS})

set(CMAKE_C_FLAGS_RELWITHDEBINFO   ${OPTIMIZATION_FLAGS})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${OPTIMIZATION_FLAGS})

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

add_subdirectory(firmware)

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.bin
    DEPENDS ${CMAKE_PROJECT_NAME}.elf
    COMMAND ${CMAKE_OBJCOPY} -O binary --gap-fill=0xFF $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf> ${CMAKE_PROJECT_NAME}.bin
)

add_custom_target(firmware
    DEPENDS ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
)

include(${CMAKE_CURRENT_LIST_DIR}/git-commit-info.cmake)
git_decorated_commit_hash(${PROJECT_SOURCE_DIR} VCS_REVISION)


find_package(Doxygen REQUIRED dot)
if(DOXYGEN_FOUND)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_ENABLE_PREPROCESSING YES)
    set(DOXYGEN_INTERNAL_DOCS YES)
    set(DOXYGEN_MACRO_EXPANSION YES)
    set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
    set(DOXYGEN_PREDEFINED "__attribute__(x)=")
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    set(DOXYGEN_PLANTUML_JAR_PATH /usr/share/plantuml/plantuml.jar)
    set(DOXYGEN_EXCLUDE_PATTERNS
        */RTE/*
        */third_party/*
        README.md
        )
    set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    doxygen_add_docs(
        html-docs
        ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generate HTML documentation"
        )
endif()
