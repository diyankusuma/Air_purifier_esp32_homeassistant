#include "loading.h"

void drawAllBar(U8G2 &display, loadingItem items[], int size) {
  display.setFont(u8g2_font_5x7_tf);
  int startY = 10;  
  int rowGap = 15;  
  int barWidth = 60;

  for (int i = 0; i < size; i++) {
    int currentY = startY + (i * rowGap);

    display.setCursor(0, currentY + 7);
    display.print(items[i].label);

    int barX = 52;
    display.drawFrame(barX, currentY, barWidth, 8);

    int fillWidth = (barWidth - 4) * items[i].progress / 100;
    if (fillWidth > 0) {
      display.drawBox(barX + 2, currentY + 2, fillWidth, 4);
    }

    display.setCursor(114, currentY + 7);
    display.print(String(items[i].progress) + "%");
  }
}

void drawloading(U8G2 &display, int x, int y, int width, int height, int progress) {
  if (progress < 0) progress = 0;
  if (progress > 100) progress = 100;

  display.drawFrame(x, y, width, height);
  int barWidth = (width - 2) * (progress / 100.0);
  display.drawBox(x + 1, y + 1, barWidth, height - 2);
}

void playloading(U8G2 &display) {
  for (int i = 0; i <= 100; i++){
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(42, 25, "BOOTING");
    drawloading(display, 14, 35, 100, 12, i);
    display.sendBuffer();
    delay(12); 
  }
}

void playAllBars(U8G2 &display, loadingItem items[], int size, int durationMs) {
  int delayPerStep = durationMs / 100;
  
  for (int i = 0; i < size; i++) {
    for (int p = 0; p <= 100; p++) {
      items[i].progress = p;
      
      display.clearBuffer();
      drawAllBar(display, items, size);
      display.sendBuffer();
      
      delay(delayPerStep);
    }
  }
}
