#include "wifi_mqtt.h"
#include "Arduino.h"
#include "control.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "oled.h"
#include "state.h"
#include "oled_control.h"
#include <cstring>

const char* ssid = "OpenWrt";
const char* pass = "root1234";
const char* mqtt_server = "192.168.1.8";
const int port = 1883;
const char* mqtt_user = "KusumaIot";
const char* mqtt_pass = "root1234";
const char* topic_relay = "Esp/Relay";
const char* topic_pwm = "Esp/pwm";
const char* topic_restart = "Esp/restart";
const char* topic_pid = "Esp/pid";

WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length) {

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial.print("Pesan diterima [ ");
  Serial.print(topic);
  Serial.print(" ] ");
  Serial.println(message);

  if (strcmp(topic, topic_relay) == 0) {
    if (!pid_enable) {
      if (strcmp(message, "1") == 0) {
        relay_control(true);
      } else if (strcmp(message, "0") == 0) {
        relay_control(false);
      } 
    }
  } else if (strcmp(topic, topic_pwm) == 0) {
    int value = atoi(message);
    pwm_control(value);

  } else if (strcmp(topic, topic_restart) == 0 ) {
    ESP.restart();
  
  } else if (strcmp(topic, topic_pid) == 0) {
    if (strcmp(message, "1") == 0) {
      pid_enable = true;
      beep();
      wake_oled();
    } else if (strcmp(message, "0") == 0) {
      pid_enable = false;
      beep();
      wake_oled();
    }
  }
}

void setupWiFi() {
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); 
    Serial.print(".");
  }
  Serial.println("\nWiFi tersambung!");
}

void setupMQTT() {
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}


void loopMQTT() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      
      Serial.print("Mencoba koneksi MQTT...");
      if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
        Serial.println("berhasil!");
        client.subscribe(topic_relay);
        client.subscribe(topic_pwm);
        client.subscribe(topic_restart);
        client.subscribe(topic_pid);
      } else { 
        Serial.print("gagal, rc=");
        Serial.print(client.state());
        Serial.println(" Coba lagi dalam 5 detik");
      }
    }
  } else {
    client.loop();
  }
}

