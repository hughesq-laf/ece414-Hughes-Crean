// File: ts_test.c - Code Skeleton
// Prepared by Prof. Priyank Kalgaonkar

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "stdbool.h"
#include "inttypes.h"
#include "TouchScreen.h"
#include "TFTMaster.h"
#include "ts_lcd.h"

// TODO: Define buffers for storing coordinate strings
/*
#define	ILI9340_BLACK   0x0000
#define	ILI9340_BLUE    0x001F
#define	ILI9340_RED     0xF800
#define	ILI9340_GREEN   0x07E0
#define ILI9340_CYAN    0x07FF
#define ILI9340_MAGENTA 0xF81F
#define ILI9340_YELLOW  0xFFE0
#define ILI9340_WHITE   0xFFFF
*/
static char buffer1[30], buffer2[30];
// TODO: Define state tracking variables
static bool screenTouched, firstscreenTouched;
static uint16_t last_lcdx, last_lcdy; // Store last touch coordinates

void writeRAWCoor(struct TSPoint p) {
    // TODO: Set cursor position for raw coordinates display
    tft_setCursor(0, 0);

    // Set text color for raw coordinates
    tft_setTextColor(ILI9340_WHITE);

    // Format and display raw coordinates
    sprintf(buffer1, "x: %d, y: %d, z: %d", p.x, p.y, p.z);
    tft_writeString(buffer1);
}

void writeLCDCoor(uint16_t xcoor, uint16_t ycoor) {
    // Set cursor position for LCD coordinates display
    tft_setCursor(50, 50);

    // Set text color for LCD coordinates
    tft_setTextColor(ILI9340_WHITE);

    // Format and display LCD coordinates
    sprintf(buffer2, "x: %d, y: %d", xcoor, ycoor);
    tft_writeString(buffer2);
}

void drawCrossHair(uint16_t xcoor, uint16_t ycoor) {
    // Draw a circle at the touch point (10 pixel diameter)
    tft_drawCircle(xcoor, ycoor, 5, ILI9340_MAGENTA);

    // Draw horizontal line through center
    tft_drawLine(xcoor - 5, ycoor, xcoor + 5, ycoor, ILI9340_MAGENTA);

    // Draw vertical line through center
    tft_drawLine(xcoor, ycoor - 5, xcoor, ycoor + 5, ILI9340_MAGENTA);

    // Draw a center pixel for better visibility
    tft_drawPixel(xcoor, ycoor, ILI9340_RED);
}

void deleteCrossHair(uint16_t xcoor, uint16_t ycoor) {
    // Erase the crosshair by drawing over it with background color
    tft_drawCircle(xcoor, ycoor, 5, ILI9340_BLACK);
    tft_drawLine(xcoor - 5, ycoor, xcoor + 5, ycoor, ILI9340_BLACK);
    tft_drawLine(xcoor, ycoor - 5, xcoor, ycoor + 5, ILI9340_BLACK);
}

void clearScreen(uint16_t lastx, uint16_t lasty) {
    // Clear previous LCD/RAW text by overwriting with background color
    tft_setCursor(0, 0);
    tft_setTextColor(ILI9340_BLACK);
    tft_writeString(buffer1);

    tft_setCursor(50, 50);
    tft_writeString(buffer2);

    // Erase the previous crosshair
    deleteCrossHair(lastx, lasty);
}

int main() {
    // Initialize the touchscreen and LCD system
    ts_lcd_init();

    // Initialize the first touch flag to false
    firstscreenTouched = false;

    while (1) {
        // Clear previous frame's display elements
        clearScreen(last_lcdx, last_lcdy);

        struct TSPoint p;
        getPoint(&p);
        writeRAWCoor(p);

        uint16_t lcdx, lcdy;
        screenTouched = get_ts_lcd(&lcdx, &lcdy);

        if (screenTouched) {
            firstscreenTouched = true;
            last_lcdx = lcdx;
            last_lcdy = lcdy;
        }

        // Display last coordinates and crosshair if screen has ever been touched
        if (firstscreenTouched) {
            writeLCDCoor(last_lcdx, last_lcdy);
            drawCrossHair(last_lcdx, last_lcdy);
        }

        // Small delay to prevent flickering
        sleep_ms(100);
    }
}
