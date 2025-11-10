#include "wifi_mqtt.h"
#include "HardwareSerial.h"
#include "IPAddress.h"
#include "WiFiType.h"
#include "control.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <cstring>
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "pin.h"
#include "state.h"
#include "oled.h"
#include "oled_control.h"
#include "mqtt_publish.h"

const char* ssid = "KusumaSmart";
const char* pass = "otwlulus";
const char* mqtt_server = "192.168.1.8";
const int port = 1883;
const char* mqtt_user = "KusumaIot";
const char* mqtt_pass = "root1234";
const char* topic_relay = "Esp/Relay";
const char* topic_pwm = "Esp/pwm";
const char* topic_restart = "Esp/restart";
const char* topic_pid = "Esp/pid";

IPAddress local_IP(192, 168, 1, 5);
IPAddress gateaway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDns(8, 8, 8, 8);
IPAddress secondDns(8, 8, 4, 4);

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
    if (strcmp(message, "1") == 0) {
      relay_control(true);
    } else if (strcmp(message, "0") == 0) {
      relay_control(false);
    }
  } else if (strcmp(topic, topic_pwm) == 0) {
    // Konversi message ke integer menggunakan atoi (lebih efisien dari toInt())
    int value = atoi(message);
    pwm_control(value);

  } else if (strcmp(topic, topic_restart) == 0 ) {
    if (strcmp(message, "1") == 0 ) {
    ESP.restart();
    }
  
  } else if (strcmp(topic, topic_pid) == 0 ) {
    if (strcmp(message, "1") == 0 || strcmp(message, "true") == 0 ) {
      if (!pid_enable) {
        pid_enable = true;
        beep();
        wake_oled();
        client.publish(topic_status_pid, "1");
      
      }
    
    } else if (strcmp(message, "0") == 0 || strcmp(message, "false") == 0) {
      if (pid_enable) {
        pid_enable = false;
        pwm_control(0);
        wake_oled();
        client.publish(topic_status_pid, "0");
        digitalWrite(buzzerPin, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, LOW);
      
      }
    
    }
  
  }
}

void setupWiFi() {
  if (!WiFi.config(local_IP, gateaway, subnet, primaryDns, secondDns)) {
    Serial.print("Ip statis gagal");  
  }

  WiFi.begin(ssid, pass);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("wifi tersambung");
    Serial.print("Alamat IP: ");
    Serial.print(WiFi.localIP());
  } else {
    Serial.print("Gagal terhubung ke wifi restart");
    delay(3000);
    ESP.restart();
  }
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
