/*
 * IL.c
 *
 *  Created on: Apr 13, 2019
 *      Author: Stud
 */
#include "derivative.h"
#include "config.h"
#include "IL.h"

int state = NORMAL;
int savedSwitch;

void IL_mainfunction(void) {
	extern int state;
	
	switch(state) {
		case LANECHANGE:
			IL_LANECHANGE();
			break;
			
		case NORMAL:
			IL_NORMAL();
			break;
			
		case HAZARD:
			IL_HAZARD();
			break;

		default:
			IL_OFF();
			break;
		
	}
};




void IL_NORMAL(void) {
/*
 * Timer duration: 10 ms
 * If Right switch or Left switch are pressed for longer than 500 ms,the corresponding LED will flash  
 * with a a PWM(30%, 1 Hz)
 * 
 * 		500 ms / 10 = 50					300 ms ON, 700 ms OFF
 * 		
 * 		
 * 	If the switch is released for at least 100 ms, all LEDS should turn off
 */
	extern int state;
	extern int savedSwitch;
	static unsigned insideCOUNTER;
	static unsigned cntRELEASE;
	static unsigned cntHAZARD;

	// **************** HAZARD ***************
	if (!HAZARD_SWITCH) 
	    ++cntHAZARD;
	else {
	   if (cntHAZARD >= 20)       insideCOUNTER = cntRELEASE = cntHAZARD = 0, state = HAZARD;   // Override to mode if it's pressed for at least 200 ms and released
	   else                       cntHAZARD = 0;
	}

	if (LEFT_SWITCH && RIGHT_SWITCH) { // if both of them are released, check for 100 ms if one was recently pressed 

	        ++cntRELEASE;
	        if (cntRELEASE >= 10) {
	           cntRELEASE = insideCOUNTER = 0;
	           state = OFF;
	          }
	}
	else {

	    insideCOUNTER++;			// savedSwitch is 1 for leftLED and 2 for rightLED
	    if (insideCOUNTER < 30)  PTFD = savedSwitch == 1? 1<<7 : 1;	// For 300 ms, the LED will be on
	    else if (insideCOUNTER < 100)  PTFD = 0;			    	// For 700 ms, the LED will be off
	    else insideCOUNTER = 0;		// After 1000 ms, the counter will reset itself
	}
}

void IL_LANECHANGE(void) {
	  extern int state;
	  extern int savedSwitch;
	  static unsigned cntRIGHT;
	  static unsigned cntLEFT;
	  static unsigned cntHAZARD;
	  static unsigned countToThree;
	  static unsigned countOnOff;


/* --------------------------------------------------------------------------------
*  Override to NORMAL state or HAZARD
*/

// **************** HAZARD ***************
	if (!HAZARD_SWITCH) 
	  ++cntHAZARD;
	else {   // Override to mode if it's pressed for at least 200 ms and released
	   if (cntHAZARD >= 20)       cntHAZARD  = cntRIGHT = cntLEFT = countToThree=countOnOff= 0, state = HAZARD;     
	   else                       cntHAZARD = 0;
	}
	  
// **************** NORMAL ***************  
	if ((!LEFT_SWITCH && RIGHT_SWITCH) || (LEFT_SWITCH && !RIGHT_SWITCH)) {
	  cntHAZARD = 0;
	  !LEFT_SWITCH? ++cntLEFT : ++cntRIGHT; // We increase the counter for the right switch
	    
	   if (cntLEFT  == 50 || cntRIGHT == 50) {
	        savedSwitch = cntRIGHT< cntLEFT? 1 : 2; // The higher value corresponds to the pressed switch
	        cntLEFT = cntRIGHT = 0;
	        state = NORMAL;
	   }
	    
	}
	else if (LEFT_SWITCH && RIGHT_SWITCH)
	    cntRIGHT = cntLEFT = 0;
	
// --------------------------- MAIN LOGIC ------------------------------------------------

	if (countToThree < 3) {
	  ++countOnOff;
	  
	    // LEFT_SWITCH is saved if savedSwitch is 1, RIGHT_SWITCH if 2
	  if (countOnOff < 30)  PTFD = savedSwitch == 1?  1 : 1<<7;
	    else if (countOnOff < 100) 	PTFD = 0;

	    else countOnOff = 0, countToThree ++; 
	 }
	  else countToThree = 0, countOnOff = 0, state = OFF;
	    

	}

void IL_HAZARD(void)  {
  extern int state;
  static unsigned LEDsTimeCounter;
  static unsigned cntHAZARD;

// ********** DEFAULT OVERRIDE *********
  if (!HAZARD_SWITCH) 
    ++cntHAZARD;
  else {
     if (cntHAZARD >= 20)       cntHAZARD = 0, state = OFF;      // Override to default mode if it's pressed for at least 200 ms and released
     else                       cntHAZARD = 0;
  }
  
// *******************************************
  ++LEDsTimeCounter;
  if (LEDsTimeCounter < 50) PTFD = 1 | 1<<7;
  else if (LEDsTimeCounter < 100) PTFD = 0;
  else LEDsTimeCounter = 0;
  
};

void IL_OFF(void) {
    extern int state;
    extern int savedSwitch;
    static unsigned cntRIGHT;
    static unsigned cntLEFT;
    static unsigned cntHAZARD;
   
    PTFD = 0;

  
/* --------------------------------------------------------------------------------
 *  Override to NORMAL state or HAZARD
 */

    // ***************************HAZARD************************************************
      if (!HAZARD_SWITCH) 
        ++cntHAZARD;
      else {  // If the HAZARD_SWITCH is released, we check if it was pressed for longer than 200 ms
         if (cntHAZARD >= 20)       cntHAZARD = 0, state = HAZARD;      // Override to mode if it's pressed for at least 200 ms and released
         else                       cntHAZARD = 0;
      }
      
    // ****************************  NORMAL *******************************************

        if ((!LEFT_SWITCH && RIGHT_SWITCH) || (!RIGHT_SWITCH && LEFT_SWITCH)){  // If right switch is pressed or left one is pressed
          !LEFT_SWITCH? ++cntLEFT : ++cntRIGHT; // We increase the counter for the right switch
          
          if (cntLEFT  == 50 || cntRIGHT == 50) {
            savedSwitch = cntRIGHT< cntLEFT? 1 : 2; // The higher value corresponds to the pressed switch
            cntLEFT = cntRIGHT = 0;
            state = NORMAL;
          }

        }
        //****************************** LANE CHANGE ***********************************//
        else if (LEFT_SWITCH && RIGHT_SWITCH) { 
            if ((cntLEFT > 0 && cntLEFT< 50) || (cntRIGHT > 0 && cntRIGHT< 50)) {
                savedSwitch = cntRIGHT < cntLEFT ? 1 : 2;
                cntLEFT = cntRIGHT = 0;
                state = 1;
            }
            else cntLEFT = cntRIGHT = 0, savedSwitch;
            
        }
};
