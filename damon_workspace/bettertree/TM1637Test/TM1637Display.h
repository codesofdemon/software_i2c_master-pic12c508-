#ifndef __TM1637DISPLAY__
#define __TM1637DISPLAY__

#include <inttypes.h>

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000

class TM1637Display {

public:
  //! Initialize a TM1637Display object, setting the clock and
  //! data pins.
  //!
  //! @param pinClk - The number of the digital pin connected to the clock pin of the module
  //! @param pinDIO - The number of the digital pin connected to the DIO pin of the module
  TM1637Display(uint8_t pinClk, uint8_t pinDIO);
  
  //! Sets the brightness of the display.
  //!
  //! The setting takes effect when a command is given to change the data being
  //! displayed.
  //!
  //! @param brightness A number from 0 (lowes brightness) to 7 (highest brightness)
  void setBrightness(uint8_t brightness = 7);
  
  //! Display arbitrary data on the module
  //!
  //! This function receives raw segment values as input and displays them. The segment data
  //! is given as a byte array, each byte corresponding to a single digit. Within each byte,
  //! bit 0 is segment A, bit 1 is segment B etc.
  //! The function may either set the entire display or any desirable part on its own. The first
  //! digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length
  //! argument is the number of digits to be set. Other digits are not affected.
  //!
  //! @param segments An array of size @ref length containing the raw segment values
  //! @param length The number of digits to be modified
  //! @param pos The position from which to start the modification (0 - leftmost, 3 - rightmost)
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
  
  //! Displayes a decimal number
  //!
  //! Dispalyes the given argument as a decimal number
  //!
  //! @param num The number to be shown
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position least significant digit (0 - leftmost, 3 - rightmost)
#if 0
  //it is useless
  void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
#endif
  //! Translate a single digit into 7 segment code
  //!
  //! The method accepts a number between 0 - 15 and converts it to the
  //! code required to display the number on a 7 segment display.
  //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
  //!
  //! @param digit A number between 0 to 15
  //! @return A code representing the 7 segment image of the digit (LSB - segment A;
  //!         bit 6 - segment G; bit 7 - always zero)
  uint8_t encodeDigit(uint8_t digit);
  
  // display a number 
  //if the numer is less than 1000 may you should change the mask
  // e.g. you want to display "789", the "0789" will be display because the mask is 0b1111, you should change 
  // mask to be 0b0111 to dispaly "789"
  void displayNumberByMask(uint16_t num, uint8_t mask = 0b1111);
  // set brightness
  void init();

  void enableColon();
  void disableColon();
  

protected:
   void bitDelay();
   
   void start();
   
   void stop();
   
   bool writeByte(uint8_t b);
   
private:
	uint8_t m_pinClk;
	uint8_t m_pinDIO;
	uint8_t m_brightness;
  uint8_t m_colon;
  uint8_t m_second_segment; // control the colon
};
void displayTM1637(uint8_t pinClk, uint8_t pinDIO, uint16_t number, bool colon_mode, uint8_t mask);
void displayTM1637(uint8_t pinClk, uint8_t pinDIO, uint16_t number, bool colon_mode);
void displayTM1637(uint8_t pinClk, uint8_t pinDIO, uint16_t number);

#endif // __TM1637DISPLAY__
