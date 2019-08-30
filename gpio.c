#include "gpio.h"

pinMap SCL = {ADI_GPIO_PORT0, ADI_GPIO_PIN_4};
pinMap SDA = {ADI_GPIO_PORT0, ADI_GPIO_PIN_5};
pinMap SCLK = {ADI_GPIO_PORT0, ADI_GPIO_PIN_0};
pinMap MISO = {ADI_GPIO_PORT0, ADI_GPIO_PIN_2};
pinMap MOSI = {ADI_GPIO_PORT0, ADI_GPIO_PIN_1};
pinMap CS = {ADI_GPIO_PORT1, ADI_GPIO_PIN_10};
pinMap RDY = {ADI_GPIO_PORT1, ADI_GPIO_PIN_14};
pinMap IO28 = {ADI_GPIO_PORT1, ADI_GPIO_PIN_12};
pinMap IO08 = {ADI_GPIO_PORT0, ADI_GPIO_PIN_8};
pinMap IO27 = {ADI_GPIO_PORT1, ADI_GPIO_PIN_11};
pinMap IO33 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_1};
pinMap IO09 = {ADI_GPIO_PORT0, ADI_GPIO_PIN_9};
pinMap IO13 = {ADI_GPIO_PORT0, ADI_GPIO_PIN_13};
pinMap IO15 = {ADI_GPIO_PORT0, ADI_GPIO_PIN_15};
pinMap TX = {ADI_GPIO_PORT0, ADI_GPIO_PIN_10};
pinMap RX = {ADI_GPIO_PORT0, ADI_GPIO_PIN_11};
pinMap AIN0 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_3};
pinMap AIN1 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_4};
pinMap AIN2 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_5};
pinMap AIN3 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_6};
pinMap AIN4 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_7};
pinMap AIN5 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_8};
pinMap IO16 = {ADI_GPIO_PORT1, ADI_GPIO_PIN_0};
pinMap IO12 = {ADI_GPIO_PORT0, ADI_GPIO_PIN_12};
pinMap DS4 = {ADI_GPIO_PORT1, ADI_GPIO_PIN_10};
pinMap DS3 = {ADI_GPIO_PORT2, ADI_GPIO_PIN_2};

ADI_GPIO_RESULT pinMode(pinMap pm, mode m)
{
	if(m == INPUT)
	{
		return adi_gpio_PullUpEnable(pm.port, pm.pin, true);
	}
	else
	{
		return adi_gpio_OutputEnable(pm.port, pm.pin, true);
	}
	return 0;
}

ADI_GPIO_RESULT digitalWrite(pinMap pm, logicLevel b)
{
	if(b)
	{
		return adi_gpio_SetHigh(pm.port, pm.pin);
	}
	else
	{
		return adi_gpio_SetLow(pm.port, pm.pin);
	}
	return 0;
}

logicLevel digitalRead(pinMap pm)
{
	uint16_t val;
	adi_gpio_GetData(pm.port, pm.pin, &val);
	return val ? HIGH : LOW;
}
