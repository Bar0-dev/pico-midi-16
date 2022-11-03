file(GLOB FILES *.c *.h)
add_library(midi_usb ${FILES})

# Make sure TinyUSB can find tusb_config.h
target_include_directories(midi_usb PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(midi_usb PUBLIC pico_stdlib tinyusb_device tinyusb_board)