#ifndef HCSR04_H
#define HCSR04_H

const int trigPin = 27;
const int echoPin = 28;

#define SOUND_SPEED 0.034

extern float distanceCm;   // Declare variable so other files can use it

void initHCSR04();         // function to call in setup()
void updateHCSR04();       // function to call in loop()

#endif
