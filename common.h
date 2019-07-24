#pragma once

// Elsewhere we always refer to the pixel count as strip.numPixels().
const size_t NUM_PIXELS = 50;

const uint8_t dataPin  = 4;
const uint8_t clockPin = 5;
const uint8_t inputPin = 14;

Adafruit_WS2801 strip = Adafruit_WS2801(NUM_PIXELS, dataPin, clockPin);


