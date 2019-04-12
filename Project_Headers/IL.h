/*
 * IL.h
 *
 *  Created on: Apr 13, 2019
 *      Author: Stud
 */

#ifndef IL_H_
#define IL_H_
	enum States {
		OFF,
		LANECHANGE,
		NORMAL,
		HAZARD
	};
	

	extern void IL_mainfunction(void);
	extern void IL_OFF(void);
	extern void IL_NORMAL(void);
	extern void IL_LANECHANGE(void);
	extern void IL_HAZARD(void);

#endif /* IL_H_ */



