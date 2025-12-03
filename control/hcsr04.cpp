#include "hcsr04.h"
#include <Arduino.h>

float distanceCm;

void initHCSR04() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void updateHCSR04() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED / 2;  
}
