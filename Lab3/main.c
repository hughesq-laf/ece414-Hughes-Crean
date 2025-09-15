/* ECE414 - Lab 3 - main.c file Template.
Ideally, in your Lab3 project folder, you should have the following files to start with:
	- CMakeLists.txt
	- Lab3.c (or main.c)
	- led_out.c
	- led_out.h
	- sw_in.c
	- sw_in.h
	- debounce_sw1.c
	- debounce_sw1.h
	- debounce_sw2.c
	- debounce_sw2.h
	- timer.c
	- timer.h
	- uart.c
	- uart.h
	(The UART functionality can be embedded in the main file (#include "hardware/uart.h"), which is acceptable since it's just initialization and printf statements. Separate uart.c/uart.h files would be good practice for modularity, but isn't strictly necessary for this lab.)
	- gpio.c (to test pico's GPIO functionality if needed)
*/

#include "pico/stdlib.h"
#include "sw_in.h"
#include <stdint.h>
#include <stdlib.h>
#include "led_out.h"
#include <stdbool.h>
#include "timer.h"
#include "debounce_sw1.h"
#include "debounce_sw2.h"
#include "hardware/uart.h"
#include <stdio.h>

#define UART_ID uart0
#define BAUD_RATE 115200

// UART pin definitions
#define UART_TX_PIN 0
#define UART_RX_PIN 1
int scoreL;
int scoreR;

// FSM States - TODO: Define your game states
enum PONG_States
{
    // TODO: Add your states here (e.g., Init, Serve, Travel, etc.)
    Init,
    Serve,
    Travel,
    Thwack,
    Victory,
    Miss,
    // Add more states...
} PONG_State;

// Player enum - TODO: Define which player is currently active
enum currentPlayer
{
    // TODO: Define PlayerL and PlayerR
    PlayerL,
    PlayerR,
} currentPlayer;


// Global variables
uint8_t ledsStates = 0;        // 8-bit value for LED control
uint8_t ledsStatesCounter = 0; // Counter for multi-tick actions
uint32_t t1, t2, t3, t4;       // Timers for debounce and game timing
uint32_t gameTimer = 350;      // Game speed timer (starts at 300ms)
uint8_t numberOfRoundsPlayed = 0; // Track rounds for speed increase
uint32_t randValue;
uint32_t travelTimer = 0; // global, timer for travel state
uint32_t flashTime = 0;
uint32_t flashCount = 0;

// Main game state machine tick function
void tick()
{
    switch (PONG_State)
    {
    case Init:

        if(rand()%2 == 1) {
            currentPlayer = PlayerL;
            ledsStates = 0x80;
            printf("L Serving");
        }
        else{
            currentPlayer = PlayerR;
            ledsStates = 0x01;
            printf("R Serving");
        }
        led_out_write(ledsStates);    
        PONG_State = Serve;
    // TODO: Initialize the game
        // Hints:
        // - Use rand() % 2 to randomly choose starting player
        // - Set appropriate LED (0x80 for left, 0x01 for right)
        // - Print serve message to UART
        // - Transition to Serve state
        // - Reset counters
        
        break;
        
    case Serve:
        if(currentPlayer == PlayerR && debounce_sw2_pressed()) {
            PONG_State = Travel;
            currentPlayer = PlayerL;
        }
        else if (currentPlayer == PlayerL && debounce_sw1_pressed()) {
            PONG_State = Travel;
            currentPlayer = PlayerR;
        }
        else {
            PONG_State = Serve;
        }
        break;
    // TODO: Add case for Serve state
    // Hints:
    // - Wait for serving player to press their button
    // - When pressed, switch to other player and go to Travel state
    case Travel:

   

        if(currentPlayer == PlayerL)
        {

            if(timer_elapsed_ms(travelTimer, timer_read()) >= gameTimer) 
            {
                if(ledsStates == 0x80)
                {
                    PONG_State = Victory;
                }
                else
                {
                ledsStates = ledsStates<<1;
                led_out_write(ledsStates);
                travelTimer = timer_read(); 
                }
            }
            if(debounce_sw1_pressed()) {
                    PONG_State = Thwack;
                }

                else
                {
                    PONG_State = Travel;
                }
        }

         else if(currentPlayer == PlayerR)
        {

            if(timer_elapsed_ms(travelTimer, timer_read()) >= gameTimer) 
            {
                 if(ledsStates == 0x01)
                {
                    PONG_State = Victory;
                }
                else
                {
               
                ledsStates = ledsStates>>1;
                led_out_write(ledsStates);
                travelTimer = timer_read(); 
                }
            }
            if(debounce_sw2_pressed()) {

                    PONG_State = Thwack;
                }
                else
                {
                    PONG_State = Travel;
                }
        }

        // TODO: Add case for Travel state  
    // Hints:
    // - Move LED toward current player's side
    // - Use bit shifting (<<1 or >>1) to move the LED
    // - Check if ball reached the end (counter >= 7)
    // - If player presses early, they lose (go to Victory)
    // - If ball reaches end, go to Thwack state

        break;


        case Thwack:
        
        if(currentPlayer == PlayerL && ledsStates == 0x80)
        {
            currentPlayer = PlayerR;
            PONG_State = Travel;
        }
        else if(currentPlayer == PlayerR && ledsStates == 0x01)
        {
            currentPlayer = PlayerL;
            PONG_State = Travel; 
        }
        else if(currentPlayer == PlayerR && ledsStates != 0x01)
        {
            PONG_State = Victory; 
        }
        else if(currentPlayer == PlayerL && ledsStates != 0x80)
        {
            PONG_State = Victory;
        }
        break; 
    // TODO: Add case for Thwack state
    // Hints:
    // - Player must hit the ball when it's at their end
    // - If they hit it, switch players and go to Travel
    // - If they miss, go to Victory state

        case Victory:

    if (timer_elapsed_ms(flashTime, timer_read()) >= 200) {
        flashTime = timer_read();
        ledsStates ^= (currentPlayer == PlayerL) ? 0x01 : 0x80;
        led_out_write(ledsStates);
        flashCount++;

       if (flashCount >= 6) {
    flashCount = 0;
    gameTimer = (numberOfRoundsPlayed >= 5) ? 100 :
                (numberOfRoundsPlayed >= 3) ? 200 : 300;
    numberOfRoundsPlayed++;

  if (currentPlayer == PlayerL) {
        scoreL -= 0;    // Left lost
        scoreR += 1;    // Right gains
        printf("Left player lost\n");
    } else {
        scoreR -= 0;    // Right lost
        scoreL += 1;    // Left gains
        printf("Right player lost\n");
    }

    ledsStates = 0;
    led_out_write(ledsStates);
    PONG_State = Init;
}

   break;

    // TODO: Add case for Victory state
    // Hints:
    // - Flash the losing player's LED
    // - Use ledsStatesCounter to control flash timing
    // - Print loss message to UART
    // - After flashing, go back to Init
    // - Update numberOfRoundsPlayed and adjust gameTimer speed

    default:
        break;
    }
}

// Initialization function - TODO: Complete the initialization
void initializeStuff()
{
     scoreL = 0;
     scoreR = 0;
    // TODO: Initialize all required modules
    // Hints:
    // - stdio_init_all() for UART
    // - uart_init() with proper parameters
    // - Set GPIO functions for UART pins
    // - Initialize LED and switch modules
    // - Initialize debounce modules
    // - Read initial timer values
uart_init(UART_ID, BAUD_RATE);
gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
stdio_init_all(); 
sw_in_init();
debounce_sw1_init();
debounce_sw2_init();
led_out_init();

}

int main()
{
    initializeStuff();
    
    // TODO: Set initial state for the state machine
    PONG_State = Init;
    int Deb_Time = timer_read();

    while (1)
    {

        
        // Button Debounce Code - TODO: Implement debounce timing
        // Hints:
        // - Read current time with timer_read()
        // - Check if DEBOUNCE_PD_MS has elapsed
        // - If so, call debounce tick functions

  

        if(timer_elapsed_ms(Deb_Time, timer_read()) >= DEBOUNCE_PD_MS)
        {
            debounce_sw1_tick();
            debounce_sw2_tick();
            Deb_Time = timer_read();
        }
        
        // Game State Machine Timing - TODO: Implement game timing
        // Hints:
        // - Read current time
        // - Check if gameTimer milliseconds have elapsed
        // - If so, call tick() function
        // - Update timer reference

            tick();
        
    }
}
}

/*
 * IMPLEMENTATION HINTS AND REQUIREMENTS:
 * 
 * 1. State Machine Design:
 *    - Init: Random player assignment and setup
 *    - Serve: Wait for serving player button press
 *    - Travel: Move ball across LEDs
 *    - Thwack: Handle ball hitting at player's end
 *    - Victory: Flash winner's LED and restart
 * 
 * 2. LED Control:
 *    - Use bit patterns: 0x80 = leftmost, 0x01 = rightmost
 *    - Use bit shifting to move the ball: <<1 (left), >>1 (right)
 *    - led_out_write() outputs the pattern to LEDs
 * 
 * 3. Timing Requirements:
 *    - Initial game speed: 300ms
 *    - After 3 rounds: 200ms  
 *    - After 5 rounds: 100ms
 *    - Debounce timing: DEBOUNCE_PD_MS (25ms)
 * 
 * 4. Player Logic:
 *    - PlayerL uses debounce_sw1_pressed()
 *    - PlayerR uses debounce_sw2_pressed()
 *    - Early button press = lose point
 *    - Missing the ball at end = lose point
 * 
 * 5. UART Messages:
 *    - "Left Player Serves\n" or "Right Player Serves\n"
 *    - "Left player lost\n" or "Right player lost\n"
 *    - Remove all debug printf statements in final code
 * 
 * 6. Victory Animation:
 *    - Flash the WINNING side's LED at least 3 times (per requirement G5)
 *    - Use ledsStatesCounter to control timing
 *    - Alternate between LED on and off states
 */
 
 // Template prepared by Prof. Priyank Kalgaonkar.