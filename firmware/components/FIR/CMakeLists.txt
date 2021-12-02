set(lib "fir")

add_library(${lib}
    fir.cpp
)

add_library(components::fir ALIAS ${lib})

target_link_libraries(${lib}
    PUBLIC
    rte
    hal
)
target_include_directories(${lib} PUBLIC .)
