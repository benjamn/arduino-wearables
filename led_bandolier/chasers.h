#pragma once

#include "animate.h"
#include "modes.h"
#include "schemes.h"

byte BRIGHTNESS_BY_OFFSET[] = {255, 255, 255, 255, 255, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32};

// Input a value 0 to 255 to get a color bytearray [r,g,b].
void ChaserWheel(byte WheelPos, byte rgb[]) {
  ColorScheme& scheme = currentScheme();
  
  // Divide the wheel into portions according to how many colors there are.
  byte wheelSliceSize = 255 / scheme.numColors;
  byte* color = scheme.colors[WheelPos / wheelSliceSize];
  
  // The color is brightest in the middle of the slice, 0 near the edges.
  byte brightness = 0;
  char sliceMidpoint = wheelSliceSize / 2;
  char positionWithinSlice = WheelPos % wheelSliceSize;
  int offsetFromMidpoint = sliceMidpoint - positionWithinSlice;
  byte brightnessIndex = abs(offsetFromMidpoint);
  if (brightnessIndex < sizeof(BRIGHTNESS_BY_OFFSET)) {
    brightness = BRIGHTNESS_BY_OFFSET[abs(offsetFromMidpoint)];
  }

  byte r = int(color[0]) * brightness / 255;
  byte g = int(color[1]) * brightness / 255;
  byte b = int(color[2]) * brightness / 255;

  setRGB(r,g,b,rgb);
}

// Animate some colors that chase each other around the wheel. Each color will
// have a few bright pixels in the middle and a dimmer halo around it.
void chasers(AnimationState *state, byte rgb[3]) {
  size_t ledCount = state->ledCount;
  size_t ledIndex = state->ledIndex;
  ChaserWheel(((ledIndex * 256 / ledCount) + state->timeMs / 16) % 256, rgb);
}
