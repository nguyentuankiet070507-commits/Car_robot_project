#include <sensor.h>     

const int trigPin = 5;
const int echoPin = 18;

long duration;
float distanceCm;

void initSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

short readSensor() {
  // Emit ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the time of the reflected wave (in microseconds)
  duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30 ms to avoid blocking if no echo is received
  
  // Calculate distance in centimeters
  distanceCm = duration * SOUND_SPEED / 2;
  
  // Print results to Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  
  if (distanceCm < 30 && distanceCm > 10) { // Check if the distance is less than 30 cm and greater than 10 cm
    Serial.println("Obstacle detected!.");
    return 1; // Return 1 to indicate that an obstacle is detected
  }
  else if (distanceCm <= 10) { // Check if the distance is less than or equal to 10 cm
    Serial.println("Obstacle too close!.");
    return 2; // Return 2 to indicate that an obstacle is too close
  }
  else {
    Serial.println("No obstacle detected.");
  }

  return 0; // Return 0 to indicate that the reading is complete
}
