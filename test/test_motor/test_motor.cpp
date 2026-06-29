#include "WiFiType.h"
#include "esp32-hal.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define pwm_channel 0
#define pwm_freq 2000
#define pwm_res 8

const char* ssid = "OpenWrt";
const char* pass = "root1234";

const char* mqtt_server   = "192.168.1.8"; 
const int   mqtt_port     = 1883;
const char* mqtt_user     = "KusumaIot"; 
const char* mqtt_password = "root1234"; 
const char* topic_motor   = "test/motor"; 

const int motorPin = 13;

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

  if (String(topic) == topic_motor) {
    int pwmValue = message.toInt();

    pwmValue = constrain(pwmValue, 0, 100);

    ledcWrite(pwm_channel, pwmValue); 
    
    Serial.print("Kecepatan motor (Channel ");
    Serial.print(pwm_channel);
    Serial.print(") diatur ke: ");
    Serial.println(pwmValue);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba koneksi MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("terhubung!");
      
      client.subscribe(topic_motor);
      Serial.print("Berhasil subscribe ke topik: ");
      Serial.println(topic_motor);
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
  
  ledcSetup(pwm_channel, pwm_freq, pwm_res);
  
  ledcAttachPin(motorPin, pwm_channel);
  
  ledcWrite(pwm_channel, 0);
  
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
