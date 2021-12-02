set(lib "lockin")

add_library(${lib}
    lockIn.cpp
)

add_library(components::lockin ALIAS ${lib})

target_link_libraries(${lib}
    PUBLIC
    rte
    hal
    fir
    dds
)
target_include_directories(${lib} PUBLIC .)
