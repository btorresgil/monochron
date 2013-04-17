#include <avr/io.h> 
#include <avr/pgmspace.h>

#ifndef PGM_BOOTLOADER
  #include <avr/eeprom.h>
#else
  #define EEMEM PROGMEM
//Code to provide OptiBoot compatibility.
//For this to work, since Optiboot does not support programming EEPROM,
//we have to have a seperate loader in order to accomplish that task.
//Anyone using the stock Arduino bootloader, can program the .eep file
//as for anyone doing direct ISP programming.
//
//Optiboot is currently not compatible with this hardware, but if and when it
//does, we would like to be able to use it in the future.
#endif

#include "ratt.h"
#ifdef DEATHCHRON
  #include "deathclock.h"
#endif

//DO NOT set EE_INITIALIZED + EE_VERSION to 0xFF / 255,  as that is
//the state the eeprom will be in, when totally erased.
//If we add/delete stuff from here, Change the EE_VERSION define.
//The clocks firmware will beep 3 times then pause repeatedly, if the eeprom
//is not initialized.
#define EE_INITIALIZED 0xC3
#define EE_VERSION 2

uint8_t PROGMEM EE_DATA[1] = { (uint8_t)(EE_INITIALIZED + EE_VERSION + STYLE_ABOUT) };
uint8_t EEMEM EE_INIT=(uint8_t)(EE_INITIALIZED + EE_VERSION + STYLE_ABOUT);
uint8_t EEMEM  EE_ALARM_HOUR=7;
uint8_t EEMEM EE_ALARM_MIN=30;
uint8_t EEMEM EE_BRIGHT=OCR2A_VALUE;
uint8_t EEMEM EE_REGION=REGION_US;
uint8_t EEMEM EE_TIME_FORMAT=TIME_12H;
uint8_t EEMEM EE_SNOOZE=10;
uint8_t EEMEM EE_STYLE=STYLE_RANDOM;
#ifdef DEATHCHRON
  uint8_t EEMEM EE_DOB_MONTH = 11; //Death Clock variables are preserved in the event of an extended power outage.
  uint8_t EEMEM EE_DOB_DAY = 14;
  uint8_t EEMEM EE_DOB_YEAR = 80;
  uint8_t EEMEM EE_SET_MONTH = 7;
  uint8_t EEMEM EE_SET_DAY = 28;
  uint8_t EEMEM EE_SET_YEAR = 110;
  uint8_t EEMEM EE_GENDER = DC_gender_male;
  uint8_t EEMEM EE_DC_MODE = DC_mode_normal;
  uint8_t EEMEM EE_BMI_UNIT = BMI_Imperial;
  uint16_t EEMEM EE_BMI_WEIGHT = 400;
  uint16_t EEMEM EE_BMI_HEIGHT = 78;
  uint8_t EEMEM EE_SMOKER = DC_non_smoker;
  uint8_t EEMEM EE_SET_HOUR = 20;
  uint8_t EEMEM EE_SET_MIN = 05;
  uint8_t EEMEM EE_SET_SEC = 25;
#endif
#ifdef GPSENABLE
  uint8_t EEMEM EE_TIMEZONE=-32;	//Both CaitSith2 and Dataman reside at timezone -8:00. :)
#endif

#ifdef RATTDEATH
unsigned char EEMEM BigFont[] = {
	0xFF, 0x81, 0x81, 0xFF,// 0
	0x00, 0x00, 0x00, 0xFF,// 1
	0x9F, 0x91, 0x91, 0xF1,// 2
	0x91, 0x91, 0x91, 0xFF,// 3
	0xF0, 0x10, 0x10, 0xFF,// 4
	0xF1, 0x91, 0x91, 0x9F,// 5
	0xFF, 0x91, 0x91, 0x9F,// 6
	0x80, 0x80, 0x80, 0xFF,// 7
	0xFF, 0x91, 0x91, 0xFF,// 8 
	0xF1, 0x91, 0x91, 0xFF,// 9
#ifdef DEATHCHRON
    0x00, 0x66, 0x66, 0x00,// :
    0x00, 0x18, 0x18, 0x00,// -
#endif
	0x00, 0x00, 0x00, 0x00,// SPACE
#ifdef DEATHCHRON
    0xFF, 0x90, 0x90, 0xFF,// A
    0xFF, 0x90, 0x90, 0xF0,// P
    0x9F, 0x90, 0x90, 0x9F,// M
    0x00, 0x60, 0x60, 0x00,// High . of :
    0x00, 0x06, 0x06, 0x00,// Low . of :
#endif
};
#endif

unsigned char EEMEM Font5x7[] = {
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)
	0x00, 0x00, 0x5F, 0x00, 0x00,// !
	0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
	0x23, 0x13, 0x08, 0x64, 0x62,// %
	0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x1C, 0x22, 0x41, 0x00,// (
	0x00, 0x41, 0x22, 0x1C, 0x00,// )
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
	0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x50, 0x30, 0x00, 0x00,// ,
	0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41,// <
	0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x41, 0x22, 0x14, 0x08, 0x00,// >
	0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A
	0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x7F, 0x09, 0x09, 0x01, 0x01,// F
	0x3E, 0x41, 0x41, 0x51, 0x32,// G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W
	0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x03, 0x04, 0x78, 0x04, 0x03,// Y
	0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41,// [
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]
	0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x01, 0x02, 0x04, 0x00,// `
	0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x08, 0x14, 0x54, 0x54, 0x3C,// g
	0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x20, 0x40, 0x44, 0x3D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44,// k
	0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x7C, 0x14, 0x14, 0x14, 0x08,// p
	0x08, 0x14, 0x14, 0x18, 0x7C,// q
	0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y
	0x44, 0x64, 0x54, 0x4C, 0x44,// z
	0x00, 0x08, 0x36, 0x41, 0x00,// {
	0x00, 0x00, 0x7F, 0x00, 0x00,// |
	0x00, 0x41, 0x36, 0x08, 0x00,// }
	0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};



unsigned char PROGMEM FontGr[] =
{
// format is one character per line:
// length, byte array[length]
//0x0B,0x3E,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x3C,0x00,// 0. Folder Icon
//0x06,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF		       	// 1. Solid 6x8 block
#ifdef INTRUDERCHRON
16, 0, 0, 88, 188, 28, 22, 22, 63, 63, 22, 22, 28, 188, 88, 0, 0, // 0 = Triangle Up
16, 0, 0, 30, 184, 125, 54, 60, 60, 60, 60, 54, 125, 184, 30, 0, 0,// 1 = Square Up
16, 0, 0, 156, 158, 94, 118, 55, 95, 95, 55, 118, 94, 158, 156, 0, 0, // 2 = Circle Up
16, 0, 0, 24, 28, 156, 86, 182, 95, 95, 182, 86, 156, 28, 24, 0, 0, // 3 = Triangle Down
16, 0, 0, 112, 24, 125, 182, 188, 60, 60, 188, 182, 125, 24, 112, 0, 0, // 4 = Square Down
16, 0, 0, 28, 94, 254, 182, 55, 95, 95, 55, 182, 254, 94, 28, 0, 0, // 5 = Circle Down
16, 0, 0, 0x81, 0x5A, 0x24, 0x5A, 0xA5, 0x5A, 0x5A, 0xA5, 0x5A, 0x24, 0x5A, 0x81, 0, 0, // 6 = Explode
16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7 = Clear
16, 0, 0, 0, 0xE0, 0xE0, 0xF0, 0xFE, 0xFE, 0xF0, 0xE0, 0xE0, 0, 0, 0, 0, 0, //8 = Gun
12, 248, 252, 254, 254, 63, 31, 31, 63, 254, 254, 252, 248,  // 9 = Base
1, 0xC0,   // 10 = Bullet Frame 0
1, 0x3E,   // 11 = Bullet Frame 1
1, 0x07,   // 12 = Bullet Frame 2
1, 0x00    // 13 = Bullet Clear
#endif
};

#ifdef SEVENCHRON
uint8_t EEMEM alphatable[] = {
	0xFA, /* a */
	0x3E, /* b */
	0x1A, /* c */
	0x7A, /* d */
	0xDE, /* e */
	0x8E, /* f */
	0xF6, /* g */
	0x2E, /* h */
	0x60, /* i */
	0x78, /* j */
	0xAE, //k
	0x1C, // l
	0xAA, // m
	0x2A, // n
	0x3A, // o
	0xCE, //p
	0xF3, // q
	0x0A, //r
	0xB6, //s
	0x1E, //t
	0x38, //u
	0x38, //v // fix?
	0xB8, //w
	0x6E, //x
	0x76, // y
	0xDA, //z
	/* more */
};
#endif

#ifdef NUMBERTABLE
uint8_t EEMEM numbertable[] = { 
  0xFC /* 0 */, 
  0x60 /* 1 */,
  0xDA /* 2 */,
  0xF2 /* 3 */,
  0x66 /* 4 */,
  0xB6 /* 5 */,
  0xBE, /* 6 */
  0xE0, /* 7 */
  0xFE, /* 8 */
#ifdef FEATURE_9
  // Normal 7-segment "9" digit looks the same as letter "g".
  // "This notation is ambiguous but the meaning will be clear in context." - Hungerford, "Algebra"
  0xF6, /* 9 */
#else
  // ladyada's version of "9" is non-standard but distinct from letter "g".
  0xE6, /* 9 */
#endif
};
//#ifdef SEVENCHRON
#endif

#ifdef OPTION_DOW_DATELONG
uint8_t DOWText[] EEMEM = "sunmontuewedthufrisat";
uint8_t MonthText[] EEMEM = "   janfebmaraprmayjunjulaugsepoctnovdec";
#endif

uint8_t about[] EEMEM =      "\0\0\0\0\0\0\0\0"
	                              // 123456789ABCDEF0123456
	                         "\x0a" "MultiChron"
                                  // 123456789ABCDEF0123456
                             "\x0b" "Version 1.1"
                                  // 123456789ABCDEF0123456
                             "\x00"
                                  // 123456789ABCDEF0123456
                             "\x0a" "MultiChron"
                                  // 123456789ABCDEF0123456
                         #ifdef INTRUDERCHRON
	                         "\x0d" "IntruderChron" 
	                     #endif
                         #ifdef TSCHRON
	                         "\x10" "TimesSquareChron" 
	                     #endif
                                  // 123456789ABCDEF0123456
	                         "\x0a" "by Dataman"
                                  // 123456789ABCDEF0123456
	                         "\x0d" "Data Magician"
	                       #ifndef OPTION_DOW_DATELONG
	                         "\x12" "http://crjones.com"	//These lines don't fit with OPTION_DOW_DATELONG :(
	                       #endif
                                  // 123456789ABCDEF0123456
                             "\x00"
                           #ifdef DEATHCHRON
                             "\x0a" "DeathChron"
                           #endif
                                  // 123456789ABCDEF0123456
                             "\x0c" "Optimization"
                                  // 123456789ABCDEF0123456
	                         "\x09" "Debugging"
                                  // 123456789ABCDEF0123456
                             "\x0c" "by CaitSith2"
                                  // 123456789ABCDEF0123456
                             "\x0a" "Code Jedi!"
                            #ifndef OPTION_DOW_DATELONG
                             "\x14" "http://caitsith2.com"
                            #endif
                                  // 123456789ABCDEF0123456
                             "\x00"
                                  // 123456789ABCDEF0123456
                         #ifdef RATTCHRON
	                         "\x09" "RATTChron"
	                     #endif
	                     #ifdef SEVENCHRON
                                  // 123456789ABCDEF0123456
	                         "\x0a" "SevenChron"
	                     #endif
	                     #ifdef XDALICHRON
                                  // 123456789ABCDEF0123456
                             "\x0b" "XADALICHRON"
                         #endif
                                  // 123456789ABCDEF0123456
                             "\x12" "MonoChron Hardware"
                                  // 123456789ABCDEF0123456
			                 "\x0a" "by LadyAda"
                                  // 123456789ABCDEF0123456
			                 "\x10" "Simply The Best!"
                                  // 123456789ABCDEF0123456
                             "\x00"
                         #ifdef DEATHCHRON
                             "\x05" "Skull"
                             "\x09" "Tombstone"
                             "\x0F" "The Grim Reaper"
                             "\x11" "The Adafruit logo"
                             "\x12" "by Phillip Torrone"
                             "\x0C" "Very Awesome"
                             "\x00"
                         #endif
                                  // 123456789ABCDEF0123456
	                         "\x13" "Adafruit Industries" 
                                  // 123456789ABCDEF0123456
                             "\x10" "www.adafruit.com"
                                  // 123456789ABCDEF0123456
                             "\0\0" "\xff";
