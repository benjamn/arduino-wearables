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

bool isButtonDepressed() {
  return !digitalRead(inputPin);
}

bool depressed = false;
bool shouldChangeDimness = false;
uint32_t timeOfLastDepressionMs = 0;

void loop() {
  uint32_t now = millis();

  if (isButtonDepressed()) {
    if (!depressed) {
      timeOfLastDepressionMs = now;
      depressed = true;
      shouldChangeDimness = true;
    } else if (now - timeOfLastDepressionMs > 1000) {
      timeOfLastDepressionMs = now;
      modeIndex = (modeIndex + 1) % modeCount;
      shouldChangeDimness = false;
    }
    digitalWrite(0, LOW);
  } else {
    if (depressed && shouldChangeDimness) {
      changeDimness();
    }
    digitalWrite(0, HIGH);
    depressed = false;
    shouldChangeDimness = false;
  }

  animate(modes[modeIndex]);

  meshLoop();
}
