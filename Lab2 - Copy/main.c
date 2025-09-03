#include "pico/stdlib.h"
#include "sw_in.h"
#include "led_out.h"

int main() {
    bool switch1_state, switch2_state;
    uint8_t led_pattern;

    sw_in_init();
    led_out_init();

    while(true){
        switch1_state = sw_in_read1();
        switch2_state = sw_in_read2();

        if(switch1_state && switch2_state){
            led_pattern = 0b11111111;
        }
        else if(switch1_state && !switch2_state){
            led_pattern = 0b10000000;
        }
        else if(switch2_state && !switch1_state){
            led_pattern = 0b00000001;
        }
        else {
            led_pattern = 0x00;
        }

        led_out_write(led_pattern);
        sleep_ms(10);
    }
}