#include "icons.h"

void ok(U8G2 &display) {
  display.clearBuffer();
  display.drawXBMP(4, 2, 64, 64, ok_icon);
  display.sendBuffer();
}

void fan(U8G2 &display) {
  display.clearBuffer();
  display.drawXBMP(4, 2, 64, 64, fan_icon);
  display.sendBuffer();
}

void setting(U8G2 &display) {
  display.clearBuffer();
  display.drawXBMP(22, -8, 80, 80, setting_icon);
  display.sendBuffer();
}

void manual_icons(U8G2 &display){
  display.clearBuffer();
  display.drawXBMP(4, 2, 32, 32, manual_icon);
}
