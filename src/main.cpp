#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timerISR.h"

unsigned char reseter;
unsigned char increment;
unsigned char i;

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
   return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
              //   Set bit to 1           Set bit to 0
}

unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}

void ADC_init() {
  ADMUX = (1<<REFS0);
	ADCSRA|= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}



unsigned int ADC_read(unsigned char chnl){
  uint8_t low, high;

  ADMUX  = (ADMUX & 0xF8) | (chnl & 7);
  ADCSRA |= 1 << ADSC ;
  while((ADCSRA >> ADSC)&0x01){}


	low  = ADCL;
	high = ADCH;

	return ((high << 8) | low) ;
}


int nums[16] = {0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011, 0b1110111, 0b0011111, 0b1001110, 0b0111101, 0b1001111, 0b1000111 }; 
// a  b  c  d  e  f  g
void outNum(int num){
	PORTD = nums[num] << 1; //assigns bits 1-7 of nums(a-f) to pins 2-7 of port d
  PORTB = SetBit(PORTB, 0 ,nums[num]&0x01); // assigns bit 0 of nums(g) to pin 0 of port b
}

//directions[] and outDir() will be neeeded for ex 2 and 3
int directions[4] = { }; //TODO: copmlete the array containg the values needed for the 7 sgements for each of the 4 directions
// a  b  c  d  e  f  g
//TODO: display the direction to the 7-seg display. HINT: will be very similar to outNum()
void outDir(int dir){
  
}

int phases[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001}; //8 phases of the stepper motor step

enum states {INIT, idle_state, increase, decrease} state; //TODO: finish the enum for the SM


void Tick() {

  // State Transistions
  //TODO: complete transitions 
  switch(state) {

    case INIT:
    state = idle_state;
    i = 0;
      break;

    case idle_state:
    if(A1){
      state = increase;
    }
    else if(A1){
      state = decrease;
    }
    else if(A2){
      state = INIT;
    }

      break;

    case increase:
    state = idle_state;
      break;

    case decrease:
    state = idle_state;
      break;


    default:
      state = INIT;
      break;

  }

  // State Actions
  //TODO: complete transitions
  switch(state) {

    case INIT:
      break;

    case idle_state:
      break;

    case increase:
      break;

    case decrease:
      break;

    default:
      break;

  }

}



int main(void)
{
	//TODO: initialize all outputs and inputs


  ADC_init();//initializes the analog to digital converter
  increment = ADC_read(0);
  reseter = ADC_read(2);
	
  state = INIT;

  TimerSet(1); //period of 1 ms. good period for the stepper mottor
  TimerOn();


    while (1)
    {

		  Tick();      // Execute one synchSM tick
      while (!TimerFlag){}  // Wait for SM period
      TimerFlag = 0;        // Lower flag
     }

    return 0;
}