file(GLOB FILES *.c *.h)
add_library(lcd_os ${FILES})
target_link_libraries(lcd_os pico_stdlib hardware_i2c)