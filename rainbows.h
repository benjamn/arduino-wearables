#pragma once

#include "animate.h"

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  WheelPos -= 170;
  return Color(0, WheelPos * 3, 255 - WheelPos * 3);
}

uint32_t rainbows(AnimationState *state) {
  // For simplicity, we assume every bandolier has the same number of pixels (50).
  size_t allLedCount = state->ledCount * state->meshCount;
  size_t allLedIndex = state->ledCount * state->meshIndex + state->ledIndex;
  return Wheel(((allLedIndex * 256 / allLedCount) + state->time / 32) % 256);
}
