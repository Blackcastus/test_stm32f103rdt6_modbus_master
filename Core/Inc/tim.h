/*
* @Author: nhantt
* @Date:   2020-07-01
* @Last Modified by:   nhantt
* @Last Modified time: 2020-07-01
*/

#ifndef __TIM_H
#define __TIM_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;

void MX_TIM4_Init(void);
void MX_TIM3_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */