#include "oled.h"
#include "Arduino.h"
#include "clib/u8g2.h"
#include "dust.h"
#include "mq_7.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "icons.h"
#include "pin.h"
#include "loading.h"

// OLED 1: SH1106 (alamat 0x3D)
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled1(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void beep(){
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(250);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

void setupOLED() {
  Wire.begin();
  oled1.setI2CAddress(0x3D * 2);

  oled1.begin();

  playloading(oled1);
  oled1.clearBuffer();
  oled1.sendBuffer();
  
}

void PID_auto() {
  //task1
  oled1.clearBuffer();
  oled1.setFont(u8g2_font_ncenB10_tr);
  oled1.setCursor(4, 12);
  oled1.print("Auto");
  oled1.setCursor(62, 14);
  oled1.print("PM25");
  oled1.setCursor(58, 27);
  oled1.print(pm25);
  oled1.print(" ug/m3");

  /*oled1.setCursor(72, 42);
  oled1.print("CO");
  oled1.setCursor(58, 57);
  oled1.print(ppm_mq7);
  oled1.print(" PPM");*/
  oled1.drawXBMP(4, 22, 32, 32, ok_icon);
  oled1.drawBox(49, 0, 3, 64);

  oled1.setFont(u8g2_font_u8glib_4_tf);
  oled1.setCursor(55, 43);
  oled1.print("GOOD");
  oled1.drawBox(55, 45, 23, 7); //bar condition (x, y, l, t)
  
  oled1.setFont(u8g2_font_u8glib_4_tf);
  oled1.setCursor(80, 43);
  oled1.print("SAFE");
  oled1.drawBox(80, 45, 23, 7); 

  oled1.setFont(u8g2_font_u8glib_4_tf);
  oled1.setCursor(105, 43);
  oled1.print("TOXIC");
  oled1.drawBox(105, 45, 23, 7); 

  oled1.setFont(u8g2_font_5x7_tf);
  oled1.setCursor(pm25Indicator, 60);
  oled1.print("^");

  oled1.sendBuffer();
}

void PID_manual() {
  oled1.clearBuffer();
  oled1.setFont(u8g2_font_ncenB08_te);
  oled1.setCursor(4, 12);
  oled1.print("Manual");
  oled1.setFont(u8g2_font_ncenB10_tr);
  oled1.setCursor(62, 14);
  oled1.print("PM25");
  oled1.setCursor(58, 27);
  oled1.print(pm25);
  oled1.print(" ug/m3");

  /*oled1.setCursor(72, 42);
  oled1.print("CO");
  oled1.setCursor(58, 57);
  oled1.print(ppm_mq7);
  oled1.print(" PPM");*/
  oled1.drawXBMP(4, 22, 32, 32, manual_icon);
  oled1.drawBox(49, 0, 3, 64);

  oled1.setFont(u8g2_font_u8glib_4_tf);
  oled1.setCursor(55, 43);
  oled1.print("GOOD");
  oled1.drawBox(55, 45, 23, 7); //bar condition (x, y, l, t)
  
  oled1.setFont(u8g2_font_u8glib_4_tf);
  oled1.setCursor(80, 43);
  oled1.print("SAFE");
  oled1.drawBox(80, 45, 23, 7); 

  oled1.setFont(u8g2_font_u8glib_4_tf);
  oled1.setCursor(105, 43);
  oled1.print("TOXIC");
  oled1.drawBox(105, 45, 23, 7); 

  oled1.setFont(u8g2_font_5x7_tf);
  oled1.setCursor(pm25Indicator, 60);
  oled1.print("^");

  oled1.sendBuffer();

}

void display_on(){
  oled1.setPowerSave(0);
}

void display_off(){
  oled1.setPowerSave(1);
}


