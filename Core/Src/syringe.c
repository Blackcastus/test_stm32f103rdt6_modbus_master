/*
	* @Author: nhantt
	* @Date:   2020-07-01
	* @Last Modified by:   nhantt
	* @Last Modified time: 2020-07-01
	* @This file is driver control Syringe Pumps
*/
#include "syringe.h"
#include "usart.h"
#include "string.h"

static uint8_t defBufCmd[8] = {0xCC, 0x00, 0x27, 0x00, 0x00, 0xDD, 0xD0, 0x01};
static uint8_t defBufFac[14] = {0xCC, 0x00, 0x00, 0xFF, 0xEE, 0xBB, 0xAA, 0x00, 0x00, 0x00, 0x00, 0xDD, 0xFB, 0x04};
/* Send Command 8 Byte Desc */
/*
	// 0xCC, 0x00, 0x27, 0x00, 0x00, 0xDD, 0xD0, 0x01
	// 0	->	CC is start code
	// 1	->	00 is addr
	// 2	->	27 is command
	// 3~4	->	00, 00 is command parameters 
	// 5	->	DD is end code
	// 7~8	->	checksum CC+00+27+00+00+DD = 01D0 -> D0,01
*/

/* Send Command 14 Byte Desc */
/*
	// 0xCC, 0x00, 0x00, 0xFF, 0xEE, 0xBB, 0xAA, 0x00, 0x00, 0x00, 0x00, 0xDD, 0xFB, 0x04}
	// 0		->	CC is start code
	// 1		->	00 is addr
	// 2		->	00 is command
	// 3~6		->	FF EE BB AA password factory 
	// 7~10		->	00 00 00 00 Parameters
	// 11		-> 	DD is end code
	// 12~13	->	checksum CC+00+00+FF+EE+BB+AA+00+00+00+00+DD = 04FB -> FB, 04
*/

PUMP_STRUCT xylanh;
PUMP_STRUCT valve;
uint8_t u2_rxFlag;
uint32_t u2_rxTick;
ringbuf u2_rxRb;
uint8_t u2_rxRbBuf[128];

void PUMP_Init(PUMP_STRUCT *pump, uint8_t addr)
{
	pump->addr = addr;
	memcpy(pump->txbuf, defBufCmd, 8);
	pump->flag = 0;
	u2_rxFlag = 0;
	ringbuf_init(&u2_rxRb, u2_rxRbBuf, sizeof(u2_rxRbBuf));
	MX_USART2_UART_Init();
}

uint16_t CALC_CheckSum(uint8_t *buffer, uint8_t length)
{
  uint16_t sum = 0;
  for(uint8_t i = 0; i < length; i++)
  {
    sum += buffer[i];
  }
  return sum;
}

void PUMP_Tx(PUMP_STRUCT *pump, uint8_t numByte)
{
  HAL_UART_Transmit(&huart2, pump->txbuf, numByte, 100);
}

void PUMP_SendCmd8(PUMP_STRUCT *pump, uint8_t cmd, uint16_t param)
{
	uint16_t checkSum;
	memcpy(pump->txbuf, defBufCmd, 8);
	pump->txbuf[Stack_Addr] = pump->addr;
	pump->txbuf[Stack_Cmd] = cmd;
	pump->txbuf[Stack_Param] = (uint8_t)(param);
	pump->txbuf[Stack_Param+1] = (uint8_t)(param>>8);
	checkSum = CALC_CheckSum(&pump->txbuf, 6);
	pump->txbuf[Stack_CheckSum] = (uint8_t)checkSum;
	pump->txbuf[Stack_CheckSum+1] = (uint8_t)(checkSum>>8);
	PUMP_Tx(pump, 8);
}

void PUMP_SendCmd14(PUMP_STRUCT *pump, uint8_t cmd, uint32_t param)
{
	uint16_t checkSum;
	memcpy(pump->txbuf, defBufFac, 14);
	pump->txbuf[Stack_Addr] = pump->addr;
	pump->txbuf[Stack_Cmd] = cmd;
	pump->txbuf[Stack_Param + 4] = (uint8_t)param;
	pump->txbuf[Stack_Param + 5] = (uint8_t)(param>>8);
	pump->txbuf[Stack_Param + 6] = (uint8_t)(param>>16);
	pump->txbuf[Stack_Param + 7] = (uint8_t)(param>>24);
	checkSum = CALC_CheckSum(&pump->txbuf, 12);
	pump->txbuf[Stack_CheckSum + 6] = (uint8_t)checkSum;
	pump->txbuf[Stack_CheckSum + 7] = (uint8_t)(checkSum>>8);
	PUMP_Tx(pump, 14);
}

void PUMP_ParserStack(PUMP_STRUCT *pump)
{
	uint16_t _handleParameter;
  	_handleParameter = pump->rxbuf[Stack_Param];
  	_handleParameter <<= 8;
  	_handleParameter |= pump->rxbuf[Stack_Param+1];
	switch (pump->rxbuf[Stack_Cmd]) 
	{
		case 0x00:
			pump->state = NORMAL;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		case 0x01:
			pump->state = FRAME_ERR;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		case 0x02:
			pump->state = PARAM_ERR;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		case 0x03:
			pump->state = OPTO_ERR;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		case 0x04:
			pump->state = PUMP_BUSY;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		case 0xFE:
			pump->state = SUSPENSION;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		case 0xFF:
			pump->state = UNKNOWN_ERR;
			pump->value = _handleParameter;
			pump->flag = 0;
			break;
		default:
			pump->state = UNKNOWN_ERR;
			pump->value = 0;
			pump->flag = 0;
		  	break;
	}
}

uint8_t PUMP_ParserPacket(PUMP_STRUCT *pump)
{
	uint16_t checkSumGet;
    if(pump->rxbuf[Stack_Start] != 0xCC)
    	return 0;
    if(pump->rxbuf[Stack_End] != 0xDD)
    	return 0;
    checkSumGet = pump->rxbuf[Stack_CheckSum+1];
    checkSumGet <<= 8;
    checkSumGet |= pump->rxbuf[Stack_CheckSum];
    if(checkSumGet != CALC_CheckSum(pump->rxbuf, 6))
    {
    	return 0;
    }
    PUMP_ParserStack(pump);
    return 1;
}

void PUMP_Manager(PUMP_STRUCT *pump)
{
	uint8_t c;
  	uint32_t writelen = 0, i=0;
  	uint8_t getBuff[9];
	if((HAL_GetTick() - u2_rxTick > 100) && (u2_rxFlag))
	{
		u2_rxFlag = 0;
		u2_rxTick = HAL_GetTick();
		while((ringbuf_get(&u2_rxRb, &c))&&(writelen < 9))
		{
		  	writelen++;
		  	getBuff[i++] = c;
		}
		if(writelen != 0)
		{
			memcpy(pump->rxbuf, getBuff, 8);
			if(PUMP_ParserPacket(pump))
			{
				//printf("State: 0x%02x\n", pump->state);
				//printf("value: 0x%04x\n", pump->value);
			}
			//printf("cmd: 0x%02x\n", getBuff[5]);
			//printf("%s", getBuff);
			//HAL_UART_Transmit(&huart1, getBuff, 8, 100);
		}
	}
}

void PUMP_Reset(PUMP_STRUCT *pump)
{
	PUMP_SendCmd8(pump, PUMP_RESET, 0);
}

void PUMP_Stop(PUMP_STRUCT *pump)
{
	PUMP_SendCmd8(pump, PUMP_STOP, 0);
}

void PUMP_Inquiry(PUMP_STRUCT *pump)
{
	PUMP_SendCmd8(pump, PUMP_STATUS, 0);
}

void XYLANH_Suction(float ml)
{
	uint16_t number;
	if((ml < 0.0) || (ml > 5.0)) return;
	number = (uint16_t)(ml*1000 / 0.4154);
	//printf("number: 0x%04x\n", number);
	PUMP_SendCmd8(&xylanh, XYLANH_CCW, number);
}

void XYLANH_Injection(float ml)
{
	uint16_t number;
	if((ml < 0.0) || (ml > 5.0)) return;
	number = (uint16_t)(ml*1000 / 0.4154);
	PUMP_SendCmd8(&xylanh, XYLANH_CW, number);
}

void VALVE_Run(uint8_t pos)
{
	//if((pos <= 0x00) || (pos > 0x0C)) return;
	printf("valve pos: 0x%02x\n", pos);
	PUMP_SendCmd8(&valve, VALVE_RUN, pos);
}

uint8_t XYLANH_Status(void)
{

}

uint8_t VALVE_Status(void)
{

}
