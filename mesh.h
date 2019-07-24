#pragma once

#include <painlessMesh.h>

#define MESH_PREFIX   "UltimateCelebration"
#define MESH_PASSWORD "bungeecouchgarden"
#define MESH_PORT     2453

painlessMesh mesh;
Scheduler userScheduler;

void meshSetup() {
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
}

uint32_t lastTime = 0;

void meshLoop() {
  userScheduler.execute();
  mesh.update();
}

uint32_t meshTimeMs() {
  return mesh.getNodeTime() / 1000;
}

size_t meshGetNodes(std::list<uint32_t> &nodes) {
  nodes.clear();
  nodes = mesh.getNodeList();
  return nodes.size();
}

