#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timerISR.h"
#include "serialATmega.h"

// unsigned char reseter;
//int increment = ADC_read(0);
//int button = PINC;
int i;
int j;
int counter;

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

// //directions[] and outDir() will be neeeded for ex 2 and 3
// int directions[4] = { }; //TODO: copmlete the array containg the values needed for the 7 sgements for each of the 4 directions
// // a  b  c  d  e  f  g
// //TODO: display the direction to the 7-seg display. HINT: will be very similar to outNum()
// void outDir(int dir){
  
// }

// int phases[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001}; //8 phases of the stepper motor step

enum states {INIT, idle_state, increase, decrease} state; //TODO: finish the enum for the SM

void Tick() {

  // State Transistions
  //TODO: complete transitions 
  switch(state) {

    case INIT:
    counter = i; 
    state = idle_state;
      break;

    case idle_state:
    if(ADC_read(0) >= 800){ 
      i++;
      state = increase;
    }
    else if(ADC_read(0) <= 200 ){
      // i = counter;
      i--;
      state = decrease;
    }
    else if(PINC == 0){
      state = INIT;
    }
    else{
      state = idle_state;
    }
      break;
      
    case increase: 
    if(ADC_read(0) < 800){
      counter = i;
      state = idle_state;
    }
      break;

    case decrease:
    if(ADC_read(0) > 200){
      counter = i;
      state = idle_state;
   }
      break;

    // default:
    //   state = INIT;
    //   break;

  }

  // State Actions
  //TODO: complete transitions
  switch(state) {

    case INIT:
    i = 0;
      break;

    case idle_state:
    outNum(i);
      break;

    case increase: 
    if(i > 15){
      i = 0;
    }
      break;

    case decrease:
    if(i < 0){
      i = 15;
    }
     break;

    // default:
    //   break;

  }

}



int main(void)
{
	//TODO: initialize all outputs and inputs
  //  Output: DDR = 1, PORT = 0
  //  Input: DDR = 0, PORT = 1
  DDRC = 0b111000; PORTC = 0b000111;
  DDRB = 0b111111; PORTB = 0b000000;
  DDRD = 0b11111111; PORTD = 0b00000000;

  ADC_init();//initializes the analog to digital converter
  // ADC_read(1);
 
	//add code to check for values for up and down as well as left and right for next parts of lab
  state = INIT;

// serial_init(9600);
  TimerSet(1); //period of 1 ms. good period for the stepper mottor
  TimerOn();


    while(1)
    {
      //outNum(1);
serial_println(ADC_read(0));
		  Tick();      // Execute one synchSM tick
      while (!TimerFlag){}  // Wait for SM period
      TimerFlag = 0;        // Lower flag
     }

    return 0;
}

// int main(void)
// {

//   DDRB = 0xFF; PORTB = 0x00; //sets all of port b as outputs even though we are only using pins 2-5(digital pins 10-13)(stepper motor)

//   DDRC = 0x00; PORTC = 0xFF; //sets all of port c as inputs even though we are only using pin 2(A2)(joystick button)

//   int i = 0;
                                                                                                                                                                                                                                                    



//   while (1)
//   {
//      if((PINC >> 2) & 0x01){//button not pressed
//          PORTB = (PORTB & 0x03) | phases[i] << 2;//& first to reset pins 2-5 but not 0-1 then | with phase shifted left 2 to assign the right value to pins 2-5
//          i++;//increment to next phase
//          if(i>7){ //if all phases are completed, restart
//             i = 0;
//          }
//      }else{
//          PORTB = (PORTB & 0x03) | phases[i] << 2;
//          i--;
//          if(i<0){
//              i = 7;
//          }

//      }
//      _delay_ms(1);//time to wait in each phase. NO NOT USE THIS IN SYNCH STATE MACHINES. USE THE PERIOD OF THE SM INSTEAD
//    }


//    return 0;

// }