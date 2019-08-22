#pragma once
#include "animate.h"
#include "rainbows.h"
#include <math.h>

double xRange = 8;
double yRange = 50;

double cx = 0, cy = 0;
void setCenter(AnimationState *state) {
  cx = xRange * pow(sin(PI * state->timeMs / 1000), 2);
  cy = yRange * pow(sin(PI * state->timeMs / 20000), 2);
}

void gyre(AnimationState *state, byte rgb[3]) {
  if (state->ledIndex == 0) {
    setCenter(state);
  }

  // For simplicity, we assume every bandolier has the same number of pixels (50).
  //double allLedCount = state->ledCount * state->meshCount;
  //double allLedIndex = state->ledCount * state->meshIndex + state->ledIndex;

  double y = yRange * state->ledIndex / double(state->ledCount);
  double x = 0;

  double d = sqrt(pow(x - cx, 2) + pow(y - cy, 2));
  double shadow = 2;

  if (d < xRange + shadow) {
    byte pos = floor(256 * d / (xRange + shadow));
    byte mod = (pos - state->timeMs / 2) % 256;
    Wheel(mod, rgb);

    if (d > xRange) {
      double factor = (xRange + shadow - d) / shadow;
      rgb[0] *= factor;
      rgb[1] *= factor;
      rgb[2] *= factor;
    }
  } else {
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
  }
}
