
#include <stdbool.h>
#include <stdint.h>

#include "io/serial.h"

#include "rx/rx.h"
#include "rx/targetcustomserial.h"
#include "fc/fc_core.h"


#define MSP_CMD_START			                '$'
#define MSP_CMD_HEADER_M		              'M'
#define MSP_CMD_HEADER_ARROW              '<'



#define MSP_CMD_STATE_WAIT_START			    0
#define MSP_CMD_STATE_WAIT_HEADER_M			  1
#define MSP_CMD_STATE_WAIT_HEADER_ARROW		2
#define MSP_CMD_STATE_WAIT_DATA_SIZE	  	3
#define MSP_CMD_STATE_WAIT_CMD				    4
#define MSP_CMD_STATE_WAIT_DATA				    5
#define MSP_CMD_STATE_WAIT_CHECKSUM       6


#define MSP_SET_RAW_RC_TINY               150   //in message          4 rc chan
#define MSP_ARM                           151
#define MSP_DISARM                        152



uint8_t  Cmd_State = MSP_CMD_STATE_WAIT_START;
uint8_t  Index_Data;


#define MSP_CMD_MAX_LENGTH		64


typedef struct
{
	uint8_t	Cmd;
	uint8_t	Length;
	uint8_t CheckSum;
	uint8_t	Data[MSP_CMD_MAX_LENGTH];
} MSP_CMD_OBJ;


typedef struct
{
	uint8_t	Cmd;
	uint8_t	Length;
	uint8_t ErrorCode;
	uint8_t CheckSum;
	uint8_t	Data[MSP_CMD_MAX_LENGTH];
} MSP_RESP_OBJ;


MSP_CMD_OBJ		Cmd;
MSP_RESP_OBJ	Resp;


static rxRuntimeConfig_t *rxRuntimeConfigPtr;
static serialPort_t *serialPort;

#define SUPPORTED_CHANNEL_COUNT 8
static uint32_t channelData[SUPPORTED_CHANNEL_COUNT];
static bool rcFrameComplete = false;


bool rxUpdate( uint8_t ch )
{
	bool    Ret = false;


	//-- 명령어 상태
	//
	switch( Cmd_State )
	{

		//-- 시작 문자 기다리는 상태
		//
		case MSP_CMD_STATE_WAIT_START:

			// 시작 문자를 기다림
			if( ch == MSP_CMD_START )
			{
				Cmd_State    = MSP_CMD_STATE_WAIT_HEADER_M;
			}
			break;


		//-- 'M' 기다리는 상태
		//
		case MSP_CMD_STATE_WAIT_HEADER_M:
			if( ch == MSP_CMD_HEADER_M )
			{
				Cmd_State = MSP_CMD_STATE_WAIT_HEADER_ARROW;
			}
			else
			{
				Cmd_State = MSP_CMD_STATE_WAIT_START;
			}
			break;


		//-- '<' 기다리는 상태
		//
		case MSP_CMD_STATE_WAIT_HEADER_ARROW:
			if( ch == MSP_CMD_HEADER_ARROW )
			{
				Cmd.CheckSum = 0x00;
				Cmd.Length   = 0;
				Cmd_State = MSP_CMD_STATE_WAIT_DATA_SIZE;
			}
			else
			{
				Cmd_State = MSP_CMD_STATE_WAIT_START;
			}
			break;


		//-- 데이터 사이즈 기다리는 상태(64까지)
		//
		case MSP_CMD_STATE_WAIT_DATA_SIZE:

			if( ch <= MSP_CMD_MAX_LENGTH )
			{
				Cmd.Length    = ch;
				Index_Data    = 0;
				Cmd.CheckSum ^= ch;
				Cmd_State     = MSP_CMD_STATE_WAIT_CMD;
			}
			else
			{
				Cmd_State = MSP_CMD_STATE_WAIT_START;
			}
			break;


		//-- 명령어를 기다리는 상태
		//
		case MSP_CMD_STATE_WAIT_CMD:

			Cmd.Cmd       = ch;
			Cmd.CheckSum ^= ch;

			if( Cmd.Length == 0 )
			{
				Cmd_State = MSP_CMD_STATE_WAIT_CHECKSUM;
			}
			else
			{
				Cmd_State = MSP_CMD_STATE_WAIT_DATA;
			}
			break;


		//-- 데이터를 기다리는 상태
		//
		case MSP_CMD_STATE_WAIT_DATA:

			Cmd.CheckSum          ^= ch;
			Cmd.Data[ Index_Data ] = ch;

			Index_Data++;

			if( Index_Data >= Cmd.Length )
			{
				Cmd_State = MSP_CMD_STATE_WAIT_CHECKSUM;
			}
			break;


		//-- 체크섬을 기다리는 상태
		//
		case MSP_CMD_STATE_WAIT_CHECKSUM:

			if( Cmd.CheckSum == ch )
			{
				Ret = true;
			}

			Cmd_State = MSP_CMD_STATE_WAIT_START;
			break;
	}

	return Ret;
}




// Receive ISR callback
static void dataReceive(uint16_t c)
{


	if (rxUpdate(c) == true)
	{
	  switch( Cmd.Cmd )
	  {
	    case MSP_SET_RAW_RC_TINY:
	      //CmdRoll     = 1000 + pCmd->Data[0] * 4 - 1500;
	      //CmdPitch    = 1000 + pCmd->Data[1] * 4 - 1500;
	      //CmdYaw      = 1000 + pCmd->Data[2] * 4 - 1500;
	      //CmdThrotle  = 1000 + pCmd->Data[3] * 4 - 1000;
	      //CmdAux      = pCmd->Data[4];
	      channelData[0] = 1000 + Cmd.Data[3] * 4;  // Throtle
	      channelData[1] = 1000 + Cmd.Data[0] * 4;  // Roll
	      channelData[2] = 1000 + Cmd.Data[1] * 4;  // Pitch
	      channelData[3] = 1000 + Cmd.Data[2] * 4;  // Yaw

	      uint8_t auxChannels = Cmd.Data[4];
	      uint8_t aux;

	      aux = (auxChannels & 0xc0) >> 6;

	      if(aux == 0){
	          channelData[4] = 1000;
	      }
	      else if(aux == 1){
	          channelData[4] = 1500;
	      }
	      else{
	          channelData[4] = 2000;
	      }


	      aux = (auxChannels & 0x30) >> 4;

	      if(aux == 0){
	          channelData[5] = 1000;
	      }
	      else if(aux == 1){
	          channelData[5] = 1500;
	      }
	      else{
	          channelData[5] = 2000;
	      }


	      aux = (auxChannels & 0x0c) >> 2;

	      if(aux == 0){
	          channelData[6] = 1000;
	      }
	      else if(aux == 1){
	          channelData[6] = 1500;
	      }
	      else{
	          channelData[6] = 2000;
	      }

	      aux = (auxChannels & 0x03);

	      if(aux == 0){
	          channelData[7] = 1000;
	      }
	      else if(aux == 1){
	          channelData[7] = 1500;
	      }
	      else{
	          channelData[7] = 2000;
	      }

	      break;

	    case MSP_ARM:
	      tryArm();
	      break;

	    case MSP_DISARM:
	      disarm();
	      break;

	    default:
	      break;
	  }

	  rcFrameComplete = true;
	}
}

static uint8_t frameStatus(void)
{
    if (!rcFrameComplete) {
        return RX_FRAME_PENDING;
    }

    // Set rcFrameComplete to false so we don't process this one twice
    rcFrameComplete = false;

    return RX_FRAME_COMPLETE;
}

static uint16_t readRawRC(const rxRuntimeConfig_t *rxRuntimeConfig, uint8_t chan)
{
    if (chan >= rxRuntimeConfig->channelCount) {
        return 0;
    }
    return channelData[chan];
}


bool targetCustomSerialRxInit(const rxConfig_t *rxConfig, rxRuntimeConfig_t *rxRuntimeConfig)
{
    rxRuntimeConfigPtr = rxRuntimeConfig;

    if (rxConfig->serialrx_provider != SERIALRX_TARGET_CUSTOM)
    {
        return false;
    }

    const serialPortConfig_t *portConfig = findSerialPortConfig(FUNCTION_RX_SERIAL);
    if (!portConfig) {
        return false;
    }

    rxRuntimeConfig->channelCount    = SUPPORTED_CHANNEL_COUNT;
    rxRuntimeConfig->rxRefreshRate   = 20000;
    rxRuntimeConfig->rcReadRawFn     = readRawRC;
    rxRuntimeConfig->rcFrameStatusFn = frameStatus;

    serialPort = openSerialPort(portConfig->identifier,
        FUNCTION_RX_SERIAL,
        dataReceive,
        115200,
        MODE_RX,
        SERIAL_NOT_INVERTED | SERIAL_STOPBITS_1 | SERIAL_PARITY_NO
        );

    return serialPort != NULL;
}

