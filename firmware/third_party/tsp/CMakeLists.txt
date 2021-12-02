cmake_minimum_required(VERSION 3.10)

project(tsp CXX)

include(version.cmake)

option(TSP_BUILD_UNITTESTS "Build TSP unittests (requires Boost::unit_test_framework)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc++11-compat")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc++14-compat")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wreorder")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wtype-limits")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

add_compile_options(
    -Wall
    -Wconversion
    -Wshadow
    -Wunused-parameter
    -Wformat=2
    -Wnull-dereference
    -Wshift-negative-value
    -Wtautological-compare
    -Wmissing-include-dirs
    -Wmissing-field-initializers
    -Waddress
    -Wchar-subscripts
    -Wcomment
    -Wempty-body
    -Wenum-compare
    -Werror
    -Wformat-extra-args
    -Wignored-qualifiers
    -Winit-self
    -Wlogical-not-parentheses
    -Wmain
    -Wmemset-transposed-args
    -Wmissing-braces
    -Wnarrowing
    -Wnonnull
    -Wparentheses
    -Wreturn-type
    -Wsequence-point
    -Wsign-compare
    -Wsizeof-pointer-memaccess
    -Wstrict-aliasing
    -Wstrict-overflow=1
    -Wswitch
    -Wtrigraphs
    -Wuninitialized
    -Wunknown-pragmas
    -Wunused-result
    -Wunused-function
    -Wunused-label
    -Wunused-value
    -Wunused-variable
    -Wvolatile-register-var
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
add_compile_options(
    -Wuseless-cast
    -Wduplicated-cond
    -Wduplicated-branches
    -Wrestrict
    -Wmemset-elt-size
    -Wimplicit-fallthrough=3
    -Wnonnull-compare
    -Wlogical-op
    -Warray-bounds=1
    -Wbool-compare
    -Wclobbered
    -Wmaybe-uninitialized
    -Wunused-but-set-parameter
    -Wunused-but-set-variable
)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
add_compile_options(
    -Wextra
    -Wheader-hygiene
    -Wcovered-switch-default
    -Wvec-elem-size
    -Wimplicit-fallthrough
    -Wlong-long
    -Warray-bounds
    -Wunused-const-variable
    -Wno-c++98-compat
    -Wno-c++98-c++11-compat
    -Wno-c++98-c++11-c++14-compat
)
endif()

# Doxygen target should only be available when not cross-compiling,
# otherwise find_package(Doxygen) does not work since it requires
# either WIN32 or UNIX to be set. (See TSP-47)
if(WIN32 OR UNIX)
    find_package(Doxygen 1.8 REQUIRED dot)
    set(DOXYGEN_PROJECT_NAME "Toptica dSP library")
    set(DOXYGEN_PROJECT_BRIEF "")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    set(DOXYGEN_IMAGE_PATH "assets")
    set(DOXYGEN_JAVADOC_BANNER YES)
    set(DOXYGEN_EXCLUDE_PATTERNS "*/3rdParty/*")
    if (WIN32)
        # JAVA_HOME should point to "C:\Program Files (x86)\Java\jre1.8.0_281"
        set(DOXYGEN_PLANTUML_JAR_PATH "$ENV{JAVA_HOME}/lib/ext/plantuml.jar" CACHE PATH "Path to the PlantUML tool")
        if(NOT EXISTS "${DOXYGEN_PLANTUML_JAR_PATH}")
            message(FATAL_ERROR "plantuml.jar not found!")
        endif()
    elseif(UNIX)
        if(EXISTS "/usr/share/plantuml/plantuml.jar")
            set(DOXYGEN_PLANTUML_JAR_PATH "/usr/share/plantuml/plantuml.jar")
        elseif(EXISTS "/usr/share/java/plantuml.jar")
            set(DOXYGEN_PLANTUML_JAR_PATH "/usr/share/java/plantuml.jar")
        else()
            message(FATAL_ERROR "plantuml.jar not found!")
        endif()
    endif()

    doxygen_add_docs(
        ${PROJECT_NAME}_doxygen
        ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generate HTML documentation"
    )

    if(NOT TARGET doxygen)
        add_custom_target(
            doxygen
            DEPENDS ${PROJECT_NAME}_doxygen)
    endif()
endif()

add_subdirectory(3rdParty)
add_subdirectory(lib)
if(TSP_BUILD_UNITTESTS)
    add_subdirectory(test)
endif()
