#include <msp430.h>
#include "lcdutils.h"
#include "libTimer.h"

unsigned int screenWidth = 128;
unsigned int screenHeight = 160;

void lcd_init() {
  configureClocks();
  enableWDTInterrupts();
}

void clearScreen(unsigned short color) {
  fillRectangle(0, 0, screenWidth, screenHeight, color);
}

void drawPixel(unsigned short col, unsigned short row, unsigned short color) {
  // Stub for pixel draw — implement SPI LCD draw here if needed
}

void fillRectangle(unsigned short col, unsigned short row,
                   unsigned short width, unsigned short height,
                   unsigned short color) {
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      drawPixel(col + c, row + r, color);
    }
  }
}
