set(lib "application")

add_library(${lib} OBJECT
    application.cpp
)

target_include_directories(${lib} PRIVATE
    ../support
    third_party/lib/include/tsp

)

target_link_libraries(${lib}
    PUBLIC
    rte
    hal
    Drivers::peripherals
    components::dds
    lib::tsp
)
