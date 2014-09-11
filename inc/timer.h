#ifndef __TIMER_H
#define __TIMER_H

#include <stm32f0xx_conf.h>




void timerInit(uint16_t period);
void gpioInit();
void PWMInit(uint16_t pulse);

#endif /* __TIMER_H */
