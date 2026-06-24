#ifndef DUST_H
#define DUST_H

#include <Arduino.h>

extern int pm25;
extern int pm10;
extern int pm25Indicator;

void setupDust();
void readDust();
void pm25_Position();

#endif

