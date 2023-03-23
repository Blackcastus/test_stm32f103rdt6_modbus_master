#ifndef _SHT_SENSORS_H
#define _SHT_SENSORS_H

#include "main.h"


#define TEMP_Y1_REG			0
#define TEMP_Y2_REG			1
#define HUMI_Y1_REG			2
#define HUMI_Y2_REG			3
#define ACTION_CALIB_REG 	4



typedef struct
{
	/* calib point */
	float X1; 	/* gia tri point X duoc lay mau khi nhan duoc gia tri point Y */
	float Y1;	/* gia tri point Y nhan tu user*/
	
	float X2; 	
	float Y2;

}SHT_POINT_t;


typedef struct
{
	uint16_t raw; 		/* gia tri tho doc tu cam bien */
	uint16_t user;		/* gia tri tinh toan sau khi hieu chinh tu user */

	float a_user;	/* calculator linear regression */
	float b_user;	/* calculator linear regression */

	SHT_POINT_t point;

}SHT_SENSOR_t;

typedef struct
{
	uint8_t id;			/* name id sensors */
	uint8_t state; 		/* 0: no error; 1: error */
	SHT_SENSOR_t temp;
	SHT_SENSOR_t humi;
	
}SHT_DEVICE_t;

void SHT_Init_Load(void);
void SHT_Read_Process(void); // Doc tat ca cac cam bien SHT
void SHT_Get_Value(uint8_t id, uint16_t t, uint16_t h);
void SHT_Get_Calib_Value(uint8_t address, uint16_t value);

#endif