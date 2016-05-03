#include <Arduino.h>
#include "TM1637Display.h"

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 2

const uint8_t SEG_DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};
void asdfasdf()
{
  noTone(1);  
}

void setup()
{
   
}

void loop()
{
  displayTM1637( CLK,  DIO, 1111, false);
  delay(300);
  displayTM1637( CLK,  DIO, 2222, false);
  delay(300);
}
 
