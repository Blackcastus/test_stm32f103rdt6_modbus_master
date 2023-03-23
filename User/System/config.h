/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "main.h"

// #include "clock_rtc.h"

#define PAGE_SIZE			(0x800) 	/* 2 KByte */
#define FLASH_SIZE			(0x60000)	/* 384 KBytes*/
#define CONFIG_AREA_SIZE	0x800
// #define CONFIG_AREA_START	((uint32_t)((0x8000000 + FLASH_SIZE) - PAGE_SIZE*(CONFIG_AREA_SIZE/PAGE_SIZE)))
#define CONFIG_AREA_START 	((uint32_t)0x0805F800)

#define CFG_HOLDER			0x00FF55AB

typedef struct {
	uint8_t mbAddr;
	uint8_t mbBaud;
	uint8_t mode_Daikin;
	uint8_t modd_fanspeed_Daikin;

	float calib_Ta[10];
	float calib_Tb[10];
	float calib_Ha[10];
	float calib_Hb[10];

	float settemp;
	float sethumi;

	uint8_t hien;

	uint32_t cfg_holder;
} CFG_STRUCT;

extern CFG_STRUCT sys_cfg;

void CFG_Save(void);
void CFG_Load(void);

#endif