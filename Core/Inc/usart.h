/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2020-07-01
*/

#ifndef __USART_H
#define __USART_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
