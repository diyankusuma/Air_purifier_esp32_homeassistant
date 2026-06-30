#include "WiFiType.h"
#include "esp32-hal.h"
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <U8g2lib.h>
#include <ArduinoOTA.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C oled1(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
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
  oled1.setI2CAddress(0x3D * 2); 
  
  oled1.begin();
}

void loop() {
  ArduinoOTA.handle();
  oled1.clearBuffer();          
  oled1.setFont(u8g2_font_ncenB08_tr); 
  
  oled1.drawStr(0, 24, "OLED SH1106 Test");
  oled1.drawStr(0, 40, "Alamat: 0x3D");
  oled1.drawStr(0, 56, "Status: BERHASIL!");
  oled1.sendBuffer();          
  
  delay(1000);
}
