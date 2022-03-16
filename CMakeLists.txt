cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_SDK_PATH "/home/pi/C/pico/pico-sdk")
include(pico_sdk_import.cmake)

project(blinken C CXX ASM)
pico_sdk_init()
add_executable(blinken blinken.c )

pico_set_program_name(blinken "blinken")
pico_set_program_version(blinken "0.1")

pico_enable_stdio_uart(blinken 1)
pico_enable_stdio_usb(blinken 0)

target_link_libraries(blinken pico_stdlib)
target_link_libraries(blinken hardware_i2c)

pico_add_extra_outputs(blinken)