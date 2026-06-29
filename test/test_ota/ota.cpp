#include "WiFiType.h"
#include "esp32-hal.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>


const char* ssid = "OpenWrt";
const char* pass = "root1234";

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  
  }
  
  ArduinoOTA.setHostname("OTA_project");
  ArduinoOTA.begin();
 
}

void loop() {
  ArduinoOTA.handle();
}
