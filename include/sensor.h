#ifndef SENSOR_H
#define SENSOR_H
#include <Arduino.h>
// Declare pin connections
extern const int trigPin;
extern const int echoPin;

// Define the speed of sound in air (in cm/us)
#define SOUND_SPEED 0.034

extern long duration;
extern float distanceCm;

void initSensor();

short readSensor();
#endif // SENSOR_H