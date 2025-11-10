#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient client;

void setupWiFi();
void setupMQTT();
void loopMQTT();

#endif

