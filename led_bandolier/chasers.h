#pragma once

#include "animate.h"

byte BRIGHTNESS_BY_OFFSET[] = {255, 255, 255, 128, 64, 64};
byte MY_COLORS[][3] = {
  {255, 0, 0},
  {0, 0, 255},
};
// TODO: someone better at C could keep this in sync automatically.
byte NUM_COLORS = 2;

// Input a value 0 to 255 to get a color bytearray [r,g,b].
// The colours are a transition r - g -b - back to r
void ChaserWheel(byte WheelPos, byte rgb[]) {
  // Divide the wheel into portions according to how many colors there are.
  byte WHEEL_SLICE_SIZE = 255 / NUM_COLORS;

  byte* color = MY_COLORS[WheelPos / WHEEL_SLICE_SIZE];
  char sliceMidpoint = WHEEL_SLICE_SIZE / 2;
  char positionWithinSlice = WheelPos % WHEEL_SLICE_SIZE;
  int offsetFromMidpoint = sliceMidpoint - positionWithinSlice;
  byte brightness = BRIGHTNESS_BY_OFFSET[abs(offsetFromMidpoint)];

  byte r = int(color[0]) * brightness / 255;
  byte g = int(color[1]) * brightness / 255;
  byte b = int(color[2]) * brightness / 255;

  setRGB(r,g,b,rgb);
}

// Animate some colors that chase each other around the wheel. Each color will
// have a few bright pixels in the middle and a dimmer halo around it.
void chasers(AnimationState *state, byte rgb[]) {
  size_t ledCount = state->ledCount;
  size_t ledIndex = state->ledIndex;
  ChaserWheel(((ledIndex * 256 / ledCount) + state->time) % 256, rgb);
}
