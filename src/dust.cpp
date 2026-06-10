#include "dust.h"
#include "esp32-hal-gpio.h"
#include "pin.h"
#include <SD_ZH03B.h>
#include "control.h"
#include "state.h"
#include "oled_control.h"

unsigned long buzzerStartTime = 0;
bool buzzerActive = false;
bool buzzerSound = false;

HardwareSerial zhSerial(2);
SD_ZH03B dustSensor(zhSerial);

int pm25 = 0;
int pm10 = 0;

void setupDust() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  zhSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void readDust() {

  if (pid_enable) {
    if (pm25 > 22) {
      digitalWrite(Relay_pin, HIGH);
      relayState = true;
      wake_oled();
    } else {
      digitalWrite(Relay_pin, LOW);
      relayState = false;
    }
  }
  
      const unsigned long BUZZER_DURATION = 5000;
      const int PM25_THRESHOLD = 40;
 
      if (buzzerActive && (millis() - buzzerStartTime >= BUZZER_DURATION)) {
        buzzerActive = false;
        digitalWrite(buzzerPin, LOW);
      }

      if (dustSensor.readData()) {
        pm25 = (int)dustSensor.getPM2_5();
        if (pm25 > 100) {
          pm25 = 100;
        }
        pm10 = (int)dustSensor.getPM10_0();
        if (pm10 > 100) {
          pm10 = 100;
        }

        if (pm25 > PM25_THRESHOLD) {
          if (!buzzerSound && !buzzerActive) {
            buzzerActive = true;
            buzzerSound = true; 
            buzzerStartTime = millis();
            digitalWrite(buzzerPin, HIGH);
          }
        } else {
          buzzerSound = false;
        }

      }
    }
