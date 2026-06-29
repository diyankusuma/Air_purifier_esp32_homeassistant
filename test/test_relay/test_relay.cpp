
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
const char* topic_relay  = "test/relay";

const int relayPin = 12;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan masuk pada topik [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (String(topic) == topic_relay) {
    if (message == "1") {
      digitalWrite(relayPin, HIGH); 
      Serial.println("relay ON");
    } else if (message == "0") {
      digitalWrite(relayPin, LOW); 
      Serial.println("relay OFF");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba koneksi MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("terhubung!");
      
      client.subscribe(topic_relay);
      Serial.print("Berhasil subscribe ke topik: ");
      Serial.println(topic_relay);
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
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); 
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Terhubung!");
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  ArduinoOTA.setHostname("OTA_project");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
}
