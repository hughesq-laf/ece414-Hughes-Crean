cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK
include(pico_sdk_import.cmake)

project(Lab4)

# Initialize the SDK
pico_sdk_init()

add_executable(Lab4)

pico_generate_pio_header(Lab4 ${CMAKE_CURRENT_LIST_DIR}/SPIPIO.pio)

# Add your main executable
target_sources(Lab4 PRIVATE
    Main1.c
    ts_lcd.c
    ts_lcd.h
    glcdfont.c
    TouchScreen.c
    TFTMaster.c
)

target_link_libraries(Lab4 PRIVATE
    pico_stdlib
    hardware_gpio
    pico_rand
    hardware_pio
    hardware_irq
    hardware_adc
    hardware_pwm 
    pico_bootsel_via_double_reset
)


pico_add_extra_outputs(Lab4)
