#include <Arduino.h>
#define BLYNK_TEMPLATE_NAME "CAR ROBOT CONTROLLER"
#define BLYNK_DEVICE_NAME "CAR ROBOT CONTROLLER"
#define BLYNK_PRINT Serial
#include "config.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OLED_display.h>
#include <sensor.h>
#include <motor.h>

bool btn_up = 0;
bool btn_down = 0;
bool btn_left = 0;
bool btn_right = 0;
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
