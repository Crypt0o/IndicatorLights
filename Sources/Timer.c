/*
 * Timer.c
 *
 *  Created on: Apr 13, 2019
 *      Author: Stud
 */


#include "derivative.h"
#include "Timer.h"

void TimerInit(void) {
	
	TPM1SC = 0x000B; // 8 prescaler
	TPM1MOD = TIMER_10MS_VALUE;
	
	TPM1C0SC = 0X0010;
	TPM1C0V = TIMER_10MS_VALUE;
	
}
