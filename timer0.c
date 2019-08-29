#include "timer0.h"

ADI_TMR_CONFIG timer0Config;
uint16_t delayCounter = 0;
uint16_t delayTarget = 0;
bool delayFlag = true;

ADI_TMR_RESULT timer0Setup()
{
	ADI_TMR_RESULT result;

	if((adi_tmr_Init(ADI_TMR_DEVICE_GP0 , timer0Callback, NULL, true)) != ADI_TMR_SUCCESS)
	{
		return result;
	}

	timer0Config.bCountingUp  = false;
	timer0Config.bPeriodic    = true;
	timer0Config.ePrescaler   = ADI_TMR_PRESCALER_16;
	timer0Config.eClockSource = ADI_TMR_CLOCK_HFOSC;
	timer0Config.nLoad        = 1625u;
	timer0Config.nAsyncLoad   = 1625u;
	timer0Config.bReloading   = false;
	timer0Config.bSyncBypass  = false;
	if((adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP0 , &timer0Config)) != ADI_TMR_SUCCESS)
	{
		return result;
	}

	return ADI_TMR_SUCCESS;
}

void timer0Callback(void* pAppHandle, uint32_t nEvent, void* pArg)
{
	if ((nEvent & ADI_TMR_EVENT_TIMEOUT) == ADI_TMR_EVENT_TIMEOUT)
	{
		if(delayFlag)
		delayCounter++;
	}
	if(delayCounter == delayTarget)
	{
		delayCounter = 0;
		delayFlag = false;
	}
}

void delay(uint16_t ms)
{
	delayTarget = ms;
	delayFlag = true;
	adi_tmr_Enable(ADI_TMR_DEVICE_GP0, true);
	while(delayFlag);
	adi_tmr_Enable(ADI_TMR_DEVICE_GP0, false);
	return;
}

void delay(uint16_t ms)
{
	delayTarget = ms;
	delayFlag = true;
	adi_tmr_Enable(ADI_TMR_DEVICE_GP0, true);
	while(delayFlag)
	{
		adi_pwr_EnterLowPowerMode(ADI_PWR_MODE_SHUTDOWN , NULL, 0);
	}
	adi_tmr_Enable(ADI_TMR_DEVICE_GP0, false);
	return;
}
