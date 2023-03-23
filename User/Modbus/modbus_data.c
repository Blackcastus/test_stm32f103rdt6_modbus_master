#include "modbus_data.h"
#include "modbus_crc.h"
#include "usart.h"
#include "config.h"
#include "sht_sensor.h"
#include "control_system.h"


extern DEVICE_t device;


// Modbus Register
uint16_t modbus_holding_buf[NoB_HOLDING_REGISTER] = {0},
		modbus_input_buf[NoB_INPUT_REGISTER] = {0};
// UART
extern DMA_Event_t      dma_uart2_rx;


void Modbus_Uart2_Process(void)
{
    // Modbus_Refesh_Regs();
    /* code */
    if (dma_uart2_rx.state) // da nhan xong frame modbus => xu ly
    {
        if (Modbus_Check_Crc16(dma_uart2_rx.data, dma_uart2_rx.length))
        {
            switch (dma_uart2_rx.data[1])       // Kiem tra Function code         
            {
                case R_HOLD_REGS:
                    if (dma_uart2_rx.data[0] == 11)     // Daikin
                    {

                    } else {
                        // Cam bien SHT
                        SHT_Get_Value(dma_uart2_rx.data[0]-1, (dma_uart2_rx.data[3]<<8) + dma_uart2_rx.data[4], 
                            (dma_uart2_rx.data[5]<<8) + dma_uart2_rx.data[6]);
                    }
                    break;

                case W_SINGLE_REG:
                    // printf("daikin\n");
                    if (dma_uart2_rx.data[0] == 11)     // Daikin
                    {

                        if (dma_uart2_rx.data[3] == 5)
                        {
                            device.cooler.status = dma_uart2_rx.data[5];
                        } 
                    }
                    break;

                
                default:
                    break;
            }
        }
        
        /* Reset */
        dma_uart2_rx.state = 0;
        dma_uart2_rx.length = 0;
    }
    
}

// Ghi gia tri toi 1 thanh ghi
void Modbus_Uart2_Write_Single_Reg(uint8_t id, uint16_t reg, uint16_t value)
{
    uint8_t payload[8] = {id, W_SINGLE_REG, reg<<8, reg, value<<8, value};
    uint16_t i = Modbus_Create_Crc16(payload, 6);
    payload[6] = (uint8_t)i;
    payload[7] = (uint8_t)(i>>8);
    Uart2_Send_Data(payload, 8);
    HAL_Delay(100);
}

// Doc gia tri 1 thanh ghi
void Modbus_Uart2_Read_Regs(uint8_t id, uint16_t reg, uint16_t number)
{
    uint8_t payload[8] = {id, R_HOLD_REGS, reg<<8, reg, number<<8, number};
    uint16_t i = Modbus_Create_Crc16(payload, 6);
    payload[6] = (uint8_t)i;
    payload[7] = (uint8_t)(i>>8);
    Uart2_Send_Data(payload, 8);
    HAL_Delay(100);
}


void Modbus_Uart1_Data_Process(uint8_t id_slave, uint8_t * data, uint16_t length)
{
	if (data[0] == id_slave && Modbus_Check_Crc16(data, length))
	{
		switch (data[1])
		{
			case R_HOLD_REGS:
			case R_INPUT_REGS:
				if (length == 8)
				{
                    uint16_t first_addr, total_addr;
					first_addr = (data[2] << 8) | data[3];
					total_addr = (data[4] << 8) | data[5];

					if (data[1] == 3)		/* Holding */
					{
						if (first_addr + total_addr > NoB_HOLDING_REGISTER)
							return;
					}
					else
					{
						if (first_addr + total_addr > NoB_INPUT_REGISTER)
							return;
					}
					/* Respone data to master */
					uint8_t data_to_send[2*total_addr+5];
					uint16_t i, j;

					data_to_send[0] = id_slave;
					data_to_send[1] = data[1];
					data_to_send[2] = 2*total_addr;

					for (i = first_addr, j=3; i < (total_addr+first_addr); i++, j=j+2)
					{
						if (data[1] == 3)		/* Holding */
                        {
                            data_to_send[j] = (uint8_t)(modbus_holding_buf[i]>>8);
                            data_to_send[j+1] = (uint8_t)modbus_holding_buf[i];
                        }
						else
                        {
                            data_to_send[j] = (uint8_t)(modbus_input_buf[i]>>8);
                            data_to_send[j+1] = (uint8_t)modbus_input_buf[i];
                        }	
					}
					/* create crc */
					i = Modbus_Create_Crc16(data_to_send, j);
					data_to_send[j] = (uint8_t)i;
					data_to_send[j+1] = (uint8_t)(i>>8);
					Uart1_Send_Data(data_to_send, j+2);
					
				}
				break;
            case W_SINGLE_REG:
                if (length == 8)	
				{
                    uint16_t first_addr;
					/* Check valid register */
					first_addr = data[2] << 8 | data[3];
                    modbus_holding_buf[first_addr] = (data[4]<<8) + data[5];
                    Uart1_Send_Data(data, length);

                    switch (first_addr)
                    {
                        case 20:
                            device.temp_set = modbus_holding_buf[20] / 10.0;
                            sys_cfg.settemp = device.temp_set;                  
                            CFG_Save();
                            break;

                        case 21:
                            device.humi_set = modbus_holding_buf[21] / 10.0;
                            sys_cfg.sethumi = device.humi_set;
                            CFG_Save();
                            break;

                        case 25:
                            device.active = modbus_holding_buf[25];
                            break;
                            
                        case 30 ... 79:        // Temp Y1 -> ID 1
                            SHT_Get_Calib_Value(first_addr,modbus_holding_buf[first_addr]);
                            break;
                        
                        default:
                            break;
                    }

				}

                break;

			default:
				break;
		}
	}
}

void Modbus_Refesh_Regs(void)
{
    // fc 03 / 06
    modbus_holding_buf[0] = device.cooler.status;
    modbus_holding_buf[5] = device.humidify.status;
    modbus_holding_buf[6] = device.moise.status;
    modbus_holding_buf[7] = device.thermistor1;
    modbus_holding_buf[8] = device.thermistor2;
    modbus_holding_buf[9] = device.thermistor3;
    modbus_holding_buf[10] = device.light;
    modbus_holding_buf[11] = device.warning_green;
    modbus_holding_buf[12] = device.warning_red;
    modbus_holding_buf[13] = device.warning_yellow;
    modbus_holding_buf[14] = device.door;
    modbus_holding_buf[15] = device.valve_fan;
    modbus_holding_buf[16] = device.valve_cooler;
    modbus_holding_buf[17] = device.valve_heater;

    // fc 04
    modbus_input_buf[55] = device.sensor_door;

}

