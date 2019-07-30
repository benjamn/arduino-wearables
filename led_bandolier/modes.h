#pragma once

#include <EEPROM.h>
#define EEPROM_SIZE 64
#define EEPROM_ADDR 0

#include "animate.h"
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

byte modeCount = sizeof(modes) / sizeof(modes[0]);
byte modeIndex = 0;

void modeSetup() {
  EEPROM.begin(EEPROM_SIZE);
  modeIndex = EEPROM.read(EEPROM_ADDR);
  if (modeIndex >= modeCount) {
    modeIndex = 0;
  }
}

bool needToCommit = false;
uint32_t timeOfLastCommit = 0;
uint32_t commitDelayMs = 10000;

void changeMode() {
  modeIndex = (modeIndex + 1) % modeCount;
  EEPROM.write(EEPROM_ADDR, modeIndex);
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
