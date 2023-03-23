# Modbus Master 

## Init file.c and file.h

    1. Modbus/fifo.x
    2. Modbus/modbus_crc.x
    3. Modbus/modbus_data.x
    4. Modbus/uart.x

## Edit file.c and file.h

    1. stm32f1xx_it.c
        /* You should init your uart and edit my variable */

        extern DMA_HandleTypeDef hdma_usart2_rx;
        extern DMA_Event_t dma_uart2_rx;
        extern UART_HandleTypeDef huart2;

        /**
        * @brief This function handles System tick timer.
        */
        void SysTick_Handler(void)
        {
            /* USER CODE BEGIN SysTick_IRQn 0 */

            if (dma_uart2_rx.timer == 1)
            {
                /* DMA timeout event: set Timeout Flag and call DMA Rx complete callback */
                dma_uart2_rx.flag = 1;
                hdma_usart2_rx.XferCpltCallback(&hdma_usart2_rx);
            }

            if (dma_uart2_rx.timer)
                --dma_uart2_rx.timer;

            /* USER CODE END SysTick_IRQn 0 */
            HAL_IncTick();
            /* USER CODE BEGIN SysTick_IRQn 1 */

            /* USER CODE END SysTick_IRQn 1 */
        }

        void USART2_IRQHandler(void)
        {
            if (LL_USART_IsActiveFlag_IDLE(USART2))
            {
                /* Clear IDLE line flag */
                LL_USART_ClearFlag_IDLE(USART2);
                /* Sart DMA timer */
                dma_uart2_rx.timer = 2;
            }
            HAL_UART_IRQHandler(&huart2);
            }void USART2_IRQHandler(void)
            {
            if (LL_USART_IsActiveFlag_IDLE(USART2))
            {
                /* Clear IDLE line flag */
                LL_USART_ClearFlag_IDLE(USART2);
                /* Sart DMA timer */
                dma_uart2_rx.timer = 2;
            }
            HAL_UART_IRQHandler(&huart2);
        }

        /**
        * @brief This function handles DMA1 channel5 global interrupt.
        */
        void DMA1_Channel6_IRQHandler(void)
        {
            /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

            /* USER CODE END DMA1_Channel5_IRQn 0 */
            HAL_DMA_IRQHandler(&hdma_usart2_rx);
            /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

            /* USER CODE END DMA1_Channel5_IRQn 1 */
        }

### Demo code
    #include "usart.h"
    #include "modbus_data.h"

    #define REQUEST_SLAVE  1000

    int _write(int file, char *data, int len);
    void SystemClock_Config(void);
    uint32_t tick_;
    uint32_t time_reqest_slave;

    int main(void)
    {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USB_DEVICE_Init();
    HAL_Delay(4000);

    /* init modbus master */
    Modbus_Master_DMA_Init(&huart2, 9600);

    while (1)
    {
        /* Process UART2 - Modbus Master */
        
        Modbus_Master_DMA_Process();
        if(HAL_GetTick() - time_reqest_slave > REQUEST_SLAVE)
        {
        Modbus_Master_Read_HoldRegs(1, 0, 2, 100);
        time_reqest_slave = HAL_GetTick();
        }
        if(HAL_GetTick() - tick_ > 1000)
        {
        if(slave.status == 0)
        {
            printf("Temp: %d | humi: %d\n", modbus_master_hold_buf[0][0], modbus_master_hold_buf[0][1]);
        }
        else
            printf("error code %d @@\n", slave.status);
        tick_ = HAL_GetTick();
        }
    }
    }
