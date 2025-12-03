#include "Computation.h"

int computeSpeedPWM(float distanceCm) {
    const int maxSpeed = 200;
    const float minDist = 10.0;
    const float maxDist = 100.0;

    // Too close, stop
    if (distanceCm <= minDist) {
        return 0;
    }

    // Far enough, full speed
    if (distanceCm >= maxDist) {
        return maxSpeed;
    }
    
    // distanceCm = 10 → speed = 0
    // distanceCm = 100 → speed = 200
    float ratio = (distanceCm - minDist) / (maxDist - minDist);
    int speed = ratio * maxSpeed;

    // safety clamp
    if (speed < 0) speed = 0;
    if (speed > maxSpeed) speed = maxSpeed;

    return speed;
}
