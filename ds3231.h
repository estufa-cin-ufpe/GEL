#ifndef LIBS_DS3231_H_
#define LIBS_DS3231_H_

#ifndef I2C_H_
#include "i2c.h"
#endif

#ifndef GPIO_H_
#include "gpio.h"
#endif

#ifndef _STDBOOL_H
#include <stdbool.h>
#endif

#define SECONDS_FROM_1970_TO_2000 946684800

typedef struct
{
	uint8_t yOff, m, d, hh, mm, ss;
} DateTime;

static const uint8_t daysInMonth [];

ADI_I2C_RESULT setAlarm(); //por enquanto o valor é hardcoded
ADI_I2C_RESULT resetAlarm();

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d);
static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s);
static uint8_t bcd2bin (uint8_t val);
DateTime now();
uint32_t unixtime(DateTime dt);

uint8_t getSecond();
uint8_t getMinute();
uint8_t getHour(bool* _h12, bool* _PM_time);
uint8_t getDoW();
uint8_t getDate();
uint8_t getMonth(bool* _Century);
uint8_t getYear();
void setSecond(uint8_t Second);
void setMinute(uint8_t Minute);
void setHour(uint8_t Hour);
void setDoW(uint8_t DoW);
void setDate(uint8_t Date);
void setMonth(uint8_t Month);
void setYear(uint8_t Year);
void setClockMode(bool h12);
void getA1Time(uint8_t* _A1Day, uint8_t* _A1Hour, uint8_t* _A1Minute, uint8_t* _A1Second, uint8_t* _AlarmBits, bool* _A1Dy, bool* _A1h12, bool* _A1PM);
void getA2Time(uint8_t* _A2Day, uint8_t* _A2Hour, uint8_t* _A2Minute, uint8_t* _AlarmBits, bool* _A2Dy, bool* _A2h12, bool* _A2PM);
void setA1Time(uint8_t A1Day, uint8_t A1Hour, uint8_t A1Minute, uint8_t A1Second, uint8_t AlarmBits, bool A1Dy, bool A1h12, bool A1PM);
void setA2Time(uint8_t A2Day, uint8_t A2Hour, uint8_t A2Minute, uint8_t AlarmBits, bool A2Dy, bool A2h12, bool A2PM);
void turnOnAlarm(uint8_t Alarm);
void turnOffAlarm(uint8_t Alarm);
bool checkAlarmEnabled(uint8_t Alarm);
bool checkIfAlarm(uint8_t Alarm);
void enableOscillator(bool TF, bool battery, uint8_t frequency);
void enable32kHz(bool TF);
bool oscillatorCheck();

static uint8_t decToBcd(uint8_t val);
static uint8_t bcdToDec(uint8_t val);
static uint8_t readControlByte(bool which);
static void writeControlByte(uint8_t control, bool which);

#endif /* LIBS_DS3231_H_ */
