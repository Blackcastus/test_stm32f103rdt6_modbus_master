#ifndef _CONTROL_SYSTEM_H_
#define _CONTROL_SYSTEM_H_

#include "main.h"
#include "stm32f1xx_hal.h"

#define ID_DAIKIN           11
#define VALVE_OFF           0
#define VALVE_COOL          1
#define VALVE_HEAT          2

#define THERMISTOR1_ON      1
#define THERMISTOR2_ON      2
#define THERMISTOR3_ON      3
#define THERMISTOR_ALL_OFF  0
#define TIME_WAIT_DEVICE    60000
#define DENTA_ERR           2
#define ACTIVE_ON           1
#define TIMEOUT             3000
#define SAMPLE_TIME_60M         3600000

typedef struct 
{
    uint8_t status;      // Trang thai hoat dong cua thiet bi 0: OFF; 1: ON; 2: ERROR
    uint8_t stt_rl;      // trang thai die khien relay
    float val_sample;    // val
    uint32_t tick;       // sample time start couter time out

}WARNING_t;

typedef struct 
{
    uint8_t active;             // Start/Stop chuong trinh

    uint8_t temp_daikin;        // gia tri nhiet do truyen cho may lanh daikin
    WARNING_t cooler;

    WARNING_t heater;             // Trang thai may gia nhiet: 0: off, 1: on

    WARNING_t moise;              // Trang thai may tao am

    WARNING_t humidify;           // Trang thai may tach am

    uint8_t thermistor1;        // Trang thai dien troi nhiet 1
    uint8_t thermistor2;        // Trang thai dien troi nhiet 2
    uint8_t thermistor3;        // Trang thai dien troi nhiet 3

    uint8_t light;              // Trang thai den
    uint8_t warning_red;        // Trang thai den canh bao do
    uint8_t warning_green;      // Trang thai den canh bao xanh
    uint8_t warning_yellow;     // Trang thai den canh bao vang, bao loi
    uint8_t valve_cooler;       // Trang thai van khi lanh
    uint8_t valve_heater;       // Trang thai van khi nong
    uint8_t valve_fan;          // Trang thai van quat gio

    uint8_t door;               // Trang thai cua
    uint8_t sensor_door;        // Trang thai cam bien cua
    

    float temp_now;             // gia tri nhiet do hien tai trong phong
    float humi_now;             // gia tri do am hien tai trong phong
    
    float temp_set;             // gia tri nhiet do set tu nguoi dung
    float humi_set;             // gia tri do am set tu nguoi dung

}DEVICE_t;




void Ctrl_System_Init_Load(void);
void Ctrl_System_Device_Manage(void);



#endif