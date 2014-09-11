//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"
#include "timer.h"

// ----------------------------------------------------------------------------
//
// STM32F0 empty sample (trace via $(trace)).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the $(trace) output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void Delay (uint32_t nTime);

int
main(int argc, char* argv[])
{
	static int pw = 0; // Define the cvariable for pulse width

	// Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
	timerInit(1000); // Init timer with a resolution of 1ms
	gpioInit();
	PWMInit(0); // Init PWM with a pulse witdth of 0
 // Infinite loop
 while (1)
   {
	  static inc = 1;
	  if ((pw < 1000) && (inc == 1))
	  {
		  pw++;
		  if (pw == 1000)
		  {
			  inc = 0;
		  }
	  }
	  else
	  {
		  pw--;
		  if (pw == 0)
		  {
			  inc = 1;
		  }
	  }
	  TIM_SetCompare1(TIM3, pw); // Update the PWM with the new pw value
	  Delay(2); // Wait 2ms
   }


}
static __IO uint32_t TimingDelay;

void Delay (uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
