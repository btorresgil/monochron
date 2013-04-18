/* ***************************************************************************
// anim.c - the main animation and drawing code for MONOCHRON
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
**************************************************************************** */

#include <avr/io.h>      // this contains all the IO port definitions
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 
#include "util.h"
#include "ratt.h"
#include "ks0108.h"
#include "ks0108conf.h"
#include "glcd.h"

#ifdef INTRUDERCHRON
//#include "font5x7.h"

// 2010-03-03 First Version
//            Requires: Patch to glcd.c routine glcdWriteCharGr(u08 grCharIdx)
//                      Font data in fontgr.h
//
// 2010-08-06 Version 2 - Integration into MultiChron
//
// 2012-01-26 Version 3 - Simulates a complete game every minute (by Dan Slage)
//          From http://forums.adafruit.com/viewtopic.php?f=41&t=16751&start=75


#define InvaderTimer 5                    // Smaller to make them move faster

//Routines called by dispatcher
void initanim_int(void);                  // Initialzes Animation
void initdisplay_int(uint8_t);            // Intializes Display
void step_int(void);                      // Moves Invaders
void drawdisplay_int(uint8_t);            // Draws Invaders
void setscore_int(uint8_t);               // Updates Time

//Local Routines
void WriteInvaders_int(uint8_t);          // Displays Invaders
void WriteBases_int(uint8_t);             // Displays Bases
void WriteTime_int(uint8_t);              // Displays Time             
void WriteDigits_int(uint8_t, uint8_t);   // Displays a Set of Digits


uint8_t pInvaders=1;                  // Invader's X Position
uint8_t pInvadersPrevious=0;          // Previous Invader's Position
int8_t  pInvadersDirection=1;         // Invader's Direction 1=Right, -1=Left
uint8_t pGun=1;		                  // Gun's X Position
uint8_t pBullet=1;		              // Bullet's X Position
uint8_t Frame=0;                      // Current Animation Frame 
uint8_t Timer = InvaderTimer;         // Count down timer so they don't move rediculously fast
uint8_t left_score, right_score;      // Store For score
uint8_t left_score2, right_score2;    // Storage for player2 score
uint8_t rows, cols, prevCols;			// DS Number of invader rows and columns
uint8_t deadInvaderFrame=8;				// DS Dead invader Animation Frame 
uint8_t bulletFrame=8;				// DS Bullet Animation Frame 
uint8_t divideByTwo[10] = {0,0,1,1,2,2,3,3,4,4}; //Used instead of division to divide by 2

extern volatile uint8_t time_s, time_m, time_h;
extern volatile uint8_t old_m, old_h;
extern volatile uint8_t date_m, date_d, date_y;
extern volatile uint8_t alarming, alarm_h, alarm_m;
extern volatile uint8_t time_format;
extern volatile uint8_t region;
extern volatile uint8_t score_mode;
extern volatile uint8_t second_changed, minute_changed, hour_changed;

uint8_t digitsmutex_int = 0;
uint8_t last_score_mode2 = 0;
uint8_t wasalarming = 0; // flag to indicate resetting bases from reverse video is required
void initanim_int(void) {
#ifdef DEBUGF
  DEBUG(putstring("screen width: "));
  DEBUG(uart_putw_dec(GLCD_XPIXELS));
  DEBUG(putstring("\n\rscreen height: "));
  DEBUG(uart_putw_dec(GLCD_YPIXELS));
  DEBUG(putstring_nl(""));
#endif
  pInvaders = 1;
  pInvadersPrevious=0;
  initdisplay_int(0);
 }

void initdisplay_int(uint8_t inverted) {
  // clear screen
  glcdFillRectangle(0, 0, GLCD_XPIXELS, GLCD_YPIXELS, inverted);
  // get time & display
  last_score_mode2 = 99;
  setscore_int(inverted);
  WriteTime_int(inverted);
  // display players 
  WriteInvaders_int(inverted);
  // Show the bases, 1 time only
  WriteBases_int(inverted);
}

void step_int(void) {
 if (--Timer==0) 
  {
  Timer=InvaderTimer;
  pInvadersPrevious = pInvaders;
  pInvaders += pInvadersDirection;
  if (pInvaders > 47) {pInvadersDirection=-1;}
  if (pInvaders < 1) {pInvadersDirection=1;}
  Frame = !Frame;
  if (deadInvaderFrame < 8) {deadInvaderFrame++;} //increment frame

  /* DS Splits the time value into high and low digits. */
  rows = 0;
  cols = time_s; //DS change this to time_m if you want the game to last an hour
  while (cols >= 10) {                // Count tens
        rows++;
        cols -= 10;
    }
  rows = 6 - rows; // range is changed to 6 to 1 instead of 0 to 5 
  cols = 5 - divideByTwo[cols]; //range is changed to 5 to 0 instead of 0 to 10
  }
   if (prevCols != cols) { //invader count has changed
	prevCols = cols;
	deadInvaderFrame = 0;
	bulletFrame = 0;
	pBullet = pGun+5;
   }
 }

void drawdisplay_int(uint8_t inverted) {
    WriteInvaders_int(inverted);
    if (alarming || wasalarming) {
     WriteBases_int(inverted);
     wasalarming=alarming;
    }
    setscore_int(inverted);
    return;
}

void setscore_int(uint8_t inverted) {
   if (minute_changed || hour_changed || last_score_mode2 != score_mode) {
   	   minute_changed = hour_changed = 0;
   if (! digitsmutex_int) {
    digitsmutex_int++;
    last_score_mode2 = score_mode;
    right_score = hours(time_h);
    right_score2 = time_m;
    if (score_mode == SCORE_MODE_ALARM) {
     left_score = hours(alarm_h);
     left_score2 = alarm_m;
    } 
    else if (score_mode == SCORE_MODE_DATE) {
      left_score = 20;
      left_score2 = date_y;
    } 
    else if (region == REGION_US) {
     left_score = date_m;
     left_score2 = date_d;
    } 
    else {
     left_score = date_d;
     left_score2 = date_m;
    }
    digitsmutex_int--;
    WriteTime_int(inverted);
	// Refresh bases
	WriteBases_int(inverted);
   }
  }
}

//DS Moved invaders to top. Added gun, explosions, etc.
void WriteInvaders_int(uint8_t inverted) {
  uint8_t j;
  uint8_t i;
  
  // Clear above Top row
  glcdFillRectangle(0, 0, 127, (6-rows)*8, inverted);
  
  // Draw Current
  for (i=0;i<rows;i++){ // i is the row and the invader type
   for (j=0;j<5;j++){ //j is column
    glcdSetAddress(pInvaders + (j*16), 6-rows+i);	//DS
    if (i >= (rows-1) && j >= cols-1) {break;} 
	else {glcdWriteCharGr(FontGr_INTRUDER_TRIANGLE_UP+(divideByTwo[i])+(Frame*3),inverted);}
   }
  }
  // Draw exploding invader
  if (deadInvaderFrame < 3) {
	if (deadInvaderFrame < 1) {glcdWriteCharGr(FontGr_INTRUDER_TRIANGLE_UP+(divideByTwo[rows-1])+(Frame*3),inverted);}
	else {glcdWriteCharGr(FontGr_INTRUDER_CIRCLE_DOWN+deadInvaderFrame,inverted);}
  }

  //Draw gun
  if (cols==1) {j=4;}
  else {j = cols-2;}
  i = pInvaders + (j*16);
  if (pGun > i) {
	if (pGun-i > 4) {pGun-=1;}
	}
  else {pGun+=3;}
  glcdSetAddress(pGun, 7);
  glcdWriteCharGr(FontGr_INTRUDER_GUN,inverted);

  //Draw Bullet
  if (bulletFrame < 4) {
	glcdSetAddress(pBullet, 6);
	glcdWriteCharGr(FontGr_INTRUDER_BULLET_0+bulletFrame,inverted);
	bulletFrame++; //increment to next frame
  }
}

void WriteBases_int(uint8_t inverted) {
  for (uint8_t i=0;i<3;i++) {
    glcdSetAddress(33 + (i*24), 6);	//DS
    glcdWriteCharGr(FontGr_INTRUDER_BASE,inverted);
   }
}

void WriteTime_int(uint8_t inverted) {
 	 glcdSetAddress(0,6);	 //DS
	 printnumber(left_score,inverted);
     printnumber(left_score2,inverted);
     glcdSetAddress(102,6);	 //DS
	 printnumber(right_score,inverted);
	 printnumber(right_score2,inverted);
}

#endif
