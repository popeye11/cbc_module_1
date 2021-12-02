if(NOT TARGET _lib_boost_sml)
    add_library(
        _lib_boost_sml
        INTERFACE
    )

    target_include_directories(
        _lib_boost_sml
        SYSTEM
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/boost.sml/include
    )

    add_library(lib::boost::sml ALIAS _lib_boost_sml)
endif()