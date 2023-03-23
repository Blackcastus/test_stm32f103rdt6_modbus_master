/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "config.h"
#include <string.h>
#include <stdio.h>
CFG_STRUCT sys_cfg;
#define DATA_32                 ((uint32_t)0x12345678)

uint32_t Address = 0, PAGEError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

void CFG_Save(void)
{
	uint32_t *cfgdest, i;
	cfgdest = (uint32_t*)&sys_cfg;
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = CONFIG_AREA_START;
	EraseInitStruct.NbPages = 2;

	for(i = 0; i <= sizeof(CFG_STRUCT)/PAGE_SIZE; i++)
	{
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	  	{
	  	}
	}
	Address = CONFIG_AREA_START;
	for(i = 0; i < sizeof(CFG_STRUCT); i+= 4)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address + i, *(uint32_t*)(cfgdest + i/4)) == HAL_OK);
		if(*(uint32_t*)(cfgdest + i/4) != *(uint32_t*)(Address + i))
			return;
	}
	HAL_FLASH_Lock();
}

void CFG_Load(void)
{
	memcpy(&sys_cfg, (void*)CONFIG_AREA_START, sizeof(CFG_STRUCT));

	if(sys_cfg.cfg_holder != CFG_HOLDER)
	{
		memset(&sys_cfg, 0x00, sizeof sys_cfg);
		sys_cfg.mbAddr = 0x01;
		sys_cfg.mbBaud = 0;
		
		sys_cfg.settemp = 30.0;
		sys_cfg.sethumi = 25.0;
		sys_cfg.mode_Daikin = 3;
		sys_cfg.hien = 1;

		for(int i=0;i<10;i++) {
			sys_cfg.calib_Ta[i] = 1.0;
			sys_cfg.calib_Tb[i] = 0.0;
			sys_cfg.calib_Ha[i] = 1.0;
			sys_cfg.calib_Hb[i] = 0.0;
		}
		sys_cfg.cfg_holder = CFG_HOLDER;

		CFG_Save();
	}
}
