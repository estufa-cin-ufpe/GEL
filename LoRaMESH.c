/* ---------------------------------------------------
        Radioenge Equipamentos de Telecomunicações
   ---------------------------------------------------
    This library contains a set of functions to configure
    and operate the EndDevice LoRaMESH Radioenge

  Date: 13/12/18

  Ported to ADuCM3029 by Rafael Marinho
*/
#include "LoRaMESH.h"

/* Frame type */
typedef struct
{
  uint8_t buffer[MAX_BUFFER_SIZE];
  uint8_t size;
  bool command;
} Frame_Typedef;

/* ----- Private Variables ----- */
static Frame_Typedef frame;
static uint16_t deviceId = -1;


/* ----- Public Variables ----- */

/* ----- Private Functions ----- */

void SerialFlush()
{
	free(rx_buffer);
	rx_buffer = NULL;
	rx_buffer_size = 0;
}



/* ----- Public Function Definitions ----- */
MeshStatus_Typedef PrepareFrameTransp(uint16_t id, uint8_t* payload, uint8_t payloadSize)
{
  uint8_t i = 0;

  if(payload == NULL) return MESH_ERROR;
  if(id > 1023) return MESH_ERROR;
  if(deviceId == -1) return MESH_ERROR;

  if((id != 0) && (deviceId == 0))  /* Is master */
  {
    frame.size = payloadSize + 2;
    /* Loads the target's ID */
    frame.buffer[i++] = id&0xFF;
    frame.buffer[i++] = (id>>8)&0x03;
  }
  else
  {
    frame.size = payloadSize;
  }

  if((payloadSize >= 0) && (payloadSize < MAX_PAYLOAD_SIZE))
  {
    /* Loads the payload */
    memcpy(&frame.buffer[i], payload, payloadSize);
  }
  else
  {
    /* Invalid payload size */
    memset(&frame.buffer[0], 0, MAX_BUFFER_SIZE);
    return MESH_ERROR;
  }

  frame.command = false;

  return MESH_OK;
}




MeshStatus_Typedef SendPacket()
{
  if(frame.size == 0) return MESH_ERROR;
  if((uartDevice == NULL) && !(frame.command)) return MESH_ERROR;

  if(!frame.command)
  {
	  uartWriteBuffer(frame.buffer, frame.size);
  }

  return MESH_OK;
}




MeshStatus_Typedef ReceivePacketTransp(uint16_t* id, uint8_t* payload, uint8_t* payloadSize, uint32_t timeout)
{
  uint16_t waitNextByte = 500;
  uint8_t i = 0;

  /* Assert parameters */
  if((id == NULL) && (deviceId == 0)) return MESH_ERROR;
  if(payload == NULL) return MESH_ERROR;
  if(payloadSize == NULL) return MESH_ERROR;
  if(uartDevice == NULL) return MESH_ERROR;
  if(deviceId == -1) return MESH_ERROR;


  /* Waits for reception */
  while( ((timeout > 0 ) || (i > 0)) && (waitNextByte > 0) )
  {
    if(uart_available() > 0)
    {
      frame.buffer[i++] = uartRead();
	    waitNextByte = 500;
    }

	  if(i > 0)
    {
      waitNextByte--;
    }
    timeout--;
    delay(1); //definir uma função para delay, não pode comentar ou remover essa liha.
  }

  /* In case it didn't get any data */
  if((timeout == 0) && (i == 0)) return MESH_ERROR;

  if(deviceId == 0)
  {
    /* Copies ID */
    *id = (uint16_t)frame.buffer[0] | ((uint16_t)frame.buffer[1] << 8);
    /* Copies payload size */
    *payloadSize = i-2;
    /* Copies payload */
    memcpy(payload, &frame.buffer[3], i-2);
  }
  else
  {
    /* Copies payload size */
    *payloadSize = i;
    /* Copies payload */
    memcpy(payload, &frame.buffer[0], i);
  }

  return MESH_OK;
}




uint16_t ComputeCRC(uint8_t* data_in, uint16_t length)
{
  uint16_t i;
  uint8_t bitbang, j;
  uint16_t crc_calc;

  crc_calc = 0xC181;
  for(i=0; i<length; i++)
  {
    crc_calc ^= (((uint16_t)data_in[i]) & 0x00FF);

    for(j=0; j<8; j++)
    {
      bitbang = crc_calc;
      crc_calc >>= 1;

      if(bitbang & 1)
      {
        crc_calc ^= 0xA001;
      }
    }
  }
  return (crc_calc&0xFFFF);
}
