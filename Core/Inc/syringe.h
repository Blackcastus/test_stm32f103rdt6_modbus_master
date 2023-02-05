/*
	This file is driver control Syringe Pumps
*/

#ifndef __SYRINGE_H__
#define __SYRINGE_H__

#include "main.h"
#include "ringbuf.h"
/* Pumps Setting Commands */
#define ADDR 		0x00
#define RS232_BAUD	0x01
#define RS485_BAUD	0x02
#define CAN_BAUD	0x03
#define MAX_SPEED	0x07
#define RESET_SPEED	0x0B
#define PWR_ON		0x0E
#define CAN_ADDR	0x01
#define RESET_DRIVER	0xFF

/* Pumps Inquiry Commands */

#define INQ_ADDR	0x20
#define INQ_RS232_BAUD	0x21
#define INQ_RS485_BAUD	0x22
#define INQ_CAN_BAUD	0x23
#define INQ_MAX_SPEED	0x27
#define INQ_RST_SPEED	0x2B
#define INQ_PWR_ON		0x2E
#define INQ_CAN_ADDR	0x03
#define INQ_VERSION		0x3F

/* Pumps Control Commands */
#define XYLANH_CCW		0x4D 	// Motor moves as CCW (suction)
#define XYLANH_CW 		0x42	// Motor moves as CW (injection), stop when touch reset Optocoupler
#define VALVE_RUN		0x44	// Motor moves to value
#define PUMP_RESET	 	0x45
#define PUMP_STOP		0x49
#define PUMP_STATUS		0x4A
#define SET_SPEED		0x4B
#define POSITION		0x66
#define CLEAR_POSITION	0x67
#define PUMP_DIRECT		0x68

/* Pumps Status */

#define NORMAL			0x00
#define FRAME_ERR		0x01
#define PARAM_ERR		0x02
#define OPTO_ERR		0x03
#define PUMP_BUSY		0x04
#define SUSPENSION		0xFE
#define UNKNOWN_ERR		0xFF

/* Frame 8 byte */
#define Stack_Start		0
#define Stack_Addr		1
#define Stack_Cmd		2
#define Stack_Param		3
#define Stack_End		5
#define Stack_CheckSum	6

/* Frame 14 byte*/
#define Stack_Pass		3

typedef struct
{
	uint8_t addr;
	uint8_t txbuf[14];
	uint8_t rxbuf[14];
	uint8_t cmd;
	uint8_t state;
	uint16_t value;
	uint8_t flag;
} PUMP_STRUCT;

typedef enum
{
	AIR = 0x01,
	CUVETTE = 0x02,
	K2S2O8 = 0x03,
	HCL = 0x04,
	SAMPLE = 0x05,
	DILLUTION = 0x06,
	STANDARD = 0x07,
	NAOH = 0x08,
	REACTOR = 0x09,
	WASTE = 0x0A
} VALVE_NAME;

extern uint8_t u2_rxFlag;
extern uint32_t u2_rxTick;
extern ringbuf u2_rxRb;
extern uint8_t u2_rxRbBuf[128];
extern PUMP_STRUCT valve;
extern PUMP_STRUCT xylanh;

void PUMP_Init(PUMP_STRUCT *pump, uint8_t addr);
uint16_t CALC_CheckSum(uint8_t *buffer, uint8_t length);
void PUMP_Tx(PUMP_STRUCT *pump, uint8_t numByte);
void PUMP_SendCmd8(PUMP_STRUCT *pump, uint8_t cmd, uint16_t param);
void PUMP_SendCmd14(PUMP_STRUCT *pump, uint8_t cmd, uint32_t param);
void PUMP_Manager(PUMP_STRUCT *pump);
void PUMP_Reset(PUMP_STRUCT *pump);
void PUMP_Stop(PUMP_STRUCT *pump);
void PUMP_Inquiry(PUMP_STRUCT *pump);
void XYLANH_Suction(float ml);
void XYLANH_Injection(float ml);
void VALVE_Run(uint8_t pos);
uint8_t XYLANH_Status(void);
uint8_t VALVE_Status(void);
#endif /* __SYRINGE_H__ */