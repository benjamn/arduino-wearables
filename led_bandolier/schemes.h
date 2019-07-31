#pragma once

typedef struct {
  // Maximum of 4 colors, each is 3 bytes rgb
  byte colors[4][3];
  byte numColors;
} ColorScheme;


void setScheme(byte colors[][3], ColorScheme& scheme) {
  int numColors = sizeof(colors) / sizeof(colors[0]);
  scheme.numColors = numColors;

  for (int i = 0; i < numColors; i++) {
    for (int j = 0; j < 3; j++) {
      scheme.colors[i][j] = colors[i][j];
    }
  }
}

// Unfortunately these schemes are initialized at runtime in modeSetup() because
// "nontrivial initializers are not implemented".
ColorScheme schemes[3];
byte schemeCount = sizeof(schemes) / sizeof(schemes[0]);
byte schemeIndex = 0;

ColorScheme& currentScheme() {
  return schemes[schemeIndex];
}

void colorSchemesSetup() {
  // Initialize the color schemes
    
  // Scheme 0: red, green, and blue
  byte scheme0[][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
  setScheme(scheme0, schemes[0]);
  
  // Scheme 1: purple and pink
  byte scheme1[][3] = {{215, 0, 130}, {110, 0, 210}};
  setScheme(scheme1, schemes[1]);

  // Scheme 2: orange-red and light blue
  byte scheme2[][3] = {{255, 84, 10}, {10, 190, 255}};
  setScheme(scheme2, schemes[2]);
}
