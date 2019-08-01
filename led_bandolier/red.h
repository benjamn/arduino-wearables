#pragma once
#include "animate.h"

void solidRed(AnimationState *state, byte rgb[3]) {
  rgb[0] = 255;
  rgb[1] = 0;
  rgb[2] = 0;
}
