file(GLOB FILES *.c *.h)
add_library(pico-mux ${FILES})
target_link_libraries(pico-mux pico_stdlib hardware_adc hardware_gpio)