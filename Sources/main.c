#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "IL.h"
#include "config.h"
#include "MCU_init.h"
#include "Timer.h"

void main(void) {
  EnableInterrupts;
  /* include your code here */

  MCU_init();
  TimerInit();

  for(;;) {
 
	  if (TPM1SC_TOF == 1) {
			 IL_mainfunction();
			 TPM1SC_TOF = 0;

	  }
	  	  
	  
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
