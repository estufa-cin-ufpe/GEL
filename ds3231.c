#include "ds3231.h"

static const uint8_t daysInMonth [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s)
{
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

static uint8_t bcd2bin (uint8_t val)
{
	return val - 6 * (val >> 4);
}

static uint8_t decToBcd(uint8_t val)
{
	return ( (val/10*16) + (val%10) );
}

static uint8_t bcdToDec(uint8_t val)
{
	return ( (val/16*10) + (val%16) );
}

static uint8_t readControlByte(bool which)
{
	uint8_t data;

	if(which)
	{
		data = 0x0F;
	}
	else
	{
		data = 0x0E;
	}
	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	return data;
}

static void writeControlByte(uint8_t control, bool which)
{
	uint8_t data[2];
	if(which)
	{
		data[0] = 0x0F;
	}
	else
	{
		data[0] = 0x0E;
	}
	data[1] = control;
	i2cWriteDS3231(2, data);
}

ADI_I2C_RESULT setAlarm()
{
	ADI_I2C_RESULT result;

	result = i2cWriteDS3231(2, "\x0F\x08");
	if(result)
	{
		return result;
	}
	result = i2cWriteDS3231(5, "\x07\x30\x80\x80\x80");
	if(result)
	{
		return result;
	}
	result = i2cWriteDS3231(2, "\x0E\x05");
	if(result)
	{
		return result;
	}
	return ADI_I2C_SUCCESS;
}

ADI_I2C_RESULT resetAlarm()
{
	return i2cWriteDS3231(2, "\x0F\x08");
}

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d)
{
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += daysInMonth[i - 1];
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

DateTime now()
{
	uint8_t data[7];
	data[0] = 0x00;

	i2cWriteDS3231(1, data);

	i2cReadDS3231(7, data);
	uint8_t ss = bcd2bin(data[0] & 0x7F);
	uint8_t mm = bcd2bin(data[1]);
	uint8_t hh = bcd2bin(data[2]);
	uint8_t d = bcd2bin(data[4]);
	uint8_t m = bcd2bin(data[5]);
	uint8_t y = bcd2bin(data[6]);

	return (DateTime){y, m, d, hh, mm, ss};
}

uint32_t unixtime(DateTime dt)
{
  uint32_t t;
  uint16_t days = date2days(dt.yOff, dt.m, dt.d);
  t = time2long(days, dt.hh, dt.mm, dt.ss);
  t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

  return t;
}


uint8_t getSecond()
{
	uint8_t data = 0x00;
	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	return bcdToDec(data);
}

uint8_t getMinute()
{
	uint8_t data = 0x01;
	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	return bcdToDec(data);
}

uint8_t getHour(bool* _h12, bool* _PM_time)
{
	uint8_t hour;
	uint8_t data = 0x02;

	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	*_h12 = data & 0b01000000;
	if(*_h12)
	{
		*_PM_time = data & 0b00100000;
		hour = bcdToDec(data & 0b00011111);
	} else
	{
		hour = bcdToDec(data & 0b00111111);
	}
	return hour;
}

uint8_t getDoW()
{
	uint8_t data = 0x03;
	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	return bcdToDec(data);
}

uint8_t getDate()
{
	uint8_t data = 0x04;
	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	return bcdToDec(data);
}

uint8_t getMonth(bool* _Century)
{
	uint8_t data = 0x05;

	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	*_Century = data & 0b10000000;
	return (bcdToDec(data & 0b01111111));
}

uint8_t getYear()
{
	uint8_t data = 0x06;
	i2cWriteDS3231(1, &data);

	i2cReadDS3231(1, &data);
	return bcdToDec(data);
}

void setSecond(uint8_t Second)
{
	uint8_t data[2];
	data[0] = 0x00;
	data[1] = decToBcd(Second);
	i2cWriteDS3231(2, data);

	uint8_t temp_buffer = readControlByte(1);
	writeControlByte((temp_buffer & 0b01111111), 1);
}

void setMinute(uint8_t Minute)
{
	uint8_t data[2];
	data[0] = 0x01;
	data[1] = decToBcd(Minute);
	i2cWriteDS3231(2, data);
}

void setHour(uint8_t Hour)
{
	bool h12;
	uint8_t data[2];
	data[0] = 0x02;
	i2cWriteDS3231(1, data);

	i2cReadDS3231(1, data);
	h12 = (data[0] & 0b01000000);
	if(h12)
	{
		if(Hour > 12)
		{
			Hour = decToBcd(Hour-12) | 0b01100000;
		}
        else
        {
			Hour = decToBcd(Hour) & 0b11011111;
		}
	}
    else
    {
		Hour = decToBcd(Hour) & 0b10111111;
	}


	data[0] = 0x02;
	data[1] = Hour;
	i2cWriteDS3231(2, data);
}

void setDoW(uint8_t DoW)
{
	uint8_t data[2];
	data[0] = 0x03;
	data[1] = decToBcd(DoW);
	i2cWriteDS3231(2, data);
}

void setDate(uint8_t Date)
{
	uint8_t data[2];
	data[0] = 0x04;
	data[1] = decToBcd(Date);
	i2cWriteDS3231(2, data);
}

void setMonth(uint8_t Month)
{
	uint8_t data[2];
	data[0] = 0x05;
	data[1] = decToBcd(Month);
	i2cWriteDS3231(2, data);
}

void setYear(uint8_t Year)
{
	uint8_t data[2];
	data[0] = 0x06;
	data[1] = decToBcd(Year);
	i2cWriteDS3231(2, data);
}

void setClockMode(bool h12)
{
	uint8_t data[2];
	data[0] = 0x02;
	i2cWriteDS3231(1, data);

	i2cReadDS3231(1, data);
	if(h12)
	{
		data[0] = data[0] | 0b01000000;
	} else
	{
		data[0] = data[0] & 0b10111111;
	}

	data[1] = data[0];
	data[0] = 0x02;
	i2cWriteDS3231(2, data);
}

void getA1Time(uint8_t* _A1Day, uint8_t* _A1Hour, uint8_t* _A1Minute, uint8_t* _A1Second, uint8_t* _AlarmBits, bool* _A1Dy, bool* _A1h12, bool* _A1PM)
{
	uint8_t data[4];

	data[0] = 0x07;
	i2cWriteDS3231(1, data);

	i2cReadDS3231(4, data);

	*_A1Second = bcdToDec(data[0] & 0b01111111);

	*_AlarmBits = *_AlarmBits | (data[0] & 0b10000000)>>7;

	*_A1Minute = bcdToDec(data[1] & 0b01111111);

	*_AlarmBits = *_AlarmBits | (data[1] & 0b10000000)>>6;

	*_AlarmBits = *_AlarmBits | (data[2] & 0b10000000)>>5;

	*_A1h12 = data[2] & 0b01000000;
	if(*_A1h12)
	{
		*_A1PM = data[2] & 0b00100000;
		*_A1Hour = bcdToDec(data[2] & 0b00011111);
	} else
	{
		*_A1Hour = bcdToDec(data[2] & 0b00111111);
	}

	*_AlarmBits = *_AlarmBits | (data[3] & 0b10000000)>>4;

	*_A1Dy = (data[3] & 0b01000000)>>6;
	if(*_A1Dy)
	{
		*_A1Day = bcdToDec(data[3] & 0b00001111);
	} else
	{
		*_A1Day = bcdToDec(data[3] & 0b00111111);
	}
}

void getA2Time(uint8_t* _A2Day, uint8_t* _A2Hour, uint8_t* _A2Minute, uint8_t* _AlarmBits, bool* _A2Dy, bool* _A2h12, bool* _A2PM)
{
	uint8_t data[3];

	data[0] = 0x0B;
	i2cWriteDS3231(1, data);

	i2cReadDS3231(3, data);

	*_A2Minute = bcdToDec(data[0] & 0b01111111);

	*_AlarmBits = *_AlarmBits | (data[0] & 0b10000000)>>3;


	*_AlarmBits = *_AlarmBits | (data[1] & 0b10000000)>>2;

	*_A2h12 = data[1] & 0b01000000;
	if(*_A2h12)
	{
		*_A2PM = data[1] & 0b00100000;
		*_A2Hour = bcdToDec(data[1] & 0b00011111);
	}
	else
	{
		*_A2Hour = bcdToDec(data[1] & 0b00111111);
	}

	*_AlarmBits = *_AlarmBits | (data[2] & 0b10000000)>>1;

	*_A2Dy = (data[2] & 0b01000000)>>6;
	if(*_A2Dy)
	{
		*_A2Day = bcdToDec(data[2] & 0b00001111);
	}
	else
	{
		*_A2Day = bcdToDec(data[2] & 0b00111111);
	}
}

void setA1Time(uint8_t A1Day, uint8_t A1Hour, uint8_t A1Minute, uint8_t A1Second, uint8_t AlarmBits, bool A1Dy, bool A1h12, bool A1PM)
{
	uint8_t temp_buffer;
	uint8_t data[5];
	data[0] = 0x07;

	data[1] = decToBcd(A1Second) | ((AlarmBits & 0b00000001) << 7);

	data[2] = decToBcd(A1Minute) | ((AlarmBits & 0b00000010) << 6);

	if(A1h12)
	{
		if(A1Hour > 12)
		{
			A1Hour = A1Hour - 12;
			A1PM = true;
		}
		if(A1PM)
		{
			temp_buffer = decToBcd(A1Hour) | 0b01100000;
		}
		else
		{
			temp_buffer = decToBcd(A1Hour) | 0b01000000;
		}
	}
	else
	{
		temp_buffer = decToBcd(A1Hour);
	}
	temp_buffer = temp_buffer | ((AlarmBits & 0b00000100)<<5);

	data[3] = temp_buffer;

	temp_buffer = ((AlarmBits & 0b00001000)<<4) | decToBcd(A1Day);
	if(A1Dy)
	{
		temp_buffer = temp_buffer | 0b01000000;
	}
	data[4] = temp_buffer;

	i2cWriteDS3231(5, data);
}

void setA2Time(uint8_t A2Day, uint8_t A2Hour, uint8_t A2Minute, uint8_t AlarmBits, bool A2Dy, bool A2h12, bool A2PM)
{
	uint8_t temp_buffer;
	uint8_t data[4];

	data[0] = 0x0B;

	data[1] = decToBcd(A2Minute) | ((AlarmBits & 0b00010000) << 3);

	if(A2h12)
	{
		if(A2Hour > 12)
		{
			A2Hour = A2Hour - 12;
			A2PM = true;
		}
		if(A2PM)
		{
			temp_buffer = decToBcd(A2Hour) | 0b01100000;
		}
		else
		{
			temp_buffer = decToBcd(A2Hour) | 0b01000000;
		}
	}
	else
	{
		temp_buffer = decToBcd(A2Hour);
	}

	temp_buffer = temp_buffer | ((AlarmBits & 0b00100000)<<2);

	data[2] = temp_buffer;

	temp_buffer = ((AlarmBits & 0b01000000)<<1) | decToBcd(A2Day);
	if(A2Dy)
	{
		temp_buffer = temp_buffer | 0b01000000;
	}
	data[3] = temp_buffer;

	i2cWriteDS3231(4, data);
}

void turnOnAlarm(uint8_t Alarm)
{
	uint8_t temp_buffer = readControlByte(0);

	if(Alarm == 1)
	{
		temp_buffer = temp_buffer | 0b00000101;
	}
	else
	{
		temp_buffer = temp_buffer | 0b00000110;
	}
	writeControlByte(temp_buffer, 0);
}

void turnOffAlarm(uint8_t Alarm)
{
	uint8_t temp_buffer = readControlByte(0);

	if(Alarm == 1)
	{
		temp_buffer = temp_buffer & 0b11111110;
	}
	else
	{
		temp_buffer = temp_buffer & 0b11111101;
	}
	writeControlByte(temp_buffer, 0);
}

bool checkAlarmEnabled(uint8_t Alarm)
{
	uint8_t result = 0x0;
	uint8_t temp_buffer = readControlByte(0);
	if(Alarm == 1)
	{
		result = temp_buffer & 0b00000001;
	}
	else
	{
		result = temp_buffer & 0b00000010;
	}
	return result;
}

bool checkIfAlarm(uint8_t Alarm)
{
	uint8_t result;
	uint8_t temp_buffer = readControlByte(1);
	if(Alarm == 1)
	{
		result = temp_buffer & 0b00000001;

		temp_buffer = temp_buffer & 0b11111110;
	}
	else
	{
		result = temp_buffer & 0b00000010;

		temp_buffer = temp_buffer & 0b11111101;
	}
	writeControlByte(temp_buffer, 1);
	return result;
}

void enableOscillator(bool TF, bool battery, uint8_t frequency)
{
	if(frequency > 3) frequency = 3;

	uint8_t temp_buffer = readControlByte(0) & 0b11100111;
	if(battery)
	{
		temp_buffer = temp_buffer | 0b01000000;
	}
	else
	{
		temp_buffer = temp_buffer & 0b10111111;
	}
	if(TF)
	{
		temp_buffer = temp_buffer & 0b01111011;
	}
	else
	{
		temp_buffer = temp_buffer | 0b10000000;
	}

	frequency = frequency << 3;
	temp_buffer = temp_buffer | frequency;

	writeControlByte(temp_buffer, 0);
}

void enable32kHz(bool TF)
{
	uint8_t temp_buffer = readControlByte(1);
	if(TF)
	{
		temp_buffer = temp_buffer | 0b00001000;
	}
	else
	{
		temp_buffer = temp_buffer & 0b11110111;
	}
	writeControlByte(temp_buffer, 1);
}

bool oscillatorCheck()
{
	uint8_t temp_buffer = readControlByte(1);
	bool result = true;
	if(temp_buffer & 0b10000000)
	{
		result = false;
	}
	return result;
}
