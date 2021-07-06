#include <Arduino.h>

#define REDPIN 6
#define GREENPIN 5
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

float getSpeed(long deltaTime);
void controlLED(float speed);

byte blueLED, redLED, greenLED;

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
  float kmh = speed*3.6;
  Serial.print(kmh);
  if(kmh == 0.0) {
      blueLED = 0;
      greenLED = 0;
      redLED = 0;
  } else if(kmh < 5.0) {
      blueLED = 0;
      greenLED = kmh*51.0;
      redLED = 0;
  } else if(kmh < 10.0){
      greenLED = 255.0-(kmh-5.0)*51.0;
      redLED = 0;
      blueLED = (kmh-5.0)*51.0;
  } else if (kmh < 15.0){
      greenLED = 0;
      redLED = (kmh-10.0)*51.0;
      blueLED = 255.0-(kmh-10.0)*51.0;
  } else {
    blueLED = 0;
    greenLED = 0;
    redLED = 0;
  }
  Serial.print(" ");
  Serial.print(redLED);
  Serial.print(" ");
  Serial.print(blueLED);
  Serial.print(" ");
  Serial.println(greenLED);
  analogWrite(REDPIN, redLED);
  analogWrite(GREENPIN, greenLED);
  analogWrite(BLUEPIN, blueLED);
}

//up to 30 km/h
//blue max 1
//blue max green man -7-
//green max -14-
//greenmax redmax-21-
//redmax -28