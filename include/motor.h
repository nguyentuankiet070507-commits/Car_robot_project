#ifndef MOTOR_H
#define MOTOR_H

// Define pin  (Only use digitalWrite)
extern const int IN1;
extern const int IN2;
extern const int IN3;
extern const int IN4;

// Define pin for PWM speed control
extern const int ENA;
extern const int ENB;

// ESP32 Core 2.x only needs to configure 2 PWM channels for 2 EN pins
extern const int ch_ENA;     
extern const int ch_ENB;     

extern const int freq;
extern const int resolution; 
extern const int MAX_SPEED;  

extern int speed1, speed2; // Speed variables for left and right motors

void forward();   // Both motors forward (IN1/IN3 HIGH, IN2/IN4 LOW)
void backward();  // Both motors backward (reverse direction)
void left();      // Left turn (right motor forward, left motor backward)
void right();     // Right turn (left motor forward, right motor backward)
void stopMotor(); // Emergency stop (all pins LOW, PWM = 0)

#endif // MOTOR_H