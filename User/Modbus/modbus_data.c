/*
* @Author: duchien 
* @Date:   2023-03-23
* @Last Modified by:   duchien
* @Last Modified time: 2023-03-23
*/

#include "modbus_data.h"
#include "modbus_crc.h"
#include "usart.h"

// Modbus Register
Modbus_State_t slave;
uint16_t modbus_holding_buf[NoB_HOLDING_REGISTER] = {0},
		modbus_input_buf[NoB_INPUT_REGISTER] = {0};
uint16_t modbus_master_hold_buf[MB_MASTER_SLAVE_NUM][NoB_HOLDING_REGISTER];
uint32_t modbus_tick_timeout;

void Modbus_Master_DMA_Init(void *dHUART, uint32_t ulBaudRate)
{
    // init uart connect with slaver
    MX_USART2_UART_Init(ulBaudRate);
}


MB_Error_Code_t Modbus_Master_DMA_Process(void)
{
    // id | fc | reg_addH | reg_addL | reg_dataH | reg_dataL | crcH | crcL

    /* code */
    if (dma_uart2_rx.state) // da nhan xong frame modbus => xu ly
    {
        if (Modbus_Check_Crc16(dma_uart2_rx.data, dma_uart2_rx.length)) // check crc is OK
        {
            switch (dma_uart2_rx.data[1])       // Check Function code
            {
                case R_HOLD_REGS:   // fc is read
                {
                    uint8_t id_slaver;
                    uint16_t total_reg;
                    id_slaver = dma_uart2_rx.data[0];
					total_reg = dma_uart2_rx.data[2]/2;

                    if(id_slaver > MB_MASTER_SLAVE_NUM || slave.reg_hold_start + total_reg > NoB_HOLDING_REGISTER)
                        return;
                    for(int i = slave.reg_hold_start, j = 3; i < slave.reg_hold_start + total_reg; i++ ,j += 2)
                    {
                        modbus_master_hold_buf[id_slaver-1][i] = dma_uart2_rx.data[j] << 8 | dma_uart2_rx.data[j + 1];
                    }
                    break;
                }
                case W_SINGLE_REG: // single write

                    break;

                default:
                    break;
            }
        }
        else
        {
            printf("Check CRC Error @@\n");
            return MC_CRCERR;
        }
        /* Reset */
        slave.index = WATTING_OK;
        slave.wait = NOWAIT;
        dma_uart2_rx.state = 0;
        dma_uart2_rx.length = 0;
    }
    
}

// Doc gia tri thanh ghi
MB_Error_Code_t Modbus_Master_Read_HoldRegs(uint8_t id, uint16_t start_reg, uint16_t total_num, uint32_t timeout)
{
    MB_Error_Code_t mbStatus = MB_ENOERR;

    switch (slave.index)
    {
        case NOWAIT:
            // transmit data
            slave.id = id;
            slave.reg_hold_start = start_reg;
            slave.reg_hold_total = total_num;
            uint8_t payload[8] = {id, R_HOLD_REGS, start_reg<<8, start_reg, total_num<<8, total_num};
            uint16_t i = Modbus_Create_Crc16(payload, 6);
            payload[6] = (uint8_t)i;
            payload[7] = (uint8_t)(i>>8);
            mbStatus = Uart2_Send_Data(payload, 8);
            slave.index = slave.wait = WAITING;
            slave.time_out = HAL_GetTick();
            break;
        case WAITING:
            // check time out
            if(HAL_GetTick() - slave.time_out >= timeout && slave.wait == WAITING)
            {
                    slave.status = MB_ETIMEDOUT;
                    slave.index = slave.wait = NOWAIT;
                    // printf("error time out\n");
                    return mbStatus;
            }
            break;
        case WATTING_OK:
            // respont data ok
            slave.index = NOWAIT;
            slave.status = MB_ENOERR;
            return 0;
        default:
            break;
    }
}

// // Ghi gia tri toi 1 thanh ghi
// void Modbus_Uart2_Write_Single_Reg(uint8_t id, uint16_t reg, uint16_t value)
// {
//     uint8_t payload[8] = {id, W_SINGLE_REG, reg<<8, reg, value<<8, value};
//     uint16_t i = Modbus_Create_Crc16(payload, 6);
//     payload[6] = (uint8_t)i;
//     payload[7] = (uint8_t)(i>>8);
//     Uart2_Send_Data(payload, 8);
//     HAL_Delay(100);
// }