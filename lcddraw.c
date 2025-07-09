#include "lcdutils.h"
#include "lcddraw.h"

static const char font_5x7[][5] = {
  {0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x5f,0x00,0x00},
  {0x00,0x07,0x00,0x07,0x00}, {0x14,0x7f,0x14,0x7f,0x14} // etc.
};

void drawChar5x7(unsigned short x, unsigned short y, char c,
                 unsigned short fg, unsigned short bg) {
  if (c < 32 || c > 126) c = '?';
  const char *glyph = font_5x7[c - 32];
  for (char col = 0; col < 5; col++) {
    char bits = glyph[col];
    for (char row = 0; row < 7; row++) {
      unsigned short color = (bits & (1 << row)) ? fg : bg;
      drawPixel(x + col, y + row, color);
    }
  }
}

void drawString5x7(unsigned short x, unsigned short y, char *string,
                   unsigned short fg, unsigned short bg) {
  while (*string) {
    drawChar5x7(x, y, *string++, fg, bg);
    x += 6;
  }
}

void drawString11x16(unsigned short x, unsigned short y, char *string,
                     unsigned short fg, unsigned short bg) {
  drawString5x7(x, y, string, fg, bg);
}
