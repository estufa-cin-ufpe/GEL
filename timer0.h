#ifndef LIBS_TIMER0_H_
#define LIBS_TIMER0_H_

#ifndef ADI_TMR_H
#include <adi_tmr.h>
#endif

#ifndef ADI_PWR_H
#include <drivers/pwr/adi_pwr.h>
#endif

ADI_TMR_RESULT timer0Setup();

void timer0Callback(void* pAppHandle, uint32_t nEvent, void* pArg);

void delay(uint16_t ms);

void sleep(uint16_t ms);

extern ADI_TMR_CONFIG timer0Config;
extern uint16_t delayCounter;
extern uint16_t delayTarget;
extern bool delayFlag;

#endif /* LIBS_TIMER0_H_ */
