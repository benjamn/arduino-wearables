#include "Adafruit_WS2801.h"
#include "SPI.h"

#include "common.h"
#include "animate.h"
#include "mesh.h"
#include "modes.h"
#include "schemes.h"

void setup() {
  Serial.begin(115200);
  strip.begin();
  clear();
  pinMode(0, OUTPUT);
  pinMode(inputPin, INPUT);
  modeSetup();
  meshSetup();
  colorSchemesSetup();
}

bool isButtonDepressed() {
  return !digitalRead(inputPin);
}

bool depressed = false;
bool shouldChangeBrightness = false;
uint32_t timeOfLastDepressionMs = 0;

void loop() {
  uint32_t now = millis();

  if (isButtonDepressed()) {
    if (!depressed) {
      timeOfLastDepressionMs = now;
      depressed = true;
      shouldChangeBrightness = true;
    } else if (now - timeOfLastDepressionMs > 1000) {
      timeOfLastDepressionMs = now;
      changeMode();
      shouldChangeBrightness = false;
    }
    digitalWrite(0, LOW);
  } else {
    if (depressed &&
        shouldChangeBrightness &&
        now - timeOfLastDepressionMs < 500) {
      changeBrightness();
    }
    digitalWrite(0, HIGH);
    depressed = false;
    shouldChangeBrightness = false;
  }

  animate(modes[modeIndex]);

  modeLoop();
  meshLoop();
}
