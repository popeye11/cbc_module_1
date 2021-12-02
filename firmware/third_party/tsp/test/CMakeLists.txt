cmake_minimum_required(VERSION 3.10)

project(TSP_Library_Unit_Tests CXX)

enable_testing()

add_link_options("--coverage")

find_package(
    Boost
    COMPONENTS
        system
        timer
        unit_test_framework
    REQUIRED
)

set(
    TESTS
    test_data.cpp
    container/test_static_vector.cpp
    tsp/test_quadratic_fit.cpp
    tsp/test_ransac.cpp
    tsp/test_iir.cpp
    tsp/test_pid.cpp
    tsp/test_util.cpp
    misc/test_misc.cpp
)

add_executable(
    ${PROJECT_NAME}
    test_main.cpp
    ${TESTS}
)

# indicates the shared library variant
target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    "BOOST_TEST_DYN_LINK=1"
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -fprofile-arcs
    -ftest-coverage
    -fno-omit-frame-pointer
    -fno-inline
    -fno-optimize-sibling-calls
)

target_link_libraries(
    ${PROJECT_NAME}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    lib::tsp
    gcov
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_BINARY_DIR}/generated
    .
)

find_program(
    CLANG_TIDY
    # Order is important here.  On Ubuntu "clang-tidy" (without any version
    # suffix) usually has an older version, so we look for newer versions first.
    NAMES clang-tidy-11 clang-tidy-10 clang-tidy-9 clang-tidy
    REQUIRED
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY CXX_CLANG_TIDY
    ${CLANG_TIDY}
    --extra-arg=-std=c++17
    --extra-arg=-stdlib=libstdc++
    --extra-arg=-Wno-unknown-warning-option
    ## this regular expression indicates which header-files should be checked.
    --header-filter=.*
    "--checks=boost-*,\
              bugprone-*,\
              clang-analyzer-*,\
              clang-diagnostic-*,\
             -clang-diagnostic-c++98-c++11-compat,\
             -clang-diagnostic-c++98-c++11-c++14-compat,\
              cppcoreguidelines-*,\
             -cppcoreguidelines-avoid-magic-numbers,\
             -cppcoreguidelines-pro-type-vararg,\
             -cppcoreguidelines-non-private-member-variables-in-classes,\
              misc-*,\
             -misc-non-private-member-variables-in-classes,\
              modernize-*,\
             -modernize-use-nodiscard,\
             -modernize-use-trailing-return-type,\
              performance-*,\
              portability-*,\
              readability-*,\
             -readability-magic-numbers"
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME} --log_format=JUNIT --log_sink=${PROJECT_NAME}.xml --log_level=all --report_level=no
)

add_custom_target(
    ${PROJECT_NAME}_cov_report
    ${CMAKE_COMMAND} -E make_directory coverage_report
    COMMAND gcovr
        --root ${CMAKE_SOURCE_DIR}
        --object-directory ${CMAKE_CURRENT_BINARY_DIR}
        --exclude ${CMAKE_SOURCE_DIR}/test
        --exclude ${CMAKE_SOURCE_DIR}/3rdParty
        --html
        --html-details
        --output coverage_report/index.html
)
