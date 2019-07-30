#include "Adafruit_WS2801.h"
#include "SPI.h"

#include "common.h"
#include "animate.h"
#include "mesh.h"
#include "modes.h"

void setup() {
  Serial.begin(115200);
  strip.begin();
  clear();
  pinMode(0, OUTPUT);
  pinMode(inputPin, INPUT);
  modeSetup();
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
      changeMode();
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

  modeLoop();
  meshLoop();
}
