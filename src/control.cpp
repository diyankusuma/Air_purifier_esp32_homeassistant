#include "control.h"
#include "pin.h"

#define pwm_channel 0
#define pwm_freq 2000
#define pwm_res 8

bool relayState = false;
int pwmValue = 0;

void setupControl() {
  pinMode(Relay_pin, OUTPUT);
  relay_control(false);

  ledcSetup(pwm_channel, pwm_freq, pwm_res);
  ledcAttachPin(Motor_pin, pwm_channel);
}

void relay_control(bool state) {
  relayState = state;
  digitalWrite(Relay_pin, relayState ? HIGH : LOW);
}

void pwm_control(int value) {
  pwmValue = constrain(value, 0, 150);
  ledcWrite(pwm_channel, pwmValue);
}

