#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT0
#define LED_GREEN BIT6

#define BUTTON1 BIT0
#define BUTTON2 BIT1
#define BUTTON3 BIT2
#define BUTTON4 BIT3
#define BUTTONS (BUTTON1 | BUTTON2 | BUTTON3 | BUTTON4)

#define BUZZER BIT6
// State definitions
typedef enum {
  STATE_OFF,
  STATE_RED_ON,
  STATE_GREEN_ON,
  STATE_RED_TOGGLE,
  STATE_GREEN_TOGGLE
} State;

volatile State current_state = STATE_OFF;

void init_buttons(){
  P2REN |= BUTTONS;         // Enable pull-up/pull-down resistors
  P2OUT |= BUTTONS;         // Set resistors as pull-up
  P2DIR &= ~BUTTONS;        // Set as input
  P2IE |= BUTTONS;          // Enable interrupts
  P2IES |= BUTTONS;         // Interrupt on high-to-low
  P2IFG &= ~BUTTONS;        // Clear flags
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
  WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer // Main
  configureClocks();
  enableWDTInterrupts();
  init_leds();
  init_buzzer();
  init_buttons();

  or_sr(0x18); // GIE + CPU off

  return 0;
}
// Interrupt: Watchdog Timer -> handles current state behavior
void __interrupt_vec(WDT_VECTOR) WDT(){
  switch(current_state){
    case STATE_OFF:
      P1OUT &= ~(LED_RED | LED_GREEN);
      stop_buzzer();
      break;
    case STATE_RED_ON:
      P1OUT |= LED_RED;
      P1OUT &= ~LED_GREEN;
      buzzer_set_period(1000);
      break;
    case STATE_GREEN_ON:
      P1OUT |= LED_GREEN;
      P1OUT &= ~LED_RED;
      buzzer_set_period(2000);
      break;
    case STATE_RED_TOGGLE:
      P1OUT ^= LED_RED;
      buzzer_set_period(1500);
      break;
    case STATE_GREEN_TOGGLE:
      P1OUT ^= LED_GREEN;
      buzzer_set_period(2500);
      break;
  }
}
// Interrupt: Port 2 (buttons)
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & BUTTON1){
    current_state = STATE_RED_ON;
  } else if (P2IFG & BUTTON2){
    current_state = STATE_GREEN_ON;
  } else if (P2IFG & BUTTON3){
    current_state = STATE_RED_TOGGLE;
  } else if (P2IFG & BUTTON4){
    current_state = STATE_GREEN_TOGGLE;
  }

  // Clear all interrupt flags
  P2IFG &= ~BUTTONS;
}
