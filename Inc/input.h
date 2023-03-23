/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __INPUT_H
#define __INPUT_H

#include "main.h"

#define IN1_PIN GPIO_PIN_4
#define IN2_PIN GPIO_PIN_3
#define IN3_PIN GPIO_PIN_2
#define IN4_PIN GPIO_PIN_12
#define IN5_PIN GPIO_PIN_13
#define IN6_PIN GPIO_PIN_9
#define IN7_PIN GPIO_PIN_8
#define IN8_PIN GPIO_PIN_5

#define IN9_PIN GPIO_PIN_12
#define IN10_PIN GPIO_PIN_1
#define IN11_PIN GPIO_PIN_2
#define IN12_PIN GPIO_PIN_3
#define IN13_PIN GPIO_PIN_5
#define IN14_PIN GPIO_PIN_4
#define IN15_PIN GPIO_PIN_1
#define IN16_PIN GPIO_PIN_0

#define In1()  HAL_GPIO_ReadPin(GPIOB, IN1_PIN)
#define In2()  HAL_GPIO_ReadPin(GPIOB, IN2_PIN)
#define In3()  HAL_GPIO_ReadPin(GPIOD, IN3_PIN)
#define In4()  HAL_GPIO_ReadPin(GPIOC, IN4_PIN)
#define In5()  HAL_GPIO_ReadPin(GPIOC, IN5_PIN)
#define In6()  HAL_GPIO_ReadPin(GPIOB, IN6_PIN)
#define In7()  HAL_GPIO_ReadPin(GPIOB, IN7_PIN)
#define In8()  HAL_GPIO_ReadPin(GPIOB, IN8_PIN)

#define In9()  HAL_GPIO_ReadPin(GPIOB, IN9_PIN)
#define In10()  HAL_GPIO_ReadPin(GPIOB, IN10_PIN)
#define In11()  HAL_GPIO_ReadPin(GPIOB, IN11_PIN)
#define In12()  HAL_GPIO_ReadPin(GPIOB, IN12_PIN)
#define In13()  HAL_GPIO_ReadPin(GPIOA, IN13_PIN)
#define In14()  HAL_GPIO_ReadPin(GPIOA, IN14_PIN)
#define In15()  HAL_GPIO_ReadPin(GPIOA, IN15_PIN)
#define In16()  HAL_GPIO_ReadPin(GPIOA, IN16_PIN)

#define NUM_OF_SAMPLE_INPUT 5
#define NUM_INPUT 16

typedef enum {
	IN1 = 0,
	IN2,
	IN3,
	IN4,
	IN5,
	IN6,
	IN7,
	IN8,
	IN9, 
	IN10,
	IN11,
	IN12,
	IN13,
	IN14,
	IN15,
	IN16
}INPUT_NAME;

typedef enum {
	IN_RELEASE = 0,
	IN_ACTIVE = 1
}INPUT_EVENT;

typedef struct {
	uint8_t actived;
	uint8_t active;
	uint8_t waitRelease;
	uint8_t cnt;
}INPUT_EXT;

typedef struct {
	uint8_t name;
	INPUT_EXT ext[NUM_INPUT];
	uint32_t tick;
}INPUT_STRUCT;

typedef enum {
	NO_INPUT,
	IN1_ON,
	IN1_OFF,
	IN2_ON,
	IN2_OFF,
	IN3_ON,
	IN3_OFF,
	IN4_ON,
	IN4_OFF,
	IN5_ON,
	IN5_OFF,
	IN6_ON,
	IN6_OFF,
	IN7_ON,
	IN7_OFF,
	IN8_ON,
	IN8_OFF,
	IN9_ON,
	IN9_OFF,
	IN10_ON,
	IN10_OFF,
	IN11_ON,
	IN11_OFF,
	IN12_ON,
	IN12_OFF,
	IN13_ON,
	IN13_OFF,
	IN14_ON,
	IN14_OFF,
	IN15_ON,
	IN15_OFF,
	IN16_ON,
	IN16_OFF,
}INPUT_ID;

void Input_Init(void);
void Input_Get(uint8_t port_id);
uint8_t Input_Process(void);
void Input_Manage(void);

#endif