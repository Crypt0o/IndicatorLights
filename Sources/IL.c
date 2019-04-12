/*
 * IL.c
 *
 *  Created on: Apr 13, 2019
 *      Author: Stud
 */
#include "derivative.h"
#include "config.h"
#include "IL.h"

void IL_mainfunction(void) {
	int state = 2;
	
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

void IL_OFF(void) {
	PTFD = 0;
}

struct ILSTRUCT {
	struct {
		unsigned cntRIGHT;
		unsigned cntLEFT;
		unsigned cntRINSIDE;
		unsigned cntLINSIDE;
	} NORMAL;
	
	struct {
		unsigned cntL;
	} LANE_CHANGE;
	
	struct {
		unsigned cntH;
	} HAZARD;
	
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
	
	static unsigned cntRIGHT;
	static unsigned cntLEFT;
	static unsigned insideCOUNTER;
	
	if (!LEFT_SWITCH && RIGHT_SWITCH) { 
	// If the left switch is pressed and the right switch is not pressed, flash the left led
	 
		if (cntLEFT == 50) {
			insideCOUNTER++;
						
			if (insideCOUNTER < 30)	// For 300 ms, the LED will be on
				PTFD = 1;
			else if (insideCOUNTER >= 30&& insideCOUNTER < 100) // For 700 ms, the LED will be off
				PTFD = 0;
			else insideCOUNTER = 0;
		}
		else ++insideCOUNTER;
	}
	
	// If the right switch is pressed but not the left one, flash the right led
	else if (LEFT_SWITCH && !RIGHT_SWITCH) {
		if (cntRIGHT == 50) {
			insideCOUNTER++;
						
			if (insideCOUNTER < 30)
				PTFD = 1<<7;
			else if (insideCOUNTER >= 30 && insideCOUNTER < 100)
				PTFD = 0;
			else insideCOUNTER = 0;
		}
		else ++insideCOUNTER;
	}
	else // if both of the switches are not pressed or both pressed, reinit the variables
		PTFD = insideCOUNTER = 0;
}

void IL_LANECHANGE(void) {
	// The corresponding LED will flash three times, 300 ms ON, 700 ms OFF if SW is pressed less than 500 ms

}

void IL_HAZARD(void) {
	
}

