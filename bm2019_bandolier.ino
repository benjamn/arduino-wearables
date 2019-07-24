#include "Adafruit_WS2801.h"
#include "SPI.h"
#include <EEPROM.h>

#include "common.h"
#include "animate.h"
#include "mesh.h"
#include "utils.h"
#include "rainbows.h"

void setup() {
  Serial.begin(115200);
  strip.begin();
  clear();
  pinMode(0, OUTPUT);
  pinMode(inputPin, INPUT);
  meshSetup();
}

bool isButtonPressed() {
  return !digitalRead(inputPin);
}

bool wasPressed = false;

void loop() {
  if (isButtonPressed()) {
    digitalWrite(0, LOW);
    clear();
    wasPressed = true;
  } else {
    if (wasPressed) {
      changeDimness();
    }
    digitalWrite(0, HIGH);
    animate(rainbows);
    wasPressed = false;
  }

  meshLoop();
}

