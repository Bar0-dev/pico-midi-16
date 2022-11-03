file(GLOB FILES *.c *.h)
add_library(lcd ${FILES})
target_link_libraries(lcd pico_stdlib hardware_i2c)
target_include_directories(lcd PUBLIC ../include/)