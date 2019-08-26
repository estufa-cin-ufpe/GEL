#ifndef UART_H_
#define UART_H_

#ifndef ADI_UART_H
#include <drivers/uart/adi_uart.h>
#endif

#ifndef _STDLIB_H_
#include <stdlib.h>
#endif


//uart control variables
extern uint8_t uartMemory[ADI_UART_BIDIR_MEMORY_SIZE];
extern uint32_t uart_hErrors;
extern ADI_UART_HANDLE uartDevice;
extern uint8_t* rx_buffer;
extern uint8_t rx_buffer_size;

void callback(void* pAppHandle, uint32_t nEvent, void* pArg);

//setups UART driver
//baudrate: baudrate
ADI_UART_RESULT uartSetup(uint32_t baudrate);

//returns number of bytes received available in buffer
uint8_t uart_available();

//returns one byte from buffer as a pointer
uint8_t* uartRead();

//reads len bytes from buffer. Returns 0 in case of success and 1 otherwise
//buf: pointer where the bytes are going to be copied to.
//len: number of bytes to be read
int uartReadBuffer(uint8_t* buf, uint8_t len);

//writes one byte (blocking)
//byte: data to be written
ADI_UART_RESULT uartWrite(uint8_t byte);

//writes len bytes (blocking)
//buffer: pointer with data to be written
//len: number of bytes to be written
ADI_UART_RESULT uartWriteBuffer(uint8_t* buffer, uint8_t len);

//writes one byte (non-blocking)
//byte: data to be written
ADI_UART_RESULT async_uartWrite(uint8_t byte);

//writes len bytes (non-blocking)
//buffer: pointer with data to be written
//len: number of bytes to be written
ADI_UART_RESULT async_uartWriteBuffer(uint8_t* buffer, uint8_t len);

#endif /* UART_H_ */
