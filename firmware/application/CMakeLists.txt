set(lib "application")

add_library(${lib} OBJECT
    application.cpp
    actors.cpp
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
    components::fir
    components::lockin
    components::CtrlLoop
    lib::tsp
)
