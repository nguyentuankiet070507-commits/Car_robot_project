#include <Arduino.h>
#define BLYNK_TEMPLATE_NAME "CAR ROBOT CONTROLLER"
#define BLYNK_DEVICE_NAME "CAR ROBOT CONTROLLER"
#define BLYNK_PRINT Serial
#include "config.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OLED_display.h>
#include <sensor.h>

// Define pin  (Chỉ dùng digitalWrite)
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

bool btn_up = 0;
bool btn_down = 0;
bool btn_left = 0;
bool btn_right = 0;
int speed1 = 0, speed2 = 0;
int speed = 0;
long last = 0, timeout = 10000;
void setup() {
  Serial.begin(115200);
  init_OLED();
  cool_display();
  initSensor();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configure PWM channels for speed control
  ledcSetup(ch_ENA, freq, resolution);
  ledcSetup(ch_ENB, freq, resolution);
  
  ledcAttachPin(ENA, ch_ENA);
  ledcAttachPin(ENB, ch_ENB);
}

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
BLYNK_WRITE(V0) {
  speed2 = param.asInt();
  // Motor speed balance: left motor (128) runs faster than right (89) 
  // This ratio makes them rotate at similar speeds when same PWM applied
  speed1 = (int) constrain(speed2 * 128 / 89, 0, MAX_SPEED);
}
BLYNK_WRITE(V1) {
  btn_up = param.asInt();

  if(btn_up)
    forward();
  else
    stopMotor();
}
BLYNK_WRITE(V2) {
  btn_down = param.asInt();

  if(btn_down)
    backward();
  else
    stopMotor();
}
BLYNK_WRITE(V3) {
  btn_left = param.asInt();

  if(btn_left)
    left();
  else
    stopMotor();
}

BLYNK_WRITE(V4) {
  btn_right = param.asInt();

  if(btn_right)
    right();
  else
    stopMotor();
}
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lose connection!");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
      Serial.print(".");
      loader_display();
    } 
    Serial.println("");
  }

  if (WiFi.status() == WL_CONNECTED && !Blynk.connected()) {
    Serial.println("Try to connect Blynk again!");
    Blynk.connect();
  }
}
void loop() {
  if (millis() - last > 10000) {
    checkConnection();
    last = millis();
  }

  if (Blynk.connected()) {
      Blynk.run();
  }
  else {
      loader_display();
      Blynk.connect(100);   // try to connect to Blynk server with a timeout of 100ms
  }

  short sensor = readSensor();
  if (sensor == 1) {
    danger_display();
  }
  else if (sensor == 2) {
    danger_display();
    stopMotor(); // Stop the motor if an obstacle is too close
  }
  else {
    cool_display();
  }
}
