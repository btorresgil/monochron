/* ***************************************************************************
// dispatch.c - Provides switchting between the different chrons
//
// Pretty tricky, just substitutes for the current routine.
// Slides right into the logic without adjustment, sneaky Mr. Data!
//
// This code is distributed under the GNU Public License
// Which can be found at http://www.gnu.org/licenses/gpl.txt
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
 
#include "ratt.h"
#include "dispatch.h"

// 2010-08-07 Version 1 - Dataman

extern volatile uint8_t displaystyle;
extern volatile uint8_t RotateFlag;
extern volatile uint8_t minute_changed, hour_changed;

void initanim(void){
 switch (displaystyle) {
#ifdef RATTCHRON
 case STYLE_RAT: initanim_rat();
                break;
#endif
#ifdef INTRUDERCHRON
 case STYLE_INT: initanim_int();
				break;
#endif
#ifdef SEVENCHRON
 case STYLE_SEV: initanim_sev();
				break;
#endif
#ifdef XDALICHRON
 case STYLE_XDA: initanim_xda();
                break;
#endif
#ifdef TSCHRON
 case STYLE_TS: initanim_ts();
                break;
#endif
#ifdef MARIOCHRON
 case STYLE_MARIO: initanim_mar();
               break;
#endif
#ifdef DEATHCHRON
 case STYLE_DEATH: initanim_death(); break;
#endif
 case STYLE_ROTATE: RotateFlag = ROTATEPERIOD;	//And fall into STYLE_RANDOM code next.
 case STYLE_RANDOM: init_crand(); displaystyle = (STYLE_BASE + 1) + (crand(0) % (STYLE_RANDOM - (STYLE_BASE + 1))); initanim();
                break;
#ifdef DEATHCHRON
 case STYLE_DEATHCFG: initanim_deathcfg(); break;
#endif
#ifdef GPSENABLE
 case STYLE_GPS: initanim_GPS();
               break;
#endif
 case STYLE_ABOUT: initanim_abo();
                break;
 }
}


void initdisplay(uint8_t inverted) {
 switch (displaystyle) {
#ifdef RATTCHRON
 case STYLE_RAT: initdisplay_rat(inverted);
                break;
#endif
#ifdef INTRUDERCHRON
 case STYLE_INT: initdisplay_int(inverted);
				break;
#endif
#ifdef SEVENCHRON
 case STYLE_SEV: initdisplay_sev(inverted); 
				break;
#endif
#ifdef XDALICHRON
 case STYLE_XDA: initdisplay_xda(inverted);
                 break;
#endif
#ifdef TSCHRON
 case STYLE_TS: initdisplay_ts(inverted);
                break;
#endif
#ifdef MARIOCHRON
 case STYLE_MARIO: initdisplay_mar(inverted);
                break;
#endif
#ifdef DEATHCHRON
 case STYLE_DEATH: initdisplay_death(inverted); break;
#endif
 }
}

void drawdisplay(uint8_t inverted) {
 if (RotateFlag && ((minute_changed==1)||(hour_changed==1))) {
  if(minute_changed)
    minute_changed = 2;
  if(hour_changed)
  	hour_changed = 2;
  if (!--RotateFlag) {
   RotateFlag = ROTATEPERIOD;
   if (++displaystyle==STYLE_RANDOM) {displaystyle=STYLE_BASE+1;}
   initanim();
  }
 }
 switch (displaystyle) {
#ifdef RATTCHRON
 case STYLE_RAT: drawdisplay_rat(inverted);
                break;
#endif
#ifdef INTRUDERCHRON
 case STYLE_INT: drawdisplay_int(inverted);
				break;
#endif
#ifdef SEVENCHRON
 case STYLE_SEV: drawdisplay_sev(inverted); 
 				break;
#endif
#ifdef XDALICHRON
 case STYLE_XDA: drawdisplay_xda(inverted);
                 break;
#endif
#ifdef TSCHRON
 case STYLE_TS: drawdisplay_ts(inverted);
                 break;
#endif
#ifdef MARIOCHRON
 case STYLE_MARIO: drawdisplay_mar(inverted); break;
#endif
#ifdef DEATHCHRON
 case STYLE_DEATH: drawdisplay_death(inverted); break;
#endif
 }
}


void step(void) {
 switch (displaystyle) {
#ifdef RATTCHRON
 case STYLE_RAT: step_rat();
                break;
#endif
#ifdef INTRUDERCHRON
 case STYLE_INT: step_int();
				break;
#endif
#ifdef SEVENCHRON
 case STYLE_SEV: step_sev(); 
 				break;
#endif
#ifdef XDALICHRON
 case STYLE_XDA: step_xda();
                 break;
#endif
#ifdef TSCHRON
 case STYLE_TS: step_ts();
                 break;
#endif
#ifdef MARIOCHRON
// case STYLE_MARIO: step_mario(); break;
#endif
#ifdef DEATHCHRON
 case STYLE_DEATH: step_death(); break;
#endif
 }
}
