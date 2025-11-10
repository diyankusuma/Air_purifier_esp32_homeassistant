#include "loading.h"

void drawloading(U8G2 &display, int x, int y, int width, int height, int progress) {
  if (progress < 0) progress = 0;
  if (progress > 100) progress = 100;

  // Menggunakan parameter 'display' dan memperbaiki koma yang hilang
  display.drawFrame(x, y, width, height);
  
  // Memperbaiki logika pembagian dan menggunakan 'display'
  int barWidth = (width - 2) * (progress / 100.0);
  
  // Memperbaiki salah ketik 'draBox' dan menggunakan 'display'
  display.drawBox(x + 1, y + 1, barWidth, height - 2);
}

void playloading (U8G2 &display) {
  // Memperbaiki salah ketik 'int !'
  for (int i = 0; i <= 100; i++){
    // Menggunakan parameter 'display' untuk semua pemanggilan fungsi
    display.firstPage();
    do {
      display.setFont(u8g2_font_ncenB08_tr);
      display.drawStr(42, 25, "BOOTING");
      drawloading(display, 14, 35, 100, 12, i);
    } while (display.nextPage());
      delay(12); 
  }
}
