#pragma once

// Elsewhere we always refer to the pixel count as strip.numPixels().
const size_t NUM_PIXELS = 50;

const uint8_t dataPin  = 4;
const uint8_t clockPin = 5;
const uint8_t inputPin = 14;

Adafruit_WS2801 strip = Adafruit_WS2801(NUM_PIXELS, dataPin, clockPin);

int8_t switchBrightnessFactors[] = {
  // disable full brightness
  1, // full brightness
  2, // half
  8, // eighth
 -1, // off
};

byte MAX_BRIGHT = 255;

size_t brightnessIndex = 0;
size_t brightnessCount = sizeof(switchBrightnessFactors) / sizeof(switchBrightnessFactors[0]);

void changeBrightness() {
  // TODO Turn off the WiFi radio when brightnessFactor is -1.
  brightnessIndex = (brightnessIndex + 1) % brightnessCount;
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte rgb[], byte brightness)
{
  size_t switchBrightnessFactor = switchBrightnessFactors[brightnessIndex];
  if (switchBrightnessFactor < 0) {
    return 0;
  }

  byte r = rgb[0];
  byte g = rgb[1];
  byte b = rgb[2];
  
  uint32_t c;
  c = r / switchBrightnessFactor * brightness / MAX_BRIGHT;
  c <<= 8;
  c |= g / switchBrightnessFactor * brightness / MAX_BRIGHT;
  c <<= 8;
  c |= b / switchBrightnessFactor * brightness / MAX_BRIGHT;
  return c;
}

void setRGB(byte r, byte g, byte b, byte rgb[]) {
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
}

byte weightedAvg(byte a, byte b, byte weight) {
  return byte((int(a) * (255 - weight) + int(b) * weight) / 255);
}

// return a color that is a blend of the two inputs, weight between 0 (color1) and 255 (color2)
void blend(byte color1[], byte color2[], byte weight, byte out[]) {
  for (int i = 0; i < 3; i++) {
    out[i] = weightedAvg(color1[i], color2[i], weight);
  }
}

void clear() {
  for (int i = 0; i < strip.numPixels(); i++) {
    byte rgb[] = {0, 0, 0};
    strip.setPixelColor(i, Color(rgb, 0));
  }
  strip.show();
}
