#ifndef LOADING_H
#define LOADING_H

#include <U8g2lib.h>

// Deklarasi fungsi harus cocok dengan implementasi dan pemanggilannya
void drawloading(U8G2 &display, int x, int y, int width, int height, int progress);
void playloading(U8G2 &display);

#endif
