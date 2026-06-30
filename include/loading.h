#ifndef LOADING_H
#define LOADING_H

#include <U8g2lib.h>

struct loadingItem {
  String label;
  int progress;
};

void drawAllBar(U8G2 &display, loadingItem items[], int size);
void drawloading(U8G2 &display, int x, int y, int width, int height, int progress);
void playloading(U8G2 &display);
void playAllBars(U8G2 &display, loadingItem items[], int size, int durationMs);

#endif
