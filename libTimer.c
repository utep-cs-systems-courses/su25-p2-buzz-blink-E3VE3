#include <msp430.h>
#include "libTimer.h"

void configureClocks() {
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;
  BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 16MHz
}

void enableWDTInterrupts() {
  WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL; // Timer mode
  IE1 |= WDTIE;
}

void timerAUpmode() {
  TACTL = TASSEL_2 + MC_1; // SMCLK, up mode
  CCTL0 = CCIE;
  CCR0 = 1000;
}
