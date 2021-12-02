add_subdirectory(RTE)
add_subdirectory(application)
add_subdirectory(third_party/tsp)
add_subdirectory(drivers/peripherals)
add_subdirectory(components/DDS)
add_subdirectory(components/FIR)
add_subdirectory(components/LockIn)
add_subdirectory(components/CtrlLoop)
add_subdirectory(hal)
## Firmware version
set(FIRMWARE_VERSION_MAJOR "1")
set(FIRMWARE_VERSION_MINOR "0")
set(FIRMWARE_VERSION_PATCH "0")
set(FIRMWARE_VERSION_APPENDIX "-alpha")

## Generate a source file containing version information including a revision
## hash.
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/firmware-version.c
    ## The following dummy output causes this command to always be re-run before
    ## the actual build.
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_ALWAYS_RUN
    COMMAND ${CMAKE_COMMAND}
        -DSOURCE_DIR="${PROJECT_SOURCE_DIR}"
        -DMAJOR="${FIRMWARE_VERSION_MAJOR}"
        -DMINOR="${FIRMWARE_VERSION_MINOR}"
        -DPATCH="${FIRMWARE_VERSION_PATCH}"
        -DSUFFIX="${FIRMWARE_VERSION_APPENDIX}"
        -DOUTPUT="${CMAKE_CURRENT_BINARY_DIR}/firmware-version.c"
        -P "${PROJECT_SOURCE_DIR}/generate_version_information.cmake"
)

add_executable(${CMAKE_PROJECT_NAME}.elf
    support/hard_fault_handler.c
    support/syscalls.c
    support/startup_stm32h743xx.s
    ${CMAKE_CURRENT_BINARY_DIR}/firmware-version.c
)

set_source_files_properties(
    support/syscalls.c
    support/hard_fault_handler.c
    support/startup_stm32h743xx.s
    PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)

set_target_properties(${CMAKE_PROJECT_NAME}.elf PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/STM32H743ZI_FLASH.ld)

## Extra compiler options, mostly warnings, that can not unfortunately
## be enabled for the whole project.
target_compile_options(${CMAKE_PROJECT_NAME}.elf PUBLIC
    -Werror
    -Wextra
    -Wcast-align
    -Wconversion
    -Wdouble-promotion
    -Wformat-overflow
    -Wformat-truncation=2
    -Wformat=2
    -Wshadow
    -Wundef
    )

target_link_options(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    "LINKER:-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map"
    "SHELL:-T ${CMAKE_CURRENT_SOURCE_DIR}/STM32H743ZI_FLASH.ld"
)

target_link_libraries(${CMAKE_PROJECT_NAME}.elf
    application
    rte
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}.elf
    POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf>
)
