#include <Arduino.h>
#include "esp32-hal.h"
#include "wifi_mqtt.h"
#include "control.h"
#include "dust.h"
#include "oled.h"
#include "mq_7.h"
#include "mqtt_publish.h"
#include "PID_control.h"
#include "Touch_sensor.h"
#include "state.h"
#include "oled_control.h"
#include <ArduinoOTA.h>


unsigned long previousMillis = 0;
const long interval = 2000;
bool pid_enable = true;

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
  setupControl();
  setupDust();
  setup_mq7();
  setupPID();
  touch_setup();
  setup_oled_control();
  setupOLED();

  ArduinoOTA.setHostname("OTA_Project");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  loopMQTT();
  mqtt_publish();
  loop_mq7();
  loop_oled_control();
  loopPID();

  static unsigned long previousMillis = 0;
  const long interval = 1000;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    touch_read();
    readDust();

    if (pid_enable) {
      PID_auto();
    
    } else {
    PID_manual();
    }
  }
}

