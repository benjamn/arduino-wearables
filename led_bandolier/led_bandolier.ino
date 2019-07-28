#include "Adafruit_WS2801.h"
#include "SPI.h"
#include <EEPROM.h>

#include "common.h"
#include "animate.h"
#include "mesh.h"
#include "rainbows.h"
#include "chasers.h"
#include "red.h"
#include "green.h"
#include "blue.h"

void (*modes[])(AnimationState*, byte[3]) = {
  rainbows,
  chasers,
  solidRed,
  solidGreen,
  solidBlue,
};
size_t modeCount = sizeof(modes) / sizeof(modes[0]);
size_t modeIndex = 0;

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
    animate(modes[modeIndex]);
    wasPressed = false;
  }

  meshLoop();
}
