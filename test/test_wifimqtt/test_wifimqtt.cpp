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
const char* mqtt_topic    = "test_mqtt";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
float angle = 0.0;

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

  unsigned long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    float raw_sin = sin(angle); 
    float oscillation_value = (raw_sin + 1.0) * 50.0; 

    angle += 0.1;
    if (angle > 360) angle = 0; 

    char msg[10];
    dtostrf(oscillation_value, 4, 2, msg); 

    Serial.print("Publish data ke [");
    Serial.print(mqtt_topic);
    Serial.print("]: ");
    Serial.println(msg);
    
    client.publish(mqtt_topic, msg);
  }
}
