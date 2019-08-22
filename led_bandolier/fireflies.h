#pragma once

#include "animate.h"
#include "noise.h"
#include <math.h>

// Fireflies
//
// Each bandolier maintains the identity and rough location of a firefly,
// which can travel between adjacent bandoliers.
//
// Coordinating handoff at the border of bandoliers requires some
// messaging between the involved bandoliers.

// A firefly moving in two dimensions but projected into one

size_t led = 0;

void setLED(AnimationState *state) {
  double n = (noise(state->timeMs / 2500., 0, 0) + 1) / 2.;
  led = round(n * state->ledCount);
}

void fireflies(AnimationState *state, byte rgb[3]) {
  if (state->ledIndex == 0) {
    setLED(state);
  }

  if (led == state->ledIndex) {
    double n = (noise(0, state->timeMs / 1000., 0) + 1) / 2.;
    rgb[0] = floor(n * 256);
    rgb[1] = 200;
    rgb[2] = 0;
  } else {
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
  }    
}
