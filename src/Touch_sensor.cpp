#include <Arduino.h>
#include "Touch_sensor.h"
#include "control.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "pin.h"
#include "state.h"
#include "oled.h"
#include "oled_control.h"
#include "mqtt_publish.h"
#include "wifi_mqtt.h"

int touch_cond = 0;
static unsigned long touchTime = 0;
const long debounce = 200;

void touch_setup() {
  pinMode(Touch_pin, INPUT_PULLUP);
}


void touch_read() {
  int touch_cond = digitalRead(Touch_pin);

  if (touch_cond == HIGH) {
    if ((millis() - touchTime) > debounce) {
      pid_enable = !pid_enable;
      touchTime = millis();
      wake_oled();

      if (pid_enable) {
        beep();
        client.publish(topic_status_pid, "1");

      } else {
      pwm_control(0);
      client.publish(topic_status_pid, "0");
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
      }
    }
  }
}
