#include "loading.h"

void drawloading(U8G2 &display, int x, int y, int width, int height, int progress) {
  if (progress < 0) progress = 0;
  if (progress > 100) progress = 100;

  display.drawFrame(x, y, width, height);
  
  int barWidth = (width - 2) * (progress / 100.0);
  
  display.drawBox(x + 1, y + 1, barWidth, height - 2);
}

void playloading (U8G2 &display) {
  for (int i = 0; i <= 100; i++){
    display.firstPage();
    do {
      display.setFont(u8g2_font_ncenB08_tr);
      display.drawStr(42, 25, "BOOTING");
      drawloading(display, 14, 35, 100, 12, i);
    } while (display.nextPage());
      delay(12); 
  }
}
