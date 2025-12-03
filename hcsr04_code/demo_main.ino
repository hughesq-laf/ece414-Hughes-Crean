#include "hcsr04.h"

void setup() {
    Serial.begin(115200);
    initHCSR04();            // initialize pins
}

void loop() {
    updateHCSR04();          // refresh the distance
    Serial.println(distanceCm);
    delay(500);
}
