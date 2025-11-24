cmake_minimum_required(VERSION 3.13)

# Initialize the Pico SDK
include(pico_sdk_import.cmake)

project(hc-sr04)

# Initialize the SDK
pico_sdk_init()

# Create the executable
add_executable(hc-sr04
    hc-sr04.c
)

# Link against the pico_stdlib which includes core functionality
target_link_libraries(hc-sr04 pico_stdlib)

# Enable USB output, so we can use printf
pico_enable_stdio_usb(hc-sr04 1)
pico_enable_stdio_uart(hc-sr04 0)

# Create the Pico binary file
pico_add_extra_outputs(hc-sr04)