#pragma once

#include "animate.h"
#include "modes.h"
#include "schemes.h"

// Animate some colors that blend into each other. Each color will
// have a few bright pixels in the middle and a dimmer halo around it.
void blended(AnimationState *state, byte rgb[3]) {
  ColorScheme& scheme = currentScheme();
  byte numColors = scheme.numColors;
  
  uint32_t colorPeriodMs = 1000;
  int offsetInsidePeriod = state->timeMs % colorPeriodMs;
  byte weight = offsetInsidePeriod * 255 / colorPeriodMs; 

  byte colorIndex = (state->timeMs / colorPeriodMs) % numColors;

  uint32_t time_s = state->timeMs / 1000;
  byte* currentColor = scheme.colors[colorIndex];
  byte* nextColor = scheme.colors[(colorIndex + 1) % numColors];

  blend(currentColor, nextColor, weight, rgb);
}
