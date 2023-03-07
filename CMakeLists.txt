cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico-midi-controller)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_subdirectory(freertos)
add_subdirectory(components/lcd)
add_subdirectory(components/controls)
add_subdirectory(components/mux)
add_subdirectory(components/midi_usb)
if (TARGET tinyusb_device)
    add_executable(main
            main.c
            )

    # pull in common dependencies
    target_link_libraries(main pico_stdlib freertos controls lcd mux midi_usb)


    # enable usb output, disable uart output
    pico_enable_stdio_usb(main 0)
    pico_enable_stdio_uart(main 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(main)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building because TinyUSB submodule is not initialized in the SDK")
endif()