/*
  DS1307.h - library for DS1307 rtc
*/

// ensure this library description is only included once
#ifndef DS1307_h
#define DS1307_h

#include "stdbool.h"

// include types & constants of Wire ic2 lib
#include "i2c/Wire.h"

#define DS1307_SEC 0
#define DS1307_MIN 1
#define DS1307_HR 2
#define DS1307_DOW 3
#define DS1307_DATE 4
#define DS1307_MTH 5
#define DS1307_YR 6

#define DS1307_BASE_YR 2000

#define DS1307_CTRL_ID 0b1101000  //DS1307

 // Define register bit masks
#define DS1307_CLOCKHALT 0b10000000

#define DS1307_LO_BCD  0b00001111
#define DS1307_HI_BCD  0b11110000

#define DS1307_HI_SEC  0b01110000
#define DS1307_HI_MIN  0b01110000
#define DS1307_HI_HR   0b00110000
#define DS1307_LO_DOW  0b00000111
#define DS1307_HI_DATE 0b00110000
#define DS1307_HI_MTH  0b00110000
#define DS1307_HI_YR   0b11110000

// library interface description
class DS1307
{
  // user-accessible "public" interface
  public:
    DS1307();
    void get(int *, bool);
    int get(int, bool);
	void set(int, int);
    void start(void);
    void stop(void);

  // library-accessible "private" interface
  private:
    uint8_t rtc_bcd[7]; // used prior to read/set ds1307 registers;
	void read(void);
	void save(void);
};

extern DS1307 RTC;

#endif
 

