#include "WiFiType.h"
#include "esp32-hal.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "OpenWrt";
const char* pass = "root1234";

const char* mqtt_server   = "192.168.1.8"; 
const int   mqtt_port     = 1883;
const char* mqtt_user     = "KusumaIot"; 
const char* mqtt_password = "root1234"; 
const char* mqtt_topic    = "test/touch";  

const int touchPin = 14;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
int lastTouchState = -1; // Menyimpan status terakhir agar tidak spam data yang sama

void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba koneksi MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("terhubung!");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(touchPin, INPUT_PULLUP);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Terhubung!");
  
  client.setServer(mqtt_server, mqtt_port);
  
  ArduinoOTA.setHostname("OTA_project");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int currentTouchState = digitalRead(touchPin);

  if (currentTouchState != lastTouchState) {
    lastTouchState = currentTouchState;

    char msg[2];
    snprintf(msg, sizeof(msg), "%d", currentTouchState);

    Serial.print("Status TTP223 berubah [");
    Serial.print(mqtt_topic);
    Serial.print("]: ");
    Serial.println(msg);
    
    client.publish(mqtt_topic, msg);
  }
}
