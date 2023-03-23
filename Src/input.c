/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "input.h"
#include "relay.h"

// #include "config.h"
#include "modbus_data.h"

// #include "control_system.h"

// extern DEVICE_t device; 

INPUT_STRUCT input;

void Input_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = IN1_PIN | IN2_PIN | IN6_PIN | IN7_PIN | IN8_PIN | IN9_PIN | IN10_PIN | IN11_PIN | IN12_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IN13_PIN | IN14_PIN | IN15_PIN | IN16_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IN3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IN4_PIN | IN5_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    for (input.name = 0; input.name < NUM_INPUT; input.name++)
    {
        input.ext[input.name].active = input.ext[input.name].actived = IN_RELEASE;
        input.ext[input.name].waitRelease = 0;
        input.ext[input.name].cnt = 0;
    }
    input.tick = HAL_GetTick();
}

void Input_Get(uint8_t port_id)
{
    uint16_t inputGet;
    switch (port_id)
    {
    case 0:
        inputGet = In1();
        if (inputGet == !IN1_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 1:
        inputGet = In2();
        if (inputGet == !IN2_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 2:
        inputGet = In3();
        if (inputGet == !IN3_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 3:
        inputGet = In4();
        if (inputGet == !IN4_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 4:
        inputGet = In5();
        if (inputGet == !IN5_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 5:
        inputGet = In6();
        if (inputGet == !IN6_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 6:
        inputGet = In7();
        if (inputGet == !IN7_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 7:
        inputGet = In8();
        if (inputGet == !IN8_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 8:
        inputGet = In9();
        if (inputGet == !IN9_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 9:
        inputGet = In10();
        if (inputGet == !IN10_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 10:
        inputGet = In11();
        if (inputGet == !IN11_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 11:
        inputGet = In12();
        if (inputGet == !IN12_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 12:
        inputGet = In13();
        if (inputGet == !IN13_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
     case 13:
        inputGet = In14();
        if (inputGet == !IN14_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
     case 14:
        inputGet = In15();
        if (inputGet == !IN15_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 15:
        inputGet = In16();
        if (inputGet == !IN16_PIN)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    }

    if (input.ext[port_id].active != input.ext[port_id].actived)
    {
        ++(input.ext[port_id].cnt);
        if (input.ext[port_id].cnt == NUM_OF_SAMPLE_INPUT)
        {
            input.ext[port_id].actived = input.ext[port_id].active;
            input.ext[port_id].cnt = 0;
        }
    }
    else
    {
        input.ext[port_id].cnt = 0;
    }
}

uint8_t Input_Process(void)
{
    uint8_t currentState = 0;
    for (input.name = 0; input.name < NUM_INPUT; input.name++)
    {
        Input_Get(input.name);

        if (input.ext[input.name].actived == IN_ACTIVE)
        {
            if (input.ext[input.name].waitRelease == 0)
            {
                switch (input.name)
                {
                
                    case IN4:
                        currentState = IN4_ON;
                        printf("INPUT4 ON\n");
                        device.sensor_door = 0;
                        // device.light = 0;

                        break;
                
                default:
                    currentState = NO_INPUT;
                    break;
                }
            }
            input.ext[input.name].waitRelease = 1;
        }
        if (input.ext[input.name].actived == IN_RELEASE)
        {
            if (input.ext[input.name].waitRelease == 1)
            {
                switch (input.name)
                {
                    
                    case IN4:
                        currentState = IN4_OFF;
                        printf("INPUT4 OFF\n");
                        device.sensor_door = 1;
                        // device.light = 1;

                        break;
                
                default:
                    currentState = NO_INPUT;
                    break;
                }
            }
            input.ext[input.name].waitRelease = 0;
        }
    }
    return currentState;
}

void Input_Manage(void)
{
    if (HAL_GetTick() - input.tick > 10)
    {
        Input_Process();
        
        input.tick = HAL_GetTick();
    }
}
