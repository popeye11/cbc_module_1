cmake_minimum_required(VERSION 3.10)

find_package(Git QUIET REQUIRED)

# re-run if there are any modifications
set_property(GLOBAL APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/.git/index"
)

# check if this is a local or jenkins build
if(NOT DEFINED ENV{JENKINS_URL})
    set(LOCAL_BUILD "-local")
endif()

# modifications to working tree?
execute_process(
    COMMAND
        ${GIT_EXECUTABLE} diff HEAD
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE
        GIT_DIRTY
)
if(NOT GIT_DIRTY STREQUAL "")
    set(GIT_DIRTY "-dirty")
endif()

# git tag
execute_process(
    COMMAND
        ${GIT_EXECUTABLE} describe --tags --exact-match --exclude devtool*
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE
        GIT_TAG
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# git branch
execute_process(
    COMMAND
        ${GIT_EXECUTABLE} describe --all --first-parent --exclude devtool* --exclude *HEAD
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE
        GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "^(remotes/origin/)|(heads/)" "" GIT_BRANCH "${GIT_BRANCH}")
string(REGEX REPLACE "/" "@" GIT_BRANCH "${GIT_BRANCH}")

# git hash
execute_process(
    COMMAND
        ${GIT_EXECUTABLE} log --pretty=format:%H -n 1
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE
        GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# git short hash
execute_process(
    COMMAND
        ${GIT_EXECUTABLE} log --pretty=format:%h -n 1
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE
        GIT_HASH_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(VERSION "${GIT_BRANCH}.${GIT_HASH_SHORT}${GIT_DIRTY}${LOCAL_BUILD}")

if(GIT_TAG)
    # split git tag in version components
    string(
        REGEX REPLACE
            "^release/([0-9]+)\\.([0-9]+)\\.([0-9]+)(\\-[A-Za-z]+[A-Za-z0-9_]*)?$"
            "\\1;\\2;\\3" _version_items "${GIT_TAG}"
    )
    list(LENGTH _version_items _version_items_length)
    if(_version_items_length EQUAL 3)
        list(GET _version_items 0 VERSION_MAJOR)
        list(GET _version_items 1 VERSION_MINOR)
        list(GET _version_items 2 VERSION_PATCH)
        string(
            REGEX REPLACE
                "^release/[0-9]+\\.[0-9]+\\.[0-9]+(\\-[A-Za-z]+[A-Za-z0-9_]*)$"
                "\\1" VERSION_APPENDIX "${GIT_TAG}"
        )
        if(VERSION_APPENDIX STREQUAL GIT_TAG)
            unset(VERSION_APPENDIX)
        endif()
        set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${GIT_HASH_SHORT}${VERSION_APPENDIX}${GIT_DIRTY}${LOCAL_BUILD}")
    else()
        message(WARNING "Tags must be in the following format: release/<MAJOR>.<MINOR>.<PATCH>[-<APPENDIX>]. The optional appendix must start with a letter and must only contain alphanumerical characters.")
        unset(GIT_TAG)
    endif()
endif()

add_custom_target(
    version
    COMMAND
        echo ${VERSION}
)

string(LENGTH "${VERSION} " VERSION_LENGTH)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/version.hpp.in
    generated/version.hpp)

unset(VERSION_LENGTH)

add_library(
    _lib_version
    INTERFACE
)

target_include_directories(
    _lib_version
    INTERFACE
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

add_library(lib::version ALIAS _lib_version)
