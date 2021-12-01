cmake_minimum_required(VERSION 2.8)

## Expect the following variables to have a value.
foreach(v MAJOR MINOR PATCH SOURCE_DIR OUTPUT)
    if("${${v}}" STREQUAL "")
        message(FATAL_ERROR "Required variable ${v} is empty")
    endif()
endforeach()

## Suffix must also be present, but can be empty, e.g. in release builds.
if(NOT DEFINED SUFFIX)
    message(FATAL_ERROR "Required variable SUFFIX is not defined")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/git-commit-info.cmake)
git_decorated_commit_hash(${SOURCE_DIR} VCS_REVISION)

if("${VCS_REVISION}" STREQUAL "")
    message(FATAL_ERROR "Could not fetch versioning information")
else()
    message(STATUS "Current revision is ${VCS_REVISION}")
endif()

set(TEMP "${OUTPUT}__")

file(WRITE "${TEMP}" "const char firmware_version[]= \"${MAJOR}.${MINOR}.${PATCH}.${VCS_REVISION}${SUFFIX}\";\n")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TEMP}" "${OUTPUT}")
execute_process(COMMAND ${CMAKE_COMMAND} -E remove "${TEMP}")


