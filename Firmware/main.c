#include "sam.h"
#include "System.h"
#include "SysTick.h"
#include "DawController.h"

int main(void)
{
    SystemClock48MhzInit();
	__enable_irq();
	// Tick every 1000ms / 100 =  10 ms
	SysTickInit(100);
	SysTickStart();
	
	DawControllerInit();

    while (1) 
    {	
		DawControllerProcessInput();
		DawControllerDraw();
	}
}