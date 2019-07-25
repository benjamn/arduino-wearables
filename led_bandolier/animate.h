#pragma once

#include "common.h"
#include "mesh.h"
#include <list>

typedef struct {
  uint32_t time;
  size_t ledCount;
  size_t ledIndex;
  size_t meshCount;
  size_t meshIndex;
  int8_t dimnessFactor;
} AnimationState;

unsigned long lastAnimationTimeMs = 0;
size_t frameDelayMs = 16;
std::list<uint32_t> nodes;

size_t computeRank() {
  size_t rank = 0;
  uint32_t ownNodeId = mesh.getNodeId();
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    if (*it < ownNodeId) {
      ++rank;
    }
  }
  return rank;
}

void animate(uint32_t (*fn)(AnimationState*)) {
  unsigned long now = millis();
  // Render frames no more often than every 16ms, which gives a smooth 60fps.
  if (now - lastAnimationTimeMs < frameDelayMs) return;
  lastAnimationTimeMs = now;

  size_t otherNodesCount = meshGetNodes(nodes);

  AnimationState state = {
    // We use the built-in millis() function to throttle animation frames on this device,
    // but we use the synchronized mesh time for time-dependent animations, so all devices
    // connected to the mesh network can agree on the current time, regardless of when
    // they were first turned on.
    .time = meshTimeMs(),
    .ledCount = strip.numPixels(),
    .ledIndex = 0,
    .meshCount = otherNodesCount + 1,
    .meshIndex = computeRank()
  };

  while (state.ledIndex < state.ledCount) {
    strip.setPixelColor(state.ledIndex, fn(&state));
    state.ledIndex += 1;
  }

  strip.show();
}


