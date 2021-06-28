#include <Arduino.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define SENSORPIN 2

#define DISTANCE_PER_ROTATION 0.2198 //meters
#define TIMEOUT_ROTATION 1000 //ms
#define DEBOUNCING_DELAY 2 //ms

bool sensorState = LOW;
bool sensorReading = LOW;
bool lastSensorState = LOW;

long lastDebounceTime;
long lastDeltaTime;

void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(SENSORPIN, INPUT);

  lastDeltaTime = millis();

  Serial.begin(115200);
}

void loop() {
  sensorReading = digitalRead(SENSORPIN);

  if(sensorReading!= lastSensorState) {
    lastDebounceTime = millis();
  }

  if(millis()- lastDebounceTime > DEBOUNCING_DELAY) {

    if(sensorReading != sensorState) {
      sensorState = sensorReading;
      if(sensorState == LOW) {
        controlLED(getSpeed(millis()-lastDeltaTime));
        lastDeltaTime = millis();
      }
    }
  }

  if(millis()-lastDeltaTime > TIMEOUT_ROTATION) {
    controlLED(getSpeed(millis()-lastDeltaTime));
    lastDeltaTime = millis();
  }

  lastSensorState = sensorReading;
}

float getSpeed(long deltaTime) { // in m/s
  //Serial.println(deltaTime);
  if(deltaTime < TIMEOUT_ROTATION) {
    double speed = DISTANCE_PER_ROTATION / (deltaTime/1000.0);
    //Serial.println(speed);
    return speed;
  } else {
    return (float)0;
  }

}

void controlLED(float speed) {
  //Serial.println(speed*3.6);
  int value = speed*50;
  Serial.println(value);
  if(speed < 25.5) {
    analogWrite(REDPIN, value);
  } else {
    analogWrite(REDPIN, 254);
  }
}