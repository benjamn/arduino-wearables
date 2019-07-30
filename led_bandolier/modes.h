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

typedef struct {
  // Maximum of 4 colors, each is 3 bytes rgb
  byte colors[4][3];
  byte numColors;
} ColorScheme;


void setScheme(byte colors[][3], ColorScheme& scheme) {
  int numColors = sizeof(colors) / sizeof(byte[3]);
  scheme.numColors = numColors;
  for (int i = 0; i < numColors; i++) {
    for (int j = 0; j < 3; j++) {
      scheme.colors[i][j] = colors[i][j];
    }
  }
}

void (*modes[])(AnimationState*, byte[3]) = {
  rainbows,
  chasers,
  solidRed,
  solidGreen,
  solidBlue,
};

// Parallel array to the above
bool modeSupportsSchemes[] = {
  false,
  true,
  false,
  false,
  false
};

byte modeCount = sizeof(modes) / sizeof(modes[0]);
byte modeIndex = 0;

// Unfortunately these schemes are initialized at runtime in modeSetup() because
// "nontrivial initializers are not implemented".
ColorScheme schemes[3];
byte schemeCount = sizeof(schemes) / sizeof(schemes[0]);
byte schemeIndex = 0;
byte jointCount = schemeCount * modeCount;

void colorSchemesSetup() {
  // Initialize the color schemes
    
  // Scheme 0: red, green, and blue
  byte scheme0[][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
  setScheme(scheme0, schemes[0]);

  // Scheme 1: purple and pink
  byte scheme1[][3] = {{215, 0, 130}, {110, 0, 210}};
  setScheme(scheme1, schemes[1]);

  // Scheme 2: orange-red and light blue
  byte scheme2[][3] = {{255, 84, 10}, {10, 190, 255}};
  setScheme(scheme2, schemes[2]);
}

ColorScheme& currentScheme() {
  return schemes[schemeIndex];
}

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
