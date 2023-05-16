#include <Arduino.h>
#include "GLWipers.h"

const int wipersRelayOutPin = 4;
const int washerRelayOutPin = 3;
const int wipersInPin = A0;
const int washerInPin = A1;
const int maxWashingTime = 10000;
const int thirdModePause = 5000;

GLWipers wipers;

void setup() {
  Serial.begin(9600);
  wipers.init(wipersInPin, washerInPin, wipersRelayOutPin, washerRelayOutPin, maxWashingTime, thirdModePause);
}

void loop() {
  // put your main code here, to run repeatedly:
  wipers.loop();
  //Проверить, возможно при включении режима первый взмах делается не сразу, а по таймеру режима
  
}