#pragma once

// Elsewhere we always refer to the pixel count as strip.numPixels().
const size_t NUM_PIXELS = 50;

const uint8_t dataPin  = 4;
const uint8_t clockPin = 5;
const uint8_t inputPin = 14;

Adafruit_WS2801 strip = Adafruit_WS2801(NUM_PIXELS, dataPin, clockPin);

int8_t dimnessFactors[] = {
  1, // full brightness
  2, // half
  8, // eighth
 -1, // off
};

size_t dimnessIndex = 0;
size_t dimnessCount = sizeof(dimnessFactors) / sizeof(dimnessFactors[0]);

void changeDimness() {
  // TODO Turn off the WiFi radio when dimnessFactor is -1.
  dimnessIndex = (dimnessIndex + 1) % dimnessCount;
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  size_t dimnessFactor = dimnessFactors[dimnessIndex];
  if (dimnessFactor < 0) {
    return 0;
  }
  uint32_t c;
  c = r / dimnessFactor;
  c <<= 8;
  c |= g / dimnessFactor;
  c <<= 8;
  c |= b / dimnessFactor;
  return c;
}

void clear() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Color(0, 0, 0));
  }
  strip.show();
}

