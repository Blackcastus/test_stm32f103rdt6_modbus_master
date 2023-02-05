/*
* @Author: nhantt
* @Date:   2020-07-01
* @Last Modified by:   nhantt
* @Last Modified time: 2020-07-01
*/

#include "usart.h"
#include "ringbuf.h"
#include "syringe.h"
#include <string.h>
#include <stdio.h>

#include "mb.h"
#include "mb_m.h"
#include "port.h"
#include "mbport.h"
#include "portserial_m.h"
#include "portserial.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

extern uint8_t singlechar;
extern uint8_t singlechar_m;

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* UART Reveice IT */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
    // Mbslave_uart_rx_handler();
		
		// HAL_UART_Receive_IT(huart, &singlechar, 1);
	}
  else if(huart->Instance == huart2.Instance)
	{
    MBmaster_uart_rx_handhler();
		
		HAL_UART_Receive_IT(huart, &singlechar_m, 1);
	}
}

// /* UART Trebsmit IT*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
    // Mbslave_uart_tx_handler();

	}
  else if(huart->Instance == huart2.Instance)
	{
    MBmaster_uart_tx_handhler();
	}
}
