set(lib "CtrlLoop")

add_library(${lib}
    ctrlLoop.cpp
)

add_library(components::CtrlLoop ALIAS ${lib})

target_link_libraries(${lib}
    PUBLIC
    rte
    hal
    dds
    fir
    lockin
    lib::tsp
)
target_include_directories(${lib} PUBLIC .)
