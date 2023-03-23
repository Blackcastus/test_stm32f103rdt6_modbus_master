#include "relay.h"
#include "modbus_data.h"

RL_STRUCT_T *relayFlag;
// extern DEVICE_t device;

uint32_t blink_tick;

void RELAY_Init(void)
{   
    relayFlag = (RL_STRUCT_T*)malloc(sizeof(RL_STRUCT_T));
    memset(relayFlag, 0xFF, sizeof(RL_STRUCT_T));
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOA, RL1_PIN|RL15_PIN|RL16_PIN, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = RL1_PIN|RL15_PIN|RL16_PIN;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOB, RL6_PIN|RL7_PIN|RL8_PIN|RL9_PIN|RL10_PIN|RL11_PIN|RL12_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = RL6_PIN|RL7_PIN|RL8_PIN|RL9_PIN|RL10_PIN|RL11_PIN|RL12_PIN;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_GPIOC_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOC, RL2_PIN|RL3_PIN|RL4_PIN|RL5_PIN|RL13_PIN|RL14_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = RL2_PIN|RL3_PIN|RL4_PIN|RL5_PIN|RL13_PIN|RL14_PIN;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void RELAY1(uint8_t status)
{
    if((status) && (relayFlag->rl1 != status))
    {
        relayFlag->rl1 = status;
        RL1(1);
        HAL_Delay(100);
    }
    if((!status) && (relayFlag->rl1 != status))
    {
        relayFlag->rl1 = status;
        RL1(0);
    }
}

void RELAY2(uint8_t status)
{
    if((status) && (relayFlag->rl2 != status))
    {
        relayFlag->rl2 = status;
        RL2(1);

    }
    if((!status) && (relayFlag->rl2 != status))
    {
        relayFlag->rl2 = status;
        RL2(0);
    }
}

void RELAY3(uint8_t status)
{
    if (status)
    {
        RL4(0);
        if(HAL_GetTick() - blink_tick > 1000)
        {
            HAL_GPIO_TogglePin(GPIOC, RL3_PIN);
            blink_tick = HAL_GetTick();
        }
        
    }
    else if((!status) && (relayFlag->rl3 != status))
    {
        relayFlag->rl3 = status;
        RL3(0);
        
    }
}

void RELAY4(uint8_t status)
{
    if (status)
    {
        RL3(0);
        if(HAL_GetTick() - blink_tick > 1000)
        {
            HAL_GPIO_TogglePin(GPIOC, RL4_PIN);
            blink_tick = HAL_GetTick();
        }
    } 
    else if((!status) && (relayFlag->rl4 != status))
    {
        relayFlag->rl4 = status;
        RL4(0);
    }
   
}

void RELAY5(uint8_t status)
{

    if (status)
    {

        if(HAL_GetTick() - blink_tick > 1000)
        {
            HAL_GPIO_TogglePin(GPIOC, RL5_PIN);
            blink_tick = HAL_GetTick();
        }
    } 
    else if((!status) && (relayFlag->rl5 != status))
    {
        HAL_Delay(50);
        relayFlag->rl5 = status;
        RL5(0);
    }

}

void RELAY6(uint8_t status)
{
    if((status) && (relayFlag->rl6 != status))
    {
        relayFlag->rl6 = status;
        RL6(1);
        // HAL_Delay(50);
    }
    if((!status) && (relayFlag->rl6 != status))
    {
        relayFlag->rl6 = status;
        RL6(0);
    }
}

void RELAY7(uint8_t status)
{
    if((status) && (relayFlag->rl7 != status))
    {
        relayFlag->rl7 = status;
        RL7(1);
        // HAL_Delay(50);
    }
    if((!status) && (relayFlag->rl7 != status))
    {
        relayFlag->rl7 = status;
        RL7(0);
    }
}

void RELAY8(uint8_t status)
{
    if((status) && (relayFlag->rl8 != status))
    {
        relayFlag->rl8 = status;
        RL8(1);
        // HAL_Delay(500);
    }
    if((!status) && (relayFlag->rl8 != status))
    {
        relayFlag->rl8 = status;
        RL8(0);
    }
}

void RELAY9(uint8_t status)
{
    if((status) && (relayFlag->rl9 != status))
    {
        relayFlag->rl9 = status;
        RL9(1);
        // HAL_Delay(500);
    }
    if((!status) && (relayFlag->rl9 != status))
    {
        relayFlag->rl9 = status;
        RL9(0);
    }
}

void RELAY10(uint8_t status)
{
    if((status) && (relayFlag->rl10 != status))
    {
        relayFlag->rl10 = status;
        RL10(1);
        // HAL_Delay(500);
    }
    if((!status) && (relayFlag->rl10 != status))
    {
        relayFlag->rl10 = status;
        RL10(0);
    }
}

void RELAY11(uint8_t status)
{
    if((status) && (relayFlag->rl11 != status))
    {
        relayFlag->rl11 = status;
        RL11(1);

    }
    if((!status) && (relayFlag->rl11 != status))
    {
        relayFlag->rl11 = status;
        RL11(0);
    }
}

void RELAY12(uint8_t status)
{
    if((status) && (relayFlag->rl12 != status))
    {
        relayFlag->rl12 = status;
        RL12(1);
        // HAL_Delay(50);
    }
    if((!status) && (relayFlag->rl12 != status))
    {
        relayFlag->rl12 = status;
        RL12(0);
    }
}

void RELAY13(uint8_t status)
{
    if((status) && (relayFlag->rl13 != status))
    {
        relayFlag->rl13 = status;
        RL13(1);
        // HAL_Delay(50);
    }
    if((!status) && (relayFlag->rl13 != status))
    {
        relayFlag->rl13 = status;
        RL13(0);
    }
}

void RELAY14(uint8_t status)
{
    if((status) && (relayFlag->rl14 != status))
    {
        relayFlag->rl14 = status;
        RL14(1);
        // HAL_Delay(100);
    }
    if((!status) && (relayFlag->rl14 != status))
    {
        relayFlag->rl14 = status;
        RL14(0);
    }
    
}

void RELAY15(uint8_t status)
{
    if((status) && (relayFlag->rl15 != status))
    {
        relayFlag->rl15 = status;
        RL15(1);
        // HAL_Delay(100);
    }
    if((!status) && (relayFlag->rl15 != status))
    {
        relayFlag->rl15 = status;
        RL15(0);
    }
}

void RELAY16(uint8_t status)
{
    if((status) && (relayFlag->rl16 != status))
    {
        relayFlag->rl16 = status;
        RL16(1);
    }
    if((!status) && (relayFlag->rl16 != status))
    {
        relayFlag->rl16 = status;
        RL16(0);
    }
}

void RELAY_Manage(void)
{
    // RELAY1(device.valve_fan);                   // quat hut
    // RELAY2(device.light);                       // den chieu sang
    // RELAY3(device.warning_red);                 // den bao do
    // RELAY4(device.warning_green);               // den bao xanh
    // RELAY5(device.warning_yellow);              // den bao vang
    // RELAY6(device.door);                        // khoa cua
    // RELAY7(device.thermistor3);                 // dien tro nhiet 3
    // RELAY8(device.thermistor1);                 // dien tro nhiet 1
    // RELAY9(device.thermistor2);                 // dien tro nhiet 2
    // RELAY10(device.moise.stt_rl);               // may tao am
    // RELAY11(device.humidify.stt_rl);            // may hut am
    // RELAY12(device.valve_heater);               // van khi nong
    // RELAY13(device.valve_cooler);               // van khi lanh 
}
