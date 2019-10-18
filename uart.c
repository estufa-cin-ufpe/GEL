#include "uart.h"

uint8_t uartMemory[ADI_UART_BIDIR_MEMORY_SIZE];
uint32_t uart_hErrors = 0;
ADI_UART_HANDLE uartDevice = NULL;
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_b = 0;
uint8_t rx_buffer_e = 0;
uint8_t rx_buffer_size = 0;
uint8_t rx_buffer_overflow = 0;

void uartCallback(void* pAppHandle, uint32_t nEvent, void* pArg)
{
	switch (nEvent)
	{
		case ADI_UART_EVENT_TX_BUFFER_PROCESSED:
			break;
		case ADI_UART_EVENT_RX_BUFFER_PROCESSED:
			adi_pwr_ExitLowPowerMode(NULL);
			break;
		case ADI_UART_EVENT_NO_RX_BUFFER_EVENT:
			adi_uart_SubmitRxBuffer(uartDevice, &rx_buffer[rx_buffer_e], 1, 0u);
			rx_buffer_e = (rx_buffer_e+1)%RX_BUFFER_SIZE;
			rx_buffer_size++;
			if(rx_buffer_e == rx_buffer_b)
			{
				rx_buffer_overflow = 1;
			}
			break;
		default:
			break;
	}
}

ADI_UART_RESULT uartSetup(uint32_t baudrate)
{
	ADI_UART_RESULT result;

	if((result = adi_uart_Open(0, ADI_UART_DIR_BIDIRECTION, uartMemory, ADI_UART_BIDIR_MEMORY_SIZE, &uartDevice)) != ADI_UART_SUCCESS)
	{
		return result;
	}
	if((result = adi_uart_RegisterCallback(uartDevice, uartCallback, NULL)) != ADI_UART_SUCCESS)
	{
		return result;
	}
	if((result = adi_uart_SetConfiguration(uartDevice, ADI_UART_NO_PARITY, ADI_UART_ONE_STOPBIT, ADI_UART_WORDLEN_8BITS)) != ADI_UART_SUCCESS)
	{
		return result;
	}
	if((result = adi_uart_EnableFifo(uartDevice, true)) != ADI_UART_SUCCESS)
	{
		return result;
	}
	if((result = adi_uart_SetRxFifoTriggerLevel(uartDevice, ADI_UART_RX_FIFO_TRIG_LEVEL_1BYTE)) != ADI_UART_SUCCESS)
	{
		return result;
	}

	switch(baudrate)
	{
		case 9600:
			result = adi_uart_ConfigBaudRate(uartDevice, 22, 3, 1734, 3);
			break;

		case 19200:
			result = adi_uart_ConfigBaudRate(uartDevice, 11, 3, 1735, 3);
			break;

		case 38400:
			result = adi_uart_ConfigBaudRate(uartDevice, 17, 1, 501, 3);
			break;

		case 57600:
			result = adi_uart_ConfigBaudRate(uartDevice, 7, 2, 31, 3);
			break;

		case 115200:
			result = adi_uart_ConfigBaudRate(uartDevice, 7, 2, 31, 2);
			break;

		case 230400:
			result = adi_uart_ConfigBaudRate(uartDevice, 7, 2, 31, 1);
			break;

		case 460800:
			result = adi_uart_ConfigBaudRate(uartDevice, 7, 2, 31, 0);
			break;

		default:
			return ADI_UART_INVALID_PARAMETER;
	}
	if(result)
	{
		return result;
	}

	return ADI_UART_SUCCESS;
}

uint8_t uartRead()
{
	adi_uart_RegisterCallback(uartDevice, NULL, NULL);

	if(rx_buffer_size>0)
	{
		uint8_t temp = rx_buffer[rx_buffer_b];
		rx_buffer_b = (rx_buffer_b+1)%RX_BUFFER_SIZE;
		rx_buffer_size--;
		adi_uart_RegisterCallback(uartDevice, uartCallback, NULL);
		return temp;
	}
	adi_uart_RegisterCallback(uartDevice, uartCallback, NULL);
	return -1;
}

int uartReadBuffer(uint8_t* buf, uint32_t len)
{
	adi_uart_RegisterCallback(uartDevice, NULL, NULL);
	if(rx_buffer_size>=len)
	{
		if(rx_buffer_b < rx_buffer_e && rx_buffer_b+len>= RX_BUFFER_SIZE)
		{
			memcpy(buf, &rx_buffer[rx_buffer_b], RX_BUFFER_SIZE-rx_buffer_b);
			memcpy(&buf[RX_BUFFER_SIZE-rx_buffer_b], &rx_buffer[0], len-(RX_BUFFER_SIZE-rx_buffer_b));
			rx_buffer_b = (rx_buffer_b+len)%RX_BUFFER_SIZE;
			rx_buffer_size-=len;
		}
		else
		{
			memcpy(buf, &rx_buffer[rx_buffer_b], len);
			rx_buffer_b = (rx_buffer_b+len)%RX_BUFFER_SIZE;
			rx_buffer_size-=len;
		}
		adi_uart_RegisterCallback(uartDevice, uartCallback, NULL);
		return 0;
	}
	adi_uart_RegisterCallback(uartDevice, uartCallback, NULL);
	return 1;
}

uint32_t uart_available()
{
	return rx_buffer_size;
}

ADI_UART_RESULT uartWrite(uint8_t byte)
{
	return adi_uart_Write(uartDevice, &byte, 1, true, &uart_hErrors);
}

ADI_UART_RESULT uartWriteBuffer(uint8_t* buffer, uint32_t len)
{
	return adi_uart_Write(uartDevice, buffer, len, true, &uart_hErrors);
}

ADI_UART_RESULT async_uartWrite(uint8_t byte)
{
	return adi_uart_SubmitTxBuffer(uartDevice, &byte, 1, true);
}

ADI_UART_RESULT async_uartWriteBuffer(uint8_t* buffer, uint32_t len)
{
	return adi_uart_SubmitTxBuffer(uartDevice, buffer, len, true);
}

void uartFlush()
{
	uint8_t rx_buffer_b = 0;
	uint8_t rx_buffer_e = 0;
	uint8_t rx_buffer_size = 0;
	uint8_t rx_buffer_overflow = 0;
}
