/*
* @Author: nhantt
* @Date:   2020-03-03
* @Last Modified by:   nhantt
* @Last Modified time: 2020-03-03
*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

USBD_HandleTypeDef hUsbDeviceFS;

static void USB_GPIO_Init(void);

void MX_USB_DEVICE_Init(void)
{
  USB_GPIO_Init();
  USB_DISABLE();
  HAL_Delay(10);
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }
  USB_ENABLE();
}

static void USB_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  USB_EN_CLOCK();

  HAL_GPIO_WritePin(USB_EN_PORT, USB_EN_PIN, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = USB_EN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_EN_PORT, &GPIO_InitStruct);
}
