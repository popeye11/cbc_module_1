set(lib "dds")

add_library(${lib}
    dds.cpp
    sinusLUT.cpp
)

add_library(components::dds ALIAS ${lib})

target_link_libraries(${lib}
    PUBLIC
    rte
    hal
)
target_include_directories(${lib} PUBLIC .)
