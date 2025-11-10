#ifndef DUST_H
#define DUST_H

#include <Arduino.h>

extern int pm25;
extern int pm10;

void setupDust();
void readDust();

#endif

