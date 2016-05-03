extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "TM1637Display.h"
#include <Arduino.h>

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[32] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // B
  0b00111001,    // C
  0b01000111,    // D
  0b01111001,    // E
  0b01110001,    // F
  0b00000000     //空白   17 - 31
  };

  /*
    Function  : dispaly number the  Nixie light
    parameter :
      @ pinClk      : clock pin
      @ pinDIO      : IO pin
      @ number      : the number you want display
      @ colon_mode  : the mode of Colon
      @ mask        : the number you want to dispay 
     
  */
void displayTM1637(uint8_t pinClk, uint8_t pinDIO, uint16_t number, bool colon_mode, uint8_t mask)
{
  TM1637Display display(pinClk, pinDIO);
  display.setBrightness(0x0f);
  
  if (colon_mode)
  {
    display.enableColon(); 
  }
  else 
  {
    display.disableColon();
  }
  display.displayNumberByMask( number, mask);
}
 /*
    Function  : dispaly number the  Nixie light
    parameter :
      @ pinClk      : clock pin
      @ pinDIO      : IO pin
      @ number      : the number you want display
      @ colon_mode  : the mode of Colon
 */
void displayTM1637(uint8_t pinClk, uint8_t pinDIO, uint16_t number, bool colon_mode)
{
  displayTM1637(pinClk, pinDIO, number, colon_mode, 0b1111);
}
 /*
    Function  : dispaly number the  Nixie light
    parameter :
      @ pinClk      : clock pin
      @ pinDIO      : IO pin
      @ number      : the number you want display
 */
void displayTM1637(uint8_t pinClk, uint8_t pinDIO, uint16_t number)
{
  displayTM1637(pinClk, pinDIO, number, false, 0b1111);
}
  
TM1637Display::TM1637Display(uint8_t pinClk, uint8_t pinDIO)
{
	// Copy the pin numbers
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;
	
	// Set the pin direction and default value.
	// Both pins are set as inputs, allowing the pull-up resistors to pull them up
  pinMode(m_pinClk, INPUT);
  pinMode(m_pinDIO,INPUT);
  digitalWrite(m_pinClk, LOW);
	digitalWrite(m_pinDIO, LOW);
}

void TM1637Display::init()
{
  setBrightness(0x0f);
}

void TM1637Display::setBrightness(uint8_t brightness)
{
	m_brightness = brightness;
}
void TM1637Display::enableColon()
{
  m_colon = 1;
  setSegments(&m_second_segment, 1, 1);
}
void TM1637Display::disableColon()
{
  m_colon = 0;
  setSegments(&m_second_segment, 1, 1);
}
void TM1637Display::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
  
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();
	
	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));
	
	// Write the data bytes
	for (uint8_t k=0; k < length; k++) 
    if (((pos & 0x03) + k == 1) )  //second segment
    {
        writeByte(segments[k] | ( m_colon<<7)  );
        m_second_segment = segments[k];
    }else
    {
	  writeByte(segments[k]);
    }
	  
	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

/* it is useless for us */
//void TM1637Display::showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
//{
//	uint8_t digits[4];
//	const static int divisors[] = { 1, 10, 100, 1000 };
//	bool leading = true;
//	
//	for(int8_t k = 0; k < 4; k++) {
//	  int divisor = divisors[4 - 1 - k];
//		int d = num / divisor;
//		
//		if (d == 0) {
//		  if (leading_zero || !leading || (k == 3))
//		    digits[k] = encodeDigit(d);
//	      else
//		    digits[k] = 0;
//		}
//		else {
//			digits[k] = encodeDigit(d);
//			num -= d * divisor;
//			leading = false;
//		}
//	}
//	
//	setSegments(digits + (4 - length), length, pos);
//}

void TM1637Display::bitDelay()
{
	delayMicroseconds(50);
}
   
void TM1637Display::start()
{
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
}
   
void TM1637Display::stop()
{
	pinMode(m_pinDIO, OUTPUT);
	bitDelay();
	pinMode(m_pinClk, INPUT);
	bitDelay();
	pinMode(m_pinDIO, INPUT);
	bitDelay();
}
  
bool TM1637Display::writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    pinMode(m_pinClk, OUTPUT);
    bitDelay();
    
	// Set data bit
    if (data & 0x01)
      pinMode(m_pinDIO, INPUT);
    else
      pinMode(m_pinDIO, OUTPUT);
    
    bitDelay();
	
	// CLK high
    pinMode(m_pinClk, INPUT);
    bitDelay();
    data = data >> 1;
  }
  
  // Wait for acknowledge
  // CLK to zero
  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO, INPUT);
  bitDelay();
  
  // CLK to high
  pinMode(m_pinClk, INPUT);
  bitDelay();
  uint8_t ack = digitalRead(m_pinDIO);
  if (ack == 0)
    pinMode(m_pinDIO, OUTPUT);
	
	
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  
  return ack;
}

uint8_t TM1637Display::encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x1f];
}

void TM1637Display::displayNumberByMask(uint16_t num, uint8_t mask)
{
  // 0111
  // 1234
  uint8_t data [4] = {0};
  int i;
  for ( i = 3; i >= 0; i--)
  {
    if(mask & (1 << (3-i)))
    {
        data[i] = encodeDigit(num % 10);
      }else
      {
       data[i] = 0b00000000; // nothing
      }
      num /= 10;
    
  } 
  setSegments(data, 4, 0);

}

   

