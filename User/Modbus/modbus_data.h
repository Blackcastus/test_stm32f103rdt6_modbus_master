/* 
 * Modbus Data
 *
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

typedef struct 
{
	uint8_t		is_load_config;			/* 0 -> no, 1 -> yes */
	uint16_t 	first_addr;				/* First address for load */
	uint16_t 	total_addr;				/* number of address */		
}Modbus_State_t;


/* Configuration --------------------------------------------------------------*/
#define NoB_INPUT_REGISTER 			(200)					/* Input registers enable read: 04 */	
#define NoB_HOLDING_REGISTER 		(200)					/* holding registers enable read/write: 03, 06, 16 */	

extern uint16_t modbus_holding_buf[NoB_HOLDING_REGISTER],
		modbus_input_buf[NoB_INPUT_REGISTER];

void Modbus_Uart1_Data_Process(uint8_t id_slave, uint8_t * data, uint16_t length);

void Modbus_Uart2_Process(void);
void Modbus_Uart2_Write_Single_Reg(uint8_t id, uint16_t reg, uint16_t value);
void Modbus_Uart2_Read_Regs(uint8_t id, uint16_t reg, uint16_t number);

#endif
