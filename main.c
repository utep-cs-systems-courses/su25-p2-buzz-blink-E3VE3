#include <msp430.h>
#include "libTimer.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "pet_idle.h"
#include "pet_feed.h"
#include "pet_play.h"
#include "pet_sleep.h"

#define LED_RED BIT0
#define LED_GREEN BIT6
#define LEDS (LED_RED | LED_GREEN)
#define BUTTONS (BIT0 | BIT1 | BIT2 | BIT3) // P2.0–P2.3
#define SPEAKER BIT6 // P2.6

enum State { IDLE, FEED, PLAY, SLEEP };
volatile enum State state = IDLE;

extern void next_state_asm();

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}

void button_init() {
  P2REN |= BUTTONS;
  P2OUT |= BUTTONS;
  P2DIR &= ~BUTTONS;
  P2IE |= BUTTONS;
  P2IES |= BUTTONS;
  P2IFG &= ~BUTTONS;
}

void speaker_init() {
  P2DIR |= SPEAKER;
  P2OUT &= ~SPEAKER;
}

void play_sound(int cycles) {
  for (int i = 0; i < cycles; i++) {
    P2OUT ^= SPEAKER;
    __delay_cycles(1000);
  }
  P2OUT &= ~SPEAKER;
}

void drawBitmap(int x, int y, int width, int height, const unsigned char *bitmap,
                unsigned short fg, unsigned short bg) {
  int byteWidth = (width + 7) / 8;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      int byteIndex = j * byteWidth + i / 8;
      if (bitmap[byteIndex] & (0x80 >> (i % 8)))
        drawPixel(x + i, y + j, fg);
      else
        drawPixel(x + i, y + j, bg);
    }
  }
}

void update_screen() {
  clearScreen(COLOR_WHITE);
  switch (state) {
    case IDLE:
      drawBitmap(56, 60, 16, 16, pet_idle_bitmap, COLOR_BLACK, COLOR_WHITE);
      break;
    case FEED:
      drawBitmap(56, 60, 16, 16, pet_feed_bitmap, COLOR_BLACK, COLOR_YELLOW);
      break;
    case PLAY:
      drawBitmap(56, 60, 16, 16, pet_play_bitmap, COLOR_BLACK, COLOR_CYAN);
      break;
    case SLEEP:
      drawBitmap(56, 60, 16, 16, pet_sleep_bitmap, COLOR_WHITE, COLOR_BLUE);
      break;
  }
}

void update_leds() {
  switch (state) {
    case IDLE: P1OUT = LED_GREEN; break;
    case FEED: P1OUT = LED_RED; break;
    case PLAY:
      for (int i = 0; i < 3; i++) {
        P1OUT = LEDS;
        __delay_cycles(100000);
        P1OUT = 0;
        __delay_cycles(100000);
      }
      break;
    case SLEEP: P1OUT = 0; break;
  }
}

void handle_state() {
  update_leds();
  update_screen();
  switch (state) {
    case FEED: play_sound(300); break;
    case PLAY: play_sound(500); break;
    default: break;
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & BIT0) state = FEED;
  else if (P2IFG & BIT1) state = PLAY;
  else if (P2IFG & BIT2) state = SLEEP;
  else if (P2IFG & BIT3) next_state_asm();
  P2IFG &= ~BUTTONS;
  handle_state();
}

int main() {
  WDTCTL = WDTPW | WDTHOLD;
  led_init();
  button_init();
  speaker_init();
  configureClocks();
  lcd_init();
  update_screen();
  enableWDTInterrupts();
  __bis_SR_register(GIE);
  while (1) __low_power_mode_0();
}
