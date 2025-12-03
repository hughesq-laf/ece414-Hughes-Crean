#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

// Returns a PWM speed (0–200) based on measured distance (cm)
int computeSpeedPWM(float distanceCm);

#endif
