cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(bitdoglab_project C CXX ASM)

pico_sdk_init()

add_executable(bitdoglab_project
    main.c
    ws2812.c
)

target_link_libraries(bitdoglab_project pico_stdlib pico_pio)

pico_enable_stdio_usb(bitdoglab_project 1)
pico_enable_stdio_uart(bitdoglab_project 0)

pico_add_extra_outputs(bitdoglab_project)