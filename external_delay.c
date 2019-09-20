#include "external_delay.h"

volatile uint32_t delayct = 0;
volatile uint32_t delaytg = 0;
volatile uint8_t ctturn = 0;
volatile uint8_t innerct = 0;

ADI_GPIO_RESULT external_delaySetup()
{
	ADI_GPIO_RESULT result;
	result = pinMode(IO13, INPUT_PULLUP);
	if(result)
	{
		return result;
	}
	result = setupInterrupt(IO13, IH_32kHz, RISING, ADI_GPIO_INTB_IRQ);
	if(result)
	{
		return result;
	}
	return result;
}

void delay(int ms)
{
	delaytg = ms;
	delayct = 0;
	innerct = 0;
	adi_gpio_RegisterCallback(ADI_GPIO_INTB_IRQ, IH_32kHz, (void*)ADI_GPIO_INTB_IRQ);
	while(delayct < delaytg);
	adi_gpio_RegisterCallback(ADI_GPIO_INTB_IRQ, NULL, (void*)ADI_GPIO_INTB_IRQ);
}

static void IH_32kHz(void* pCBParam, uint32_t Port, void* Pin)
{
	if(Port == IO13.port && *(uint32_t*)Pin == IO13.pin)
	{
		//3*38+1*37
		innerct++;
		if(ctturn < 3 && innerct == 38)
		{
			delayct++;
			innerct = 0;
			ctturn++;
		}
		else if(ctturn == 3 && innerct == 37)
		{
			delayct++;
			innerct = 0;
			ctturn = 0;
		}
	}
}
