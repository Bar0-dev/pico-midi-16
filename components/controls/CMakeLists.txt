file(GLOB FILES *.c *.h)
add_library(controls ${FILES})
target_link_libraries(controls mux midi_usb lcd_os pico_stdlib)