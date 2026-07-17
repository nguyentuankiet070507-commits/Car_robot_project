#include "motor.h"
#include <Arduino.h>

// Define pin  (Only use digitalWrite)
const int IN1 = 12;
const int IN2 = 14;
const int IN3 = 27;
const int IN4 = 26;

// Define pin for PWM speed control
const int ENA = 13;
const int ENB = 25;

// ESP32 Core 2.x only needs to configure 2 PWM channels for 2 EN pins
const int ch_ENA = 0;     
const int ch_ENB = 1;     

const int freq = 2000;      
const int resolution = 8; 
const int MAX_SPEED = 200;  

int speed1 = 0, speed2 = 0; // Speed variables for left and right motors

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ch_ENA, speed1); 
  ledcWrite(ch_ENB, speed2); 
  Serial.println("Up!");
  Serial.println("Speed: " + String(speed1));
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(ch_ENA, speed1); 
  ledcWrite(ch_ENB, speed2); 
  Serial.println("Down!");
  Serial.println("Speed: " + String(speed1));
}

void right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ch_ENA, speed1); 
  ledcWrite(ch_ENB, speed2); 
  Serial.println("Right!");
  Serial.println("Speed: " + String(speed1));
}

void left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(ch_ENA, speed1); 
  ledcWrite(ch_ENB, speed2); 
  Serial.println("Left!");
  Serial.println("Speed: " + String(speed1));
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(ch_ENA, 0); 
  ledcWrite(ch_ENB, 0); 
  Serial.println("Stop!");
}