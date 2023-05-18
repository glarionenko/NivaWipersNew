#include <Arduino.h>
#include "GLWipers.h"
#include "mcp2515.h"

const int mcpCSpin = 10;
struct can_frame canMsg;

const int wipersRelayOutPin = 4;
const int washerRelayOutPin = 3;
const int wipersInPin = A0;
const int washerInPin = A1;
const int maxWashingTime = 10000;
const int thirdModePause = 5000;

GLWipers wipers;
MCP2515 mcp2515(mcpCSpin);

void setup() {
  Serial.begin(9600);

  wipers.init(wipersInPin, washerInPin, wipersRelayOutPin, washerRelayOutPin, maxWashingTime, thirdModePause);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  // put your main code here, to run repeatedly:
  wipers.loop();
  //Проверить, возможно при включении режима первый взмах делается не сразу, а по таймеру режима
  
}