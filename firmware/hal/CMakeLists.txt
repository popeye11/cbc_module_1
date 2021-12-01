set(lib "hal")

add_library(${lib} OBJECT
    adc.cpp
    spi.cpp
)

target_link_libraries(${lib}
    rte
)

target_include_directories(${lib} PUBLIC .
../drivers/peripherals)
