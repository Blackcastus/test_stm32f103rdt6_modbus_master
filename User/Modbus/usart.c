#include "usart.h"
#include "fifo.h"
#include "modbus_crc.h"

UART_HandleTypeDef huart1, huart2;
DMA_HandleTypeDef hdma_usart1_rx, hdma_usart2_rx;

uint8_t           dma_rx1_buf[DMA_BUF_SIZE], dma_rx2_buf[DMA_BUF_SIZE],/* Circular buffer for DMA */
				  ring_buf_uart1_tx[1024], ring_buf_uart2_tx[1024], tx1_data, tx2_data;
DMA_Event_t       dma_uart1_rx = {0, {'\0'}, 0, 0, 0, DMA_BUF_SIZE}, 
				  dma_uart2_rx = {0, {'\0'}, 0, 0, 0, DMA_BUF_SIZE};
Fifo_t            fifo_uart1_tx, fifo_uart2_tx;		

/**
 * Uart1 connect to raspberry
 */
void Uart1_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 19200;
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

    /* Init tx fifo */
    FifoInit(&fifo_uart1_tx, ring_buf_uart1_tx, 1024);

    /* UART1 IDLE interrupt configuration */
    SET_BIT(USART1->CR1, USART_CR1_IDLEIE);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);

    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

     /* Start DMA Rx */
    if(HAL_UART_Receive_DMA(&huart1, (uint8_t*)dma_rx1_buf, DMA_BUF_SIZE) != HAL_OK)
    {        
        Error_Handler();
    }
    
    /* Disable Half Transfer Interrupt */
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_ERR);

}


void Uart2_Init(uint32_t baudrate)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = baudrate;
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

    /* Init tx fifo */
    FifoInit(&fifo_uart2_tx, ring_buf_uart2_tx, 1024);

    /* UART1 IDLE interrupt configuration */
    SET_BIT(USART2->CR1, USART_CR1_IDLEIE);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(&huart2,hdmarx,hdma_usart2_rx);

    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

     /* Start DMA */
    if(HAL_UART_Receive_DMA(&huart2, (uint8_t*)dma_rx2_buf, DMA_BUF_SIZE) != HAL_OK)
    {        
        Error_Handler();
    }
    
    /* Disable Half Transfer Interrupt */
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_ERR);
}



/**
 * [Uart_Put_Char description]
 */
uint8_t Uart2_Put_Char(uint8_t data)
{
    uint32_t old_primask;

    old_primask = __get_PRIMASK();
    __disable_irq();

    if (!IsFifoFull(&fifo_uart2_tx))
    {
        FifoPush(&fifo_uart2_tx, data);
        
        /* Trig UART Tx interrupt to start sending the FIFO contents */
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);

        __set_PRIMASK(old_primask);
        return 0;       /* OK */
    }

    __set_PRIMASK(old_primask);
    return 1;   /* Busy */
}


/**
 * [Uart_Rs485_Send_Data description]
 */
uint8_t Uart2_Send_Data(uint8_t *data, uint16_t length)
{
    uint32_t retry_count;
    // uint8_t putchar_status;

    for (uint32_t i = 0; i < length; ++i)
    {
        retry_count = 0;
        
        while(Uart2_Put_Char(data[i]))        /* Busy */
        {
            retry_count++;
            if (retry_count > 5)
                return 1;   /* Busy */
        }
    }
    return 0;   /* OK */
}


/**
 * [Uart_Put_Char description]
 */
uint8_t Uart1_Put_Char(uint8_t data)
{
    uint32_t old_primask;

    old_primask = __get_PRIMASK();
    __disable_irq();

    if (!IsFifoFull(&fifo_uart1_tx))
    {
        FifoPush(&fifo_uart1_tx, data);
        
        /* Trig UART Tx interrupt to start sending the FIFO contents */
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_TC);

        __set_PRIMASK(old_primask);
        return 0;       /* OK */
    }

    __set_PRIMASK(old_primask);
    return 1;   /* Busy */
}


/**
 * [Uart_Rs485_Send_Data description]
 */
void Uart1_Send_Data(uint8_t *data, uint16_t length)
{
    uint32_t retry_count;

    for (uint32_t i = 0; i < length; ++i)
    {
        retry_count = 0;
        while(Uart1_Put_Char(data[i]))        /* Busy */
        {
            retry_count++;
            if (retry_count > 5)
                break;
        }
    }
}


/**
 * DMA Rx complete AND DMA Rx Timeout function
 * Timeout event: duoc tao ra sau UART IDLE IT + DMA Timeout value
 * Callback nay duoc goi trong 2 th:
 *     1. Khi DMA nhan du SIZE (TC flag)
 *     2. Khi timeout xay ra 
 *     
 * @param huart [description]
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint16_t start, length;
    uint16_t curr_cnt;

    curr_cnt = __HAL_DMA_GET_COUNTER(huart->hdmarx);       /* So byte con lai */

    if (huart->Instance == USART1)
    {
    	if (dma_uart1_rx.flag && curr_cnt == DMA_BUF_SIZE)
	    {
	        dma_uart1_rx.state = 1;
	        dma_uart1_rx.flag = 0;
	        return;
	    }  

	    /* Determine start position in DMA buffer based on previous counter value */
	    start = (dma_uart1_rx.prev_cnt < DMA_BUF_SIZE) ? (DMA_BUF_SIZE - dma_uart1_rx.prev_cnt) : 0;

	    if (dma_uart1_rx.flag)          /* Timeout event */
	    {
	        length = (dma_uart1_rx.prev_cnt < DMA_BUF_SIZE) ? (dma_uart1_rx.prev_cnt - curr_cnt) : (DMA_BUF_SIZE - curr_cnt);
	        dma_uart1_rx.prev_cnt = curr_cnt;  
	        dma_uart1_rx.flag = 0;
	        /* Push to Fifo */
	        for(uint32_t i = 0; i < length; ++i)
	            dma_uart1_rx.data[dma_uart1_rx.length++] = dma_rx1_buf[start++];
	        dma_uart1_rx.state = 1;
	    }
	    else        /* DMA Rx Complete event */
	    {
	        length = DMA_BUF_SIZE - start;
	        dma_uart1_rx.prev_cnt = DMA_BUF_SIZE;

	        /* Push to Fifo */
	        for(uint32_t i = 0; i < length; ++i)
	            dma_uart1_rx.data[dma_uart1_rx.length++] = dma_rx1_buf[start++];
	    }
    }
    else
    {
    	if (dma_uart2_rx.flag && curr_cnt == DMA_BUF_SIZE)
        {
            dma_uart2_rx.state = 1;
            dma_uart2_rx.flag = 0;
            return;
        } 
    
        /* Determine start position in DMA buffer based on previous counter value */
        start = (dma_uart2_rx.prev_cnt < DMA_BUF_SIZE) ? (DMA_BUF_SIZE - dma_uart2_rx.prev_cnt) : 0;

        if (dma_uart2_rx.flag)          /* Timeout event */
        {
            length = (dma_uart2_rx.prev_cnt < DMA_BUF_SIZE) ? (dma_uart2_rx.prev_cnt - curr_cnt) : (DMA_BUF_SIZE - curr_cnt);
            dma_uart2_rx.prev_cnt = curr_cnt;    
            dma_uart2_rx.flag = 0;
            /* Push to Fifo */
            for(uint32_t i = 0; i < length; ++i)
                dma_uart2_rx.data[dma_uart2_rx.length++] = dma_rx2_buf[start++];
            dma_uart2_rx.state = 1;
        }
        else        /* DMA Rx Complete event */
        {
            length = DMA_BUF_SIZE - start;
            dma_uart2_rx.prev_cnt = DMA_BUF_SIZE;
            /* Push to Fifo */
            for(uint32_t i = 0; i < length; ++i)
                dma_uart2_rx.data[dma_uart2_rx.length++] = dma_rx2_buf[start++];
        }
    }
    
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
    	HAL_UART_Receive_DMA(&huart1, (uint8_t*)dma_rx1_buf, DMA_BUF_SIZE);
   	else
   		HAL_UART_Receive_DMA(&huart2, (uint8_t*)dma_rx2_buf, DMA_BUF_SIZE);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		if (!IsFifoEmpty(&fifo_uart1_tx)) 
	    {
	        tx1_data = FifoPop(&fifo_uart1_tx);
	        HAL_UART_Transmit_IT(&huart1, &tx1_data, 1);
	    } 
	}
	else
	{
		if (!IsFifoEmpty(&fifo_uart2_tx)) 
        {
            tx2_data = FifoPop(&fifo_uart2_tx);
            HAL_UART_Transmit_IT(&huart2, &tx2_data, 1);
        }
	}
    
}














