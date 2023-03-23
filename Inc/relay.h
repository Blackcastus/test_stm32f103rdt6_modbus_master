/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __RELAY_H__
#define __RELAY_H__

#include "main.h"

#define RL1_PIN	       GPIO_PIN_8
#define RL2_PIN	       GPIO_PIN_9
#define RL3_PIN		   GPIO_PIN_8
#define RL4_PIN		   GPIO_PIN_7
#define RL5_PIN		   GPIO_PIN_6
#define RL6_PIN	       GPIO_PIN_15
#define RL7_PIN	       GPIO_PIN_14
#define RL8_PIN	       GPIO_PIN_13
#define RL9_PIN        GPIO_PIN_12
#define RL10_PIN       GPIO_PIN_2
#define RL11_PIN       GPIO_PIN_1
#define RL12_PIN       GPIO_PIN_0
#define RL13_PIN       GPIO_PIN_5
#define RL14_PIN       GPIO_PIN_4
#define RL15_PIN       GPIO_PIN_7
#define RL16_PIN       GPIO_PIN_6


typedef struct
{
    uint8_t rl1;
    uint8_t rl2;
    uint8_t rl3;
    uint8_t rl4;
    uint8_t rl5;
    uint8_t rl6;
    uint8_t rl7;
    uint8_t rl8;
    uint8_t rl9;
    uint8_t rl10;
    uint8_t rl11;
    uint8_t rl12;
    uint8_t rl13;
    uint8_t rl14;
    uint8_t rl15;
    uint8_t rl16;
} RL_STRUCT_T;

typedef enum
{
    RELAY_POWER = 0,
    RELAY_LAMP,
    RELAY_UV1,
    RELAY_UV2,
    RELAY_FAN,
    RELAY_SOCKET,
    RELAY_DOOR_UP,
    RELAY_DOOR_DOWN
} RELAY_NAME;

extern RL_STRUCT_T *relayFlag;

// | 5       | Int (Read/Wite)  | Máy tách ẩm                          | 0: OFF, 1: On
// | 6       | Int (Read/Wite)  | Vỉ tạo ẩm                            | 0: OFF, 1: On
// | 7       | Int (Read/Wite)  | Điệt trở nhiệt 1                     | 0: OFF, 1: On
// | 8       | Int (Read/Wite)  | Điệt trở nhiệt 2                     | 0: OFF, 1: On
// | 9       | Int (Read/Wite)  | Điệt trở nhiệt 3                     | 0: OFF, 1: On
// | 10      | Int (Read/Wite)  | Đèn chiếu sáng                       | 0: OFF, 1: On
// | 11      | Int (Read/Wite)  | Đèn báo xanh                         | 0: OFF, 1: On
// | 12      | Int (Read/Wite)  | Đèn báo đỏ                           | 0: OFF, 1: On
// | 13      | Int (Read/Wite)  | Đèn báo vàng                         | 0: OFF, 1: On
// | 14      | Int (Read/Wite)  | Khóa cửa                             | 0: OFF, 1: On
// | 15      | Int (Read/Wite)  | Quạt hút                             | 0: OFF, 1: On
// | 16      | Int (Read/Wite)  | Van khí nóng                         | 0: OFF, 1: On
// | 17      | Int (Read/Wite)  | Van khí lạnh                         | 0: OFF, 1: On

#define RL1(x)		x?HAL_GPIO_WritePin(GPIOA, RL1_PIN, RESET):HAL_GPIO_WritePin(GPIOA, RL1_PIN, SET)
#define RL2(x)		x?HAL_GPIO_WritePin(GPIOC, RL2_PIN, RESET):HAL_GPIO_WritePin(GPIOC, RL2_PIN, SET)
#define RL3(x)		x?HAL_GPIO_WritePin(GPIOC, RL3_PIN, RESET):HAL_GPIO_WritePin(GPIOC, RL3_PIN, SET)
#define RL4(x)		x?HAL_GPIO_WritePin(GPIOC, RL4_PIN, RESET):HAL_GPIO_WritePin(GPIOC, RL4_PIN, SET)
#define RL5(x)		x?HAL_GPIO_WritePin(GPIOC, RL5_PIN, RESET):HAL_GPIO_WritePin(GPIOC, RL5_PIN, SET)
#define RL6(x)		x?HAL_GPIO_WritePin(GPIOB, RL6_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL6_PIN, SET)
#define RL7(x)		x?HAL_GPIO_WritePin(GPIOB, RL7_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL7_PIN, SET)
#define RL8(x)		x?HAL_GPIO_WritePin(GPIOB, RL8_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL8_PIN, SET)

#define RL9(x)       x?HAL_GPIO_WritePin(GPIOB, RL9_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL9_PIN, SET)
#define RL10(x)      x?HAL_GPIO_WritePin(GPIOB, RL10_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL10_PIN, SET)
#define RL11(x)      x?HAL_GPIO_WritePin(GPIOB, RL11_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL11_PIN, SET)
#define RL12(x)      x?HAL_GPIO_WritePin(GPIOB, RL12_PIN, RESET):HAL_GPIO_WritePin(GPIOB, RL12_PIN, SET)
#define RL13(x)      x?HAL_GPIO_WritePin(GPIOC, RL13_PIN, RESET):HAL_GPIO_WritePin(GPIOC, RL13_PIN, SET)
#define RL14(x)      x?HAL_GPIO_WritePin(GPIOC, RL14_PIN, RESET):HAL_GPIO_WritePin(GPIOC, RL14_PIN, SET)
#define RL15(x)      x?HAL_GPIO_WritePin(GPIOA, RL15_PIN, RESET):HAL_GPIO_WritePin(GPIOA, RL15_PIN, SET)
#define RL16(x)      x?HAL_GPIO_WritePin(GPIOA, RL16_PIN, RESET):HAL_GPIO_WritePin(GPIOA, RL16_PIN, SET)

void RELAY_Init(void);
void RELAY_Manage(void);

#endif