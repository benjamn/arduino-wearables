#include "Adafruit_WS2801.h"
#include "SPI.h"
#include <EEPROM.h>

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
uint8_t dataPin  = 4;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 5;    // Green wire on Adafruit Pixels
uint8_t inputPin = 14;

// Elsewhere we always refer to the pixel count as strip.numPixels().
const size_t NUM_PIXELS = 50;
Adafruit_WS2801 strip = Adafruit_WS2801(NUM_PIXELS, dataPin, clockPin);

void setup() {
  strip.begin();
  clear();
  pinMode(0, OUTPUT);
  pinMode(inputPin, INPUT);
}

const int pause = 7000;

bool isButtonPressed() {
  return !digitalRead(inputPin);
}

bool wasPressed = false;

void loop() {
  if (isButtonPressed()) {
    digitalWrite(0, LOW);
    clear();
    wasPressed = true;
  } else {
    if (wasPressed) {
      changeDimness();
    }
    digitalWrite(0, HIGH);
    rainbowCycle();
    wasPressed = false;
  }
}

void clear() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Color(0, 0, 0));
  }
  strip.show();
}

int j = 0;

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) % 256));
  }  
  strip.show();
  j = (j + 1) % (256 * 3);
}

int8_t dimnessFactors[] = {
  1, // full brightness
  2, // half
  8, // eighth
 -1, // off
};
size_t dimnessIndex = 0;
size_t dimnessCount = sizeof(dimnessFactors) / sizeof(dimnessFactors[0]);

void changeDimness() {
  dimnessIndex = (dimnessIndex + 1) % dimnessCount;
}

/* Helper functions */

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

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  
  if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  
  WheelPos -= 170; 
  return Color(0, WheelPos * 3, 255 - WheelPos * 3);
}

