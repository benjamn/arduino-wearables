#pragma once
#include "animate.h"
#include <math.h>

double cooling = 0.065;
double sparking = 0.45; // fire sparking rate

#define PIXEL_COUNT 50
size_t pixelsUsed = PIXEL_COUNT;

#define DIRECTION_MODE 0
// direction (and symmetry) of fire
// one-sided:  0 - from head, 1 - from tail
// symmetric:  2 - from ends, 3 - from middle

byte heatColors_r[256];
byte heatColors_g[256];
byte heatColors_b[256];

double heat[PIXEL_COUNT];

void fireSetup() {
  // create HeatColors 8-bit palette:  0x000000,
  //  0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
  //  0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00,
  //  0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF
  for (size_t i = 0; i < 256; i++) {
    if (i < 85) {
      heatColors_r[i] = floor(256 * float(i) / 85);
    } else {
      heatColors_r[i] = 255;
    }
    if (i >= 85) {
      if (i < 170) {
        heatColors_g[i] = floor(256 * float(i - 85) / 85);
      } else {
        heatColors_g[i] = 255;
      }
    }
    if (i >= 170) {
      heatColors_b[i] = floor(256 * float(i - 170) / 85);
    }
  }

  if (DIRECTION_MODE < 2) {
    // PIXEL_COUNT with respect to DIRECTION_MODE
    pixelsUsed = PIXEL_COUNT;
  } else {
    pixelsUsed = floor(PIXEL_COUNT / 2);
    cooling += 0.04; // use a little more cooling to match
  }

  for (size_t i = 0; i < sizeof(heat) / sizeof(heat[0]); ++i) {
    heat[i] = 0;
  }
}

double clamp(double value, double low, double high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

double randouble(double max) {
  return max * double(random(10000)) / 10000;
}

byte updateEvery = 2;
byte updateCounter = 0;

void update() {
  updateCounter = (updateCounter + 1) % updateEvery;
  if (updateCounter > 0) return;

  // step 1: cool down the entire strip
  for (size_t i = 0; i < pixelsUsed; i++) {
    heat[i] = clamp(heat[i] - randouble(cooling), 0, 1);
  }

  // step 2:  carry heat 'up' the strip
  for (size_t i = pixelsUsed - 1; i >= 2; i--) {
    heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
  }

  // step 3:  add sparks to the base of the flame
  if (sparking > randouble(1)) {
    size_t i = floor(randouble(pixelsUsed / 10)); // sparks form in first 10% of strip
    heat[i] = heat[i] + randouble(0.37) + 0.63; // add a minimum amount of heat to the pixel
    heat[i] = clamp(heat[i], 0, 1);
  }
}

void fire(AnimationState *state, byte rgb[3]) {
  size_t index = state->ledIndex;

  if (index == 0) update();

  byte heatLevel = 0;

  if (DIRECTION_MODE == 0) {
    heatLevel = clamp(heat[index] * 256, 0, 255);
  }

  if (DIRECTION_MODE == 1) {
    heatLevel = clamp(heat[pixelsUsed - index - 1] * 256, 0, 255);
  }

  if (DIRECTION_MODE == 2) {
    if (index < pixelsUsed) {
      heatLevel = clamp(heat[index] * 256, 0, 255);
    } else {
      heatLevel = clamp(heat[2 * pixelsUsed - index - 1] * 256, 0, 255);
    }
  }

  if (DIRECTION_MODE == 3) {
    if (index < pixelsUsed) {
      heatLevel = clamp(heat[pixelsUsed - index - 1] * 256, 0, 255);
    } else {
      heatLevel = clamp(heat[index - pixelsUsed] * 256, 0, 255);
    }
  }

  rgb[0] = heatColors_r[heatLevel];
  rgb[1] = heatColors_g[heatLevel];
  rgb[2] = heatColors_b[heatLevel];
}
