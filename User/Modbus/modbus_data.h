/*
* @Author: duchien 
* @Date:   2023-03-23
* @Last Modified by:   duchien
* @Last Modified time: 2023-03-23
*/

/* 
 * Modbus Data
 * Modbus RTU
 * 
 *
*/

#ifndef _MODBUSB_DATA_H
#define _MODBUSB_DATA_H

#include "main.h"


#define R_COIL_STATUS 	(1)
#define R_DI_STATUS 	(2)
#define R_HOLD_REGS 	(3)
#define R_INPUT_REGS 	(4)
#define W_SINGLE_COIL 	(5)
#define W_SINGLE_REG 	(6)
#define W_MULTI_COIL 	(15)
#define W_MULTI_REGS 	(16)


typedef enum
{
	NOWAIT = 0,			/*!< no waitting or recive ok */
	WAITING, 			/*!< waitting respont from slaver */
	WATTING_OK
}Check_t;

typedef struct 
{
	uint8_t 	id;						/*!< id slave */		
	uint16_t 	reg_hold_start;			/*!< addrest register start */
	uint16_t 	reg_hold_total;			/*!< total addrest register slave */
	uint16_t 	time_out;				/*!< time out */
	uint16_t	status;
	Check_t 	wait;					/*!< waiting slave respont */
	Check_t		index;					/*!< waiting slave respont */
}Modbus_State_t;



typedef enum
{
	MB_ENOERR = 0,              /*!< no error. */
	MB_TX_UART_ERR,				/*!< not transmit */
	MC_CRCERR,					/*!< crc error */
    MB_ETIMEDOUT                /*!< timeout error. */
}MB_Error_Code_t;



/* Configuration --------------------------------------------------------------*/
#define NoB_INPUT_REGISTER 			(200)					/* Input registers enable read: 04 */	
#define NoB_HOLDING_REGISTER 		(200)					/* holding registers enable read/write: 03, 06, 16 */
#define MB_MASTER_SLAVE_NUM			(16)
#define M_REG_HOLDING_START			(0)

extern uint16_t modbus_holding_buf[NoB_HOLDING_REGISTER],
		modbus_input_buf[NoB_INPUT_REGISTER];
extern uint16_t modbus_master_hold_buf[MB_MASTER_SLAVE_NUM][NoB_HOLDING_REGISTER];

extern Modbus_State_t slave;

/*
* dHUART Seting uart connect with master
* ulBaudRate Setting baudrate
*/
void Modbus_Master_DMA_Init(void *dHUART, uint32_t ulBaudRate);
MB_Error_Code_t Modbus_Master_DMA_Process(void);
MB_Error_Code_t Modbus_Master_Read_HoldRegs(uint8_t id, uint16_t start_reg, uint16_t total_num, uint32_t timeout);

// void Modbus_Uart2_Read_Regs(uint8_t id, uint16_t reg, uint16_t number);

#endif
