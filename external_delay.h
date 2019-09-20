#ifndef LIBS_EXTERNAL_DELAY_H_
#define LIBS_EXTERNAL_DELAY_H_

#ifndef GPIO_H_
#include "gpio.h"
#endif

ADI_GPIO_RESULT external_delaySetup();
void delay(int ms);

#endif /* LIBS_EXTERNAL_DELAY_H_ */
