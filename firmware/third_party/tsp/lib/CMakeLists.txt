if(NOT TARGET _lib_tsp)
    add_library(
        _lib_tsp
        INTERFACE
    )

    target_link_libraries(
        _lib_tsp
        INTERFACE
            lib::boost::sml
    )

    target_include_directories(
        _lib_tsp
        INTERFACE
            include
    )

    add_library(lib::tsp ALIAS _lib_tsp)
endif()