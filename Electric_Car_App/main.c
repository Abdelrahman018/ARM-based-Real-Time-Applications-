#include "FreeRTOS.h"
#include "task.h"
#include "RTOS_Tasks.h"

int main(void)
{
    /* Configure System Clock at 80 MHz */
		vClockConfiguration();
		/* Setup starting hardware components */
		prvSetupHardware ();
    /* Create Tasks */
		vCreate_Tasks();
		/* Start Scheduler */
    vTaskStartScheduler();
		while(1);
}
