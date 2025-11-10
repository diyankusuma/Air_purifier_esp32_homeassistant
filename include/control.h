#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

extern bool relayState;
extern int pwmValue;

void setupControl();
void relay_control(bool state);
void pwm_control(int value);

#endif

