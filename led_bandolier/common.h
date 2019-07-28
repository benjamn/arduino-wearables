#pragma once

// Elsewhere we always refer to the pixel count as strip.numPixels().
const size_t NUM_PIXELS = 50;

const uint8_t dataPin  = 4;
const uint8_t clockPin = 5;
const uint8_t inputPin = 14;

Adafruit_WS2801 strip = Adafruit_WS2801(NUM_PIXELS, dataPin, clockPin);

int8_t switchDimnessFactors[] = {
  // disable full brightness
  1, // full brightness
  2, // half
  8, // eighth
 -1, // off
};

byte MAX_BRIGHT = 255;

size_t dimnessIndex = 0;
size_t dimnessCount = sizeof(switchDimnessFactors) / sizeof(switchDimnessFactors[0]);

void changeDimness() {
  // TODO Turn off the WiFi radio when dimnessFactor is -1.
  dimnessIndex = (dimnessIndex + 1) % dimnessCount;
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte rgb[], byte brightness)
{
  size_t switchDimnessFactor = switchDimnessFactors[dimnessIndex];
  if (switchDimnessFactor < 0) {
    return 0;
  }

  byte r = rgb[0];
  byte g = rgb[1];
  byte b = rgb[2];
  
  uint32_t c;
  c = r / switchDimnessFactor * brightness / MAX_BRIGHT;
  c <<= 8;
  c |= g / switchDimnessFactor * brightness / MAX_BRIGHT;
  c <<= 8;
  c |= b / switchDimnessFactor * brightness / MAX_BRIGHT;
  return c;
}

void setRGB(byte r, byte g, byte b, byte rgb[]) {
  rgb[0] = r;
  rgb[1] = b;
  rgb[2] = g;
}

void clear() {
  for (int i = 0; i < strip.numPixels(); i++) {
    byte rgb[] = {0, 0, 0};
    strip.setPixelColor(i, Color(rgb, 0));
  }
  strip.show();
}
