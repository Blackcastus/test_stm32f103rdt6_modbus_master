/*
* @Author: duchien 
* @Date:   2023-03-23
* @Last Modified by:   duchien
* @Last Modified time: 2023-03-23
*/

#ifndef __UART_USER_H__
#define __UART_USER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

/* Configuration --------------------------------------------------------------*/
#define DMA_BUF_SIZE        256      /* DMA circular buffer size in bytes */
#define DMA_TIMEOUT_MS      4        /* DMA Timeout duration in msec, 9600bps = 4ms, 19200bps = 2ms */

typedef struct
{
    volatile uint8_t  flag;     			/* Timeout event flag */
    uint8_t 		  data[1024]; 			/* Data buffer that contains newly received data */
    uint8_t 		  state;				/* State evnt: copy done = 1, 0 is nothing */
    uint16_t 		  length;				/* Length of received data */
    uint16_t 		  timer;            	/* Timeout duration in msec */
    uint16_t 		  prev_cnt;         	/* Holds previous value of DMA_CNDTR */
} DMA_Event_t;

extern DMA_Event_t      dma_uart2_rx;
extern UART_HandleTypeDef huart1, huart2;

void 	MX_USART1_UART_Init(uint32_t baudrate);
uint8_t Uart1_Put_Char(uint8_t data);
void 	Uart1_Send_Data(uint8_t *data, uint16_t length);

void MX_USART1_UART_Init(uint32_t baudrate);
uint8_t Uart2_Put_Char(uint8_t data);
uint8_t Uart2_Send_Data(uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
