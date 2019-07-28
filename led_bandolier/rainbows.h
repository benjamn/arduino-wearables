#pragma once
#include "animate.h"

//Input a value 0 to 255 to get a color bytearray [r,g,b].
//The colours are a transition r - g -b - back to r
void Wheel(byte pos, byte rgb[3]) {
  if (pos < 85) {
    setRGB(pos * 3, 255 - pos * 3, 0, rgb);
  } else if (pos < 170) {
    pos -= 85;
    setRGB(255 - pos * 3, 0, pos * 3, rgb);
  } else {
    pos -= 170;
    setRGB(0, pos * 3, 255 - pos * 3, rgb);
  }
}

void rainbows(AnimationState *state, byte rgb[3]) {
  // For simplicity, we assume every bandolier has the same number of pixels (50).
  size_t allLedCount = state->ledCount * state->meshCount;
  size_t allLedIndex = state->ledCount * state->meshIndex + state->ledIndex;
  Wheel(((allLedIndex * 256 / allLedCount) + state->timeMs / 16) % 256, rgb);
}
