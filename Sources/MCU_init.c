/*
 * MCU_init.c
 *
 *  Created on: Apr 13, 2019
 *      Author: Stud
 */

#include "MCU_init.h"
#include "derivative.h"


void MCU_init(void) {
	
	PTFDD = 0xFF;
	PTCDD = 0x00;
	PTDDD = 0x00;
	
	PTCPE = 0xFF;
	PTDPE = 0xFF;
	
	PTFD = 0x00;
}
