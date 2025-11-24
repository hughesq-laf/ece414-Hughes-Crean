#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"


// Hardware pin definitions
#define TRIG_PIN 27           /**< Trigger pin of the ultrasonic sensor */
#define ECHO_PIN 28           /**< Echo pin of the ultrasonic sensor */

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

/**
 * @brief Controls the TRIG pin of the ultrasonic sensor.
 * 
 * This function sets the TRIG pin to either HIGH or LOW, 
 * enabling the sensor to send trigger pulses.
 * #include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#define TRIG_PIN 27
#define ECHO_PIN 28
#define SOUND_SPEED 0.034f // cm per microsecond
#define CM_TO_INCH 0.393701f

int main() {
    stdio_init_all(); // Initialize serial communication

    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    while (true) {
        // Send a 10us HIGH pulse to TRIG_PIN
        gpio_put(TRIG_PIN, 0);
        sleep_us(2);
        gpio_put(TRIG_PIN, 1);
        sleep_us(10);
        gpio_put(TRIG_PIN, 0);

        // Wait for the echo pin to go high (start of pulse)
        while (gpio_get(ECHO_PIN) == 0) {
            tight_loop_contents();
        }
        absolute_time_t start_time = get_absolute_time();

        // Wait for the echo pin to go low (end of pulse)
        while (gpio_get(ECHO_PIN) == 1) {
            tight_loop_contents();
        }
        absolute_time_t end_time = get_absolute_time();

        // Calculate pulse duration in microseconds
        int64_t duration_us = absolute_time_diff_us(start_time, end_time);

        // Calculate distance
        float distance_cm = duration_us * SOUND_SPEED / 2.0f;
        float distance_inch = distance_cm * CM_TO_INCH;

        // Print results
        printf("Distance (cm): %.2f\n", distance_cm);
        printf("Distance (inch): %.2f\n\n", distance_inch);

        sleep_ms(1000); // Wait for 1 second
    }
    return 0;
}
 * @param state The state to be set for the pin (HIGH or LOW).
 */
void write_trig_pin(bool state) {
    gpio_put(TRIG_PIN, state);  /**< Sends the state to the TRIG pin */
}


/**
 * @brief Reads the distance measured by the ultrasonic sensor.
 * 
 * This function emits a trigger pulse, waits for the echo signal, 
 * and calculates the distance based on the time taken for the echo 
 * to return, using the speed of sound formula. A timeout mechanism 
 * is included to prevent infinite loops in case of no echo detection.
 * 
 * @return The measured distance in centimeters. Returns -1.0 if no 
 * valid measurement is obtained (timeout or out-of-range reading).
 */
float get_reading_sensor() {
    const uint32_t timeout = 38000; /**< Timeout threshold to prevent infinite loops (~65ms -> ~4m max range) */

    // Trigger pulse to initiate measurement
    write_trig_pin(0);
    sleep_us(2);
    write_trig_pin(1);
    sleep_us(10);
    write_trig_pin(0);

    // Wait for the echo signal to start, with timeout
    uint32_t start_time = time_us_32();
    while (!gpio_get(ECHO_PIN)) {
        if ((time_us_32() - start_time) > timeout) {
            return -1.0;  /**< Return -1 to indicate timeout (no object detected) */
        }
    }

    // Capture the start time
    absolute_time_t start = get_absolute_time();

    // Wait for the echo signal to end, with timeout
    start_time = time_us_32();
    while (gpio_get(ECHO_PIN)) {
        if ((time_us_32() - start_time) > timeout) {
            return -1.0;  /**< Return -1 to indicate timeout */
        }
    }

    // Capture the end time
    absolute_time_t end = get_absolute_time();

    // Compute the duration of the echo pulse
    int64_t duration = absolute_time_diff_us(start, end);

    // Convert duration to distance in cm
    float distance = duration / 58.0;

    // Optional: Ignore unrealistic readings (e.g., too close or too far)
    if (distance < 1.0 || distance > 400.0) {
        return -1.0;  /**< Invalid reading */
    }

    return distance;
}


/**
 * @brief Main function to initialize the Pico and run the ultrasonic sensor.
 * 
 * This function initializes the GPIO pins, configures the TRIG pin as an output 
 * and the ECHO pin as an input, and then enters an infinite loop to continuously 
 * read and display the measured distance.
 * 
 * @return This function does not return (infinite loop).
 */
int main() {
    stdio_init_all();
    sleep_ms(200); /**< Ensures enough time for the serial interface to initialize */

    // Initialize GPIO pins
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    printf("Ultrasonic Sensor HC-SR04\n");

    while (true) {
        float distance = get_reading_sensor();  /**< Retrieves the distance measurement from the sensor */
        printf("Distance: %.2f cm\n", distance);  /**< Displays the measured distance */
        sleep_ms(1000);  /**< Waits 1 second before the next reading */
    }
}