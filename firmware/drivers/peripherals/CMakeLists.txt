set(lib "peripherals")

add_library(${lib}
    peripherals.cpp
)

add_library(Drivers::peripherals ALIAS ${lib})

target_link_libraries(${lib}
    PUBLIC
    rte
    hal
)
target_include_directories(${lib} PUBLIC .)
