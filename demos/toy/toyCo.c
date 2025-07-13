#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT0
#define LED_GREEN BIT6
#define BUTTON1 BIT0
#define BUTTON2 BIT1
#define BUTTON3 BIT2
#define BUTTON4 BIT3
#define BUTTON_PORT P2IN
#define BUTTON_REN P2REN
#define BUTTON_OUT P2OUT
#define BUTTON_DIR P2DIR
#define BUZZER BIT6

void init_buttons(){
  P1REN |= BUTTON1 | BUTTON2 | BUTTON3 | BUTTON4;
  P1OUT |= BUTTON1 | BUTTON2 | BUTTON3 | BUTTON4;
  P1DIR &= ~(BUTTON1 | BUTTON2 | BUTTON3 | BUTTON4);
}
void init_leds(){
  P1DIR |= LED_RED | LED_GREEN;
  P1OUT &= ~(LED_RED | LED_GREEN);
}
void init_buzzer(){
  P2DIR |= BUZZER;
  P2SEL |= BUZZER;
}
void buzzer_set_period(short cycles){
  CCR0 = cycles;
  CCR1 = cycles >> 1;
}
void stop_buzzer(){
  CCR0 = 0;
  CCR1 = 0;
}
int main(void){
  WDTCTL = WDTPW | WDTHOLD;
  BCSCTL3 = LFXT1S_2;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  enableWDTInterrupts();
  configureClocks();
  init_buzzer();
  init_leds();

  or_sr(0x8);

  while(1){
    if(!(BUTTON_PORT & BUTTON1)){
      P1OUT |= LED_RED;
      P1OUT &= ~LED_GREEN;
      buzzer_set_period(1000);
    }else if (!(BUTTON_PORT & BUTTON2)){
      P1OUT |= LED_GREEN;
      P1OUT &= ~LED_RED;
      buzzer_set_period(2000);  
    }else if (!(BUTTON_PORT & BUTTON3)){
      P1OUT ^= LED_RED;
      buzzer_set_period(1500);
      __delay_cycles(100000);
    }else if (!(BUTTON_PORT & BUTTON4)){
      P1OUT ^= LED_GREEN;
      buzzer_set_period(2500);
      __delay_cycles(100000);
    }else{
      P1OUT &= ~(LED_RED | LED_GREEN);
      stop_buzzer();
    }
}
}
