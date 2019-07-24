#pragma once

#include <painlessMesh.h>

#define MESH_PREFIX   "UltimateCelebration"
#define MESH_PASSWORD "bungeecouchgarden"
#define MESH_PORT     2453

painlessMesh mesh;
Scheduler userScheduler;

void sendMessage();
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);
void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections %s\n", mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void meshSetup() {
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

uint32_t lastTime = 0;

void meshLoop() {
  userScheduler.execute();
  mesh.update();

  uint32_t now = mesh.getNodeTime();
  if (now - lastTime > 1000000) {
    lastTime = now;
    Serial.printf("Time: %d\n", now);
  }
}

uint32_t meshTimeMs() {
  return mesh.getNodeTime() / 1000;
}

size_t meshGetNodes(std::list<uint32_t> &nodes) {
  nodes.clear();
  nodes = mesh.getNodeList();
  return nodes.size();
}

