#include "Adafruit_WS2801.h"
#include "SPI.h"

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
uint32_t modeCount = sizeof(modes) / sizeof(modes[0]);
uint32_t progModeIndex[1] PROGMEM = { 0 };
uint32_t modeIndex = 0;

uint8_t setModeIndex(uint8_t index) {
  if (index != modeIndex) {
    modeIndex = index;
    memcpy_P(progModeIndex, &modeIndex, sizeof(modeIndex));
  }
  return modeIndex;
}

void modeSetup() {
  memcpy_P(&modeIndex, progModeIndex, sizeof(modeIndex));
  if (modeIndex >= modeCount) setModeIndex(0);
}

uint8_t changeModeIndex() {
  return setModeIndex((modeIndex + 1) % modeCount);
}

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
      changeModeIndex();
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
