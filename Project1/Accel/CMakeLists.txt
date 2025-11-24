cmake_minimum_required(VERSION 3.13)

# Initialize the Pico SDK
include(pico_sdk_import.cmake)

project(Accel)

# Initialize the SDK
pico_sdk_init()

# Adds a CMakeLists.txt file from a subdirectory
add_subdirectory(rpi-pico-mpu6050)

# Create the executable
add_executable(Accel
    Accel.c
)

# Link against the pico_stdlib which includes core functionality
target_link_libraries(Accel 
    pico_stdlib
    hardware_uart
    hardware_i2c
    haw_mpu6050)

# Enable USB output, so we can use printf
pico_enable_stdio_usb(Accel 1)
pico_enable_stdio_uart(Accel 0)


# Create the Pico binary file
pico_add_extra_outputs(Accel)