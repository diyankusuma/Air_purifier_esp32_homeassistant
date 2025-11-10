#include <Arduino.h>
#include "oled_control.h"
#include "esp32-hal.h"
#include "oled.h"

const unsigned long Oled_timeout = 300 * 1000;
unsigned long last_activity;
bool oled_on = true;

void setup_oled_control(){
  last_activity = millis();
  display_on();
}

void loop_oled_control(){
  if (oled_on && (millis() - last_activity > Oled_timeout)) {
    display_off();
    oled_on = false;  
  }
}

void wake_oled(){
  last_activity = millis();
  if (!oled_on) {
    display_on();
    oled_on = true;
  }
}
