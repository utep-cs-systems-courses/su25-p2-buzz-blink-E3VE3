#ifndef lcdutils_included
#define lcdutils_included

extern unsigned int screenWidth, screenHeight;

void lcd_init();
void clearScreen(unsigned short color);
void drawPixel(unsigned short col, unsigned short row, unsigned short color);
void fillRectangle(unsigned short col, unsigned short row,
                   unsigned short width, unsigned short height,
                   unsigned short color);

#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F

#endif