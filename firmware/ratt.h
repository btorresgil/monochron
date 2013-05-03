#define halt(x)  while (1)

#define DEBUGGING 0
#define DEBUG(x)  if (DEBUGGING) { x; }
#define DEBUGP(x) DEBUG(putstring_nl(x))

// Software options. Uncomment to enable.
//BACKLIGHT_ADJUST - Allows software control of backlight, assuming you mounted your 100ohm resistor in R2'.
#define BACKLIGHT_ADJUST 1

//OPTION_DOW_DATELONG - Allows showing Day of Week, and Longer format Dates, 
//Like " sat","0807","2010", or " aug","  07","2010" or " sat"," aug","  07","2010".
//#define OPTION_DOW_DATELONG 1

// Undefine to disable GPS
//#define GPSENABLE 1

// Undefine the following to disable these clock modes
//#define INTRUDERCHRON 1
//#define SEVENCHRON 1
//#define RATTCHRON 1
//#define XDALICHRON 1
//#define TSCHRON 1
//#define DEATHCHRON 1

//#define MULTICHRONA 1
//#define MULTICHRONB 1
//MULTICHRONA / MULTICHRONB defined by the make files.

#ifdef MULTICHRONA
//MultiCronA Mix:
#define GPSENABLE 1
#define INTRUDERCHRON 1
#define SEVENCHRON 1
#define RATTCHRON 1
#define XDALICHRON 1
#define TSCHRON 1
#define MARIOCHRON 1
#else
//MultiCronB DeathChron Mix:
#define GPSENABLE 1
#define INTRUDERCHRON 1
#define SEVENCHRON 1
#define RATTCHRON 1
#define TSCHRON 1
#define MARIOCHRON 1
#define DEATHCHRON 1
#endif





// Undefine to use Lady A's disambiguified 9 (vs g). (SEVENCHRON, TSCHRON)
#define FEATURE_9 1

#ifndef XDALICHRON
	//The compiler is feeding me bullshit, about STYLE_XDA being undefined in ratt.c, 
	//despite there being no references to STYLE_XDA in ratt.c
	#define	STYLE_XDA 1
#endif
	
#ifdef SEVENCHRON
  #define NUMBERTABLE 1
#endif
#ifdef TSCHRON
  #ifndef NUMBERTABLE
    #define NUMBERTABLE 1
  #endif
#endif

#ifdef RATTCHRON
  #define RATTDEATH 1
#endif
#ifdef DEATHCHRON
  #ifndef RATTDEATH
    #define RATTDEATH 1
  #endif
#endif

// This is a tradeoff between sluggish and too fast to see
//#define MAX_BALL_SPEED 5 // note this is in vector arith.
#define MAX_BALL_SPEED (5*FIXED_MATH) //fixed point math rewrite. We are keeping 2 significant places, so real speed is 5.00.
#define BALL_RADIUS 2 // in pixels

// If the angle is too shallow or too narrow, the game is boring
//#define MIN_BALL_ANGLE 20
//Fixed point math rewrite. //Our sin/cos table is 1/4 of a 256 degree circle table.  As such
#define MIN_BALL_ANGLE 15 //Minimum angle of 20 in 360, is 15 in 256.


// how fast to proceed the animation, note that the redrawing
// takes some time too so you dont want this too small or itll
// 'hiccup' and appear jittery
#define ANIMTICK_MS 75

// Beeep!
#define ALARM_FREQ 4000
#define ALARM_MSONOFF 300
 
#define MAXSNOOZE 600 // 10 minutes

// how many seconds we will wait before turning off menus
#define INACTIVITYTIMEOUT 10 

/*************************** DISPLAY PARAMETERS */

// how many pixels to indent the menu items
#define MENU_INDENT 8

#define DIGIT_W 32
#define HSEGMENT_H 6
#define HSEGMENT_W 18
#define VSEGMENT_H 25
#define VSEGMENT_W 6
#define DIGITSPACING_SEV 4
#define DIGITSPACING_XDA 3
#define DOTRADIUS 4

#define DISPLAY_H10_X_SEV  0
#define DISPLAY_H1_X_SEV  VSEGMENT_W + HSEGMENT_W + 2 + DIGITSPACING_SEV
#define DISPLAY_M10_X_SEV  GLCD_XPIXELS - 2*VSEGMENT_W - 2*HSEGMENT_W - 4 - DIGITSPACING_SEV
#define DISPLAY_M1_X_SEV  GLCD_XPIXELS - VSEGMENT_W - HSEGMENT_W - 2
#define DISPLAY_TIME_Y_SEV 0

#define DISPLAY_H10_X_XDA  0
#define DISPLAY_H1_X_XDA  DIGIT_WIDTH + DIGITSPACING_XDA
#define DISPLAY_M10_X_XDA  GLCD_XPIXELS - 2*DIGIT_WIDTH - DIGITSPACING_XDA
#define DISPLAY_M1_X_XDA  GLCD_XPIXELS - DIGIT_WIDTH
#define DISPLAY_TIME_Y_XDA 0


#define DISPLAY_H10_X_RAT 30
#define DISPLAY_H1_X_RAT 45
#define DISPLAY_M10_X_RAT 70
#define DISPLAY_M1_X_RAT 85
#define DISPLAY_TIME_Y_RAT 4

#define DIGIT_PIXEL_SIZE_Y_DEATH 5

#define DISPLAY_H10_X_DEATH 0 + 5
#define DISPLAY_H1_X_DEATH 15 + 5
#define DISPLAY_HM_X_DEATH 30 + 5
#define DISPLAY_M10_X_DEATH 45 + 5
#define DISPLAY_M1_X_DEATH 60 + 5
#define DISPLAY_MS_X_DEATH 75 + 5
#define DISPLAY_S10_X_DEATH 90 + 5
#define DISPLAY_S1_X_DEATH 105 + 5

#define DISPLAY_DL1_X_DEATH 0 + 5
#define DISPLAY_DL2_X_DEATH 15 + 5
#define DISPLAY_DL3_X_DEATH 30 + 5
#define DISPLAY_DL4_X_DEATH 45 + 5
#define DISPLAY_DR1_X_DEATH 60 + 5
#define DISPLAY_DR2_X_DEATH 75 + 5
#define DISPLAY_DR3_X_DEATH 90 + 5
#define DISPLAY_DR4_X_DEATH 105 + 5



// buffer space from the top
#define DISPLAY_TIME_Y_DEATH 12

// how big are the pixels (for math purposes)
#define DISPLAY_DIGITW_DEATH 16
#define DISPLAY_DIGITH_DEATH 40

/* not used
#define ALARMBOX_X 20
#define ALARMBOX_Y 24
#define ALARMBOX_W 80
#define ALARMBOX_H 20
*/

/*************************** PIN DEFINITIONS */
// there's more in KS0108.h for the display

#define ALARM_DDR DDRB
#define ALARM_PIN PINB
#define ALARM_PORT PORTB
#define ALARM 6

#define PIEZO_PORT PORTC
#define PIEZO_PIN  PINC
#define PIEZO_DDR DDRC
#define PIEZO 3


/*************************** ENUMS */

// Whether we are displaying time (99% of the time)
// alarm (for a few sec when alarm switch is flipped)
// date/year (for a few sec when + is pressed)
enum {
	SCORE_MODE_TIME=0,
	SCORE_MODE_DATE,
	SCORE_MODE_YEAR,
	SCORE_MODE_ALARM,
#ifdef DEATHCHRON
	SCORE_MODE_DEATH_TIME,
	SCORE_MODE_DEATH_DATE,
	SCORE_MODE_DEATH_YEAR,
	SCORE_MODE_DEATH_ALARM,
#endif
#ifdef OPTION_DOW_DATELONG
	SCORE_MODE_DOW,
	SCORE_MODE_DATELONG_MON,
	SCORE_MODE_DATELONG_DAY,
#endif
};

#define SCORE_MODE_TIMEOUT ((displaystyle == STYLE_XDA)?5:3)

// Constants for how to display time & date
enum {
	REGION_US = 0, 
	REGION_EU,
	DOW_REGION_US, 
	DOW_REGION_EU,
	DATELONG, 
	DATELONG_DOW,
};
enum {
	TIME_12H = 0, TIME_24H,
};


//Contstants for calcualting the Timer2 interrupt return rate.
//Desired rate, is to have the i2ctime read out about 6 times
//a second, and a few other values about once a second.
#define OCR2B_BITSHIFT 0
#define OCR2B_PLUS 1
#define OCR2A_VALUE 16
#define TIMER2_RETURN 80
//#define TIMER2_RETURN (8000000 / (OCR2A_VALUE * 1024 * 6))

// displaymode
enum {
	NONE = 99,
	CFG_MENU = 98,
	SHOW_TIME = 0,
	SHOW_DATE,
	SHOW_ALARM,
	SET_TIME,
	SET_ALARM,
	SET_DATE,
	SET_BRIGHTNESS,
	SET_VOLUME,
	SET_REGION,
	SHOW_SNOOZE,
	SET_SNOOZE,
#ifdef DEATHCHRON
	SET_DEATHCLOCK_DOB,
	SET_DEATHCLOCK_GENDER,
	SET_DEATHCLOCK_MODE,
	SET_DEATHCLOCK_BMI,
	SET_DEATHCLOCK_SMOKER,
#endif
	SET_MONTH,
	SET_DAY,
	SET_YEAR,
	SET_HOUR = 101,
	SET_MIN,
	SET_SEC,
	SET_REG,
	SET_BRT,
#ifdef DEATHCHRON
	SET_DCGENDER,
	SET_DCMODE,
	SET_BMI_UNIT,
	SET_BMI_WT,
	SET_BMI_HT,
	SET_DCSMOKER,
#endif
	SET_STYLE = 200,
	SET_STL,
};



#ifdef GPSENABLE
 #define TIMEZONEHOUR abs(timezone)>>2
 #define TIMEZONEMIN (abs(timezone)&3)*15
#endif

enum {
	STYLE_BASE = 209,
#ifdef INTRUDERCHRON
	STYLE_INT,
#endif
#ifdef SEVENCHRON
	STYLE_SEV,
#endif
#ifdef RATTCHRON
	STYLE_RAT,
#endif
#ifdef XDALICHRON
	STYLE_XDA,
#endif
#ifdef TSCHRON
	STYLE_TS,
#endif
#ifdef MARIOCHRON
  STYLE_MARIO,
#endif
#ifdef DEATHCHRON
	STYLE_DEATH,
#endif
	STYLE_RANDOM,
	STYLE_ROTATE,
#ifdef DEATHCHRON	//DeathChron needs its own configuration menu.
	STYLE_DEATHCFG,
#endif
#ifdef GPSENABLE
	STYLE_GPS,
#endif
	STYLE_ABOUT
};


//FontGr enums - if we add other chrons in the future, using the FontGr array, that is
//defined in eeprom.c, we need to add an enum for each entry in that array.
enum {
	FontGr_BASE = -1,
#ifdef INTRUDERCHRON
	FontGr_INTRUDER_TRIANGLE_UP,
	FontGr_INTRUDER_SQUARE_UP,
	FontGr_INTRUEDER_CIRCLE_UP,
	FontGr_INTRUDER_TRIANGLE_DOWN,
	FontGr_INTRUDER_SQUARE_DOWN,
	FontGr_INTRUDER_CIRCLE_DOWN,
	FontGr_INTRUDER_EXPLODE,	
	FontGr_INTRUDER_CLEAR,	
	FontGr_INTRUDER_GUN,		
	FontGr_INTRUDER_BASE,
	FontGr_INTRUDER_BULLET_0,
	FontGr_INTRUDER_BULLET_1,
	FontGr_INTRUDER_BULLET_2,
	FontGr_INTRUDER_BULLET_Clear,		
#endif
};
	


// ROTATEPERIOD is the the wait period, in minutes, between screen rotations.
#define ROTATEPERIOD 15 

/*
#define EE_INIT 0
#define EE_ALARM_HOUR 1
#define EE_ALARM_MIN 2
#define EE_BRIGHT 3
#define EE_VOLUME 4
#define EE_REGION 5
#define EE_TIME_FORMAT 6
#define EE_SNOOZE 7*/

extern uint8_t EE_DATA[];
extern uint8_t EE_INIT;
extern uint8_t EE_ALARM_HOUR;
extern uint8_t EE_ALARM_MIN;
extern uint8_t EE_BRIGHT;
extern uint8_t EE_REGION;
extern uint8_t EE_TIME_FORMAT;
extern uint8_t EE_SNOOZE;
extern uint8_t EE_STYLE;
extern uint8_t EE_TIMEZONE;

#ifdef DEATHCHRON
extern uint8_t EE_DOB_MONTH; //Death Clock variables are preserved in the event of an extended power outage.
extern uint8_t EE_DOB_DAY;
extern uint8_t EE_DOB_YEAR;
extern uint8_t EE_SET_MONTH;
extern uint8_t EE_SET_DAY;
extern uint8_t EE_SET_YEAR;
extern uint8_t EE_GENDER;
extern uint8_t EE_DC_MODE;
extern uint8_t EE_BMI_UNIT;
extern uint16_t EE_BMI_WEIGHT;
extern uint16_t EE_BMI_HEIGHT;
extern uint8_t EE_SMOKER;
extern uint8_t EE_SET_HOUR;
extern uint8_t EE_SET_MIN;
extern uint8_t EE_SET_SEC;
#endif

/*************************** FUNCTION PROTOTYPES */

uint8_t leapyear(uint16_t y);
void clock_init(void);
void initbuttons(void);
void tick(void);
void setsnooze(void);
void initanim(void);
void initdisplay(uint8_t inverted);
void step(void);
void setscore(void);
void draw(uint8_t inverted);

void set_style(void);
void set_alarm(void);
void set_time(void);
void set_region(void);
void set_date(void);
void set_backlight(void);
void print_timehour(uint8_t h, uint8_t inverted);
void print_alarmhour(uint8_t h, uint8_t inverted);
void display_menu(uint8_t line);
void drawArrow(uint8_t x, uint8_t y, uint8_t l);
void setalarmstate(void);
void beep(uint16_t freq, uint8_t duration);
void printnumber(uint8_t n, uint8_t inverted);
void print_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t mode);


int8_t random_angle(void);

void init_crand(void);
uint8_t dotw(uint8_t mon, uint8_t day, uint8_t yr);

uint8_t i2bcd(uint8_t x);

uint8_t readi2ctime(void);

void writei2ctime(uint8_t sec, uint8_t min, uint8_t hr, uint8_t day,
		  uint8_t date, uint8_t mon, uint8_t yr);

void print_date(uint8_t month, uint8_t day, uint8_t year, uint8_t mode);

void draw7seg(uint8_t x, uint8_t y, uint8_t segs, uint8_t inverted);
void drawsegment(uint8_t s, uint8_t x, uint8_t y, uint8_t inverted);
void drawdigit(uint8_t d, uint8_t x, uint8_t y, uint8_t inverted);
void drawvseg(uint8_t x, uint8_t y, uint8_t inverted);
void drawhseg(uint8_t x, uint8_t y, uint8_t inverted);
void drawdot(uint8_t x, uint8_t y, uint8_t inverted);


// RATT SPECIFIC

#define FIXED_MATH 256
// Used to be 100, but optimizations using MSB function force this to be 256.

// How big our screen is in pixels
#define SCREEN_W 128
#define SCREEN_H 64

// don't use these... SCREEN_H will overflow an int16 if FIXED_MATH==256
// but that's ok... usually we used the middle of the screen or subtract something off of
// it before doing the actual comparison.
#define SCREEN_W_FIXED (SCREEN_W * FIXED_MATH)
#define SCREEN_H_FIXED (SCREEN_H * FIXED_MATH)

#define RIGHTPADDLE_X (SCREEN_W - PADDLE_W - 10)
#define LEFTPADDLE_X 10

#define RIGHTPADDLE_X_FIXED (RIGHTPADDLE_X * FIXED_MATH)
#define LEFTPADDLE_X_FIXED (LEFTPADDLE_X * FIXED_MATH)

// Paddle size (in pixels) and max speed for AI
#define PADDLE_H 12
#define PADDLE_W 3

#define PADDLE_H_FIXED (PADDLE_H * FIXED_MATH)
#define PADDLE_W_FIXED (PADDLE_W * FIXED_MATH)

// How thick the top and bottom lines are in pixels
#define BOTBAR_H 2
#define TOPBAR_H 2

#define BOTBAR_H_FIXED (BOTBAR_H * FIXED_MATH)
#define TOPBAR_H_FIXED (TOPBAR_H * FIXED_MATH)

// Specs of the middle line
#define MIDLINE_W 1
#define MIDLINE_H (SCREEN_H / 16) // how many 'stipples'

// Max speed for AI
#define MAX_PADDLE_SPEED (5*FIXED_MATH)

#define DISPLAY_DOW1_X 35
#define DISPLAY_DOW2_X 50
#define DISPLAY_DOW3_X 70

#define DISPLAY_MON1_X 20
#define DISPLAY_MON2_X 35
#define DISPLAY_MON3_X 50

// how big are the pixels (for math purposes)
#define DISPLAY_DIGITW 10
#define DISPLAY_DIGITH 16

#define DISPLAY_DAY10_X 70
#define DISPLAY_DAY1_X 85

//THIS IS A GUESS!!!! - IS DATELONG_MON BACKWARDS COMPAT?
#define SCORE_MODE_DATELONG 5

// ====================
// XDALICHRON SPECIFIC

#define DIGIT_WIDTH 28
#define DIGIT_HEIGHT 64
#define MAX_STEPS 32

// ====================
// DEATHCHRON SPECIFIC
#define SKULL 0
#define REAPER 1
#define RIP 2
#define REAPER_TOW_RIP 3

#ifdef GPSENABLE
char uart_getch(void);
uint8_t GPSRead(uint8_t);
uint8_t DecodeGPSBuffer(char *t);
#endif


