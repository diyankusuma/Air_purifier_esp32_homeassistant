#ifndef TOUCH_SENSOR_H
#define TOUCH_SENSOR_H
#include <Arduino.h>

extern int touch_active;

void touch_setup();
void touch_read();

#endif // !TOUCH_SENSOR_H
