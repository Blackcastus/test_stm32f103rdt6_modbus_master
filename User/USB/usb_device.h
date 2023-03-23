/*
* @Author: nhantt
* @Date:   2020-03-03
* @Last Modified by:   nhantt
* @Last Modified time: 2020-03-03
*/

#ifndef __USB_DEVICE__H__
#define __USB_DEVICE__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "usbd_def.h"

#define USB_EN_PORT   GPIOA
#define USB_EN_PIN    GPIO_PIN_15
#define USB_EN_CLOCK() __HAL_RCC_GPIOA_CLK_ENABLE()

#define USB_ENABLE()    HAL_GPIO_WritePin(USB_EN_PORT, USB_EN_PIN, GPIO_PIN_SET)
#define USB_DISABLE()   HAL_GPIO_WritePin(USB_EN_PORT, USB_EN_PIN, GPIO_PIN_RESET)

void MX_USB_DEVICE_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __USB_DEVICE__H__ */