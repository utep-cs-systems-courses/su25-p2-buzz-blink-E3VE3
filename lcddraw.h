#ifndef lcddraw_included
#define lcddraw_included

void drawChar5x7(unsigned short x, unsigned short y, char c,
                 unsigned short fg, unsigned short bg);
void drawString5x7(unsigned short x, unsigned short y, char *string,
                   unsigned short fg, unsigned short bg);
void drawString11x16(unsigned short x, unsigned short y, char *string,
                     unsigned short fg, unsigned short bg);

#endif
