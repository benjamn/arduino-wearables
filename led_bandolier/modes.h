#pragma once

#include <EEPROM.h>
#define EEPROM_SIZE 64
#define EEPROM_ADDR 0

#include "animate.h"
#include "rainbows.h"
#include "blended.h"
#include "chasers.h"
#include "red.h"
#include "green.h"
#include "blue.h"
#include "schemes.h"

void (*modes[])(AnimationState*, byte[3]) = {
  rainbows,
  chasers,
  blended,
  solidRed,
  solidGreen,
  solidBlue,
};

// Parallel array to the above
bool modeSupportsSchemes[] = {
  false,
  true,
  true,
  false,
  false,
  false
};

byte modeCount = sizeof(modes) / sizeof(modes[0]);
byte modeIndex = 0;
byte jointCount = schemeCount * modeCount;

void modeSetup() {
  EEPROM.begin(EEPROM_SIZE);
  byte jointIndex = EEPROM.read(EEPROM_ADDR);
  if (jointIndex >= jointCount) {
    jointIndex = 0;
  }
  modeIndex = jointIndex / modeCount;
  schemeIndex = jointIndex % modeCount;
}

bool needToCommit = false;
uint32_t timeOfLastCommit = 0;
uint32_t commitDelayMs = 10000;

void changeMode() {
  // Within one mode, iterate through color schemes.
  // If out of color schemes, move onto the next mode.
  if (modeSupportsSchemes[modeIndex]) {
    schemeIndex = (schemeIndex + 1) % schemeCount;
    if (schemeIndex == 0) {
      modeIndex = (modeIndex + 1) % modeCount;
    }
  } else {
    // If the mode doesn't support schemes, just move onto the next mode
    modeIndex = (modeIndex + 1) % modeCount;
  }

  byte jointIndex = modeIndex * schemeCount + schemeIndex;
  EEPROM.write(EEPROM_ADDR, jointIndex);
  needToCommit = true;
}

void modeLoop() {
  if (needToCommit) {
    uint32_t now = millis();
    if (now - timeOfLastCommit > commitDelayMs) {
      needToCommit = false;
      timeOfLastCommit = now;
      EEPROM.commit();
    }
  }
}
