file(GLOB FILES *.c *.h)
add_library(mux ${FILES})
target_link_libraries(mux pico_stdlib hardware_adc hardware_gpio hardware_i2c)