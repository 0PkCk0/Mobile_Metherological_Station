#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


/**
 * Prova
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	//println("something"); //testcommit
}
