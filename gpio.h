#ifndef GPIO_H_
#define GPIO_H_

#ifndef ADI_GPIO_H
#include <drivers/gpio/adi_gpio.h>
#endif

typedef struct {
	ADI_GPIO_PORT port;
	ADI_GPIO_DATA pin;
} pinMap;

typedef enum {
	INPUT,
	OUTPUT
} mode;

typedef enum {
	HIGH = 1,
	LOW = 0
} logicLevel;

extern uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];

extern pinMap SCL;
extern pinMap SDA;
extern pinMap SCLK;
extern pinMap MISO;
extern pinMap MOSI;
extern pinMap CS;
extern pinMap RDY;
extern pinMap IO28;
extern pinMap IO08;
extern pinMap IO27;
extern pinMap IO33;
extern pinMap IO09;
extern pinMap IO13;
extern pinMap IO15;
extern pinMap TX;
extern pinMap RX;
extern pinMap AIN0;
extern pinMap AIN1;
extern pinMap AIN2;
extern pinMap AIN3;
extern pinMap AIN4;
extern pinMap AIN5;
extern pinMap IO16;
extern pinMap IO12;
extern pinMap DS4;
extern pinMap DS3;

ADI_GPIO_RESULT gpioSetup();
ADI_GPIO_RESULT pinMode(pinMap pm, mode m);
ADI_GPIO_RESULT digitalWrite(pinMap pm, logicLevel b);
logicLevel digitalRead(pinMap pm);


#endif
