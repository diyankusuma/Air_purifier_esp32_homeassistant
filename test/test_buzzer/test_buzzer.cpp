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
const char* topic_buzzer  = "test/buzzer";

const int buzzerPin = 27;

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

  if (String(topic) == topic_buzzer) {
    if (message == "1") {
      digitalWrite(buzzerPin, HIGH); 
      Serial.println("Buzzer ON");
    } else if (message == "0") {
      digitalWrite(buzzerPin, LOW); 
      Serial.println("Buzzer OFF");
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
      
      client.subscribe(topic_buzzer);
      Serial.print("Berhasil subscribe ke topik: ");
      Serial.println(topic_buzzer);
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
  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); 
  
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
