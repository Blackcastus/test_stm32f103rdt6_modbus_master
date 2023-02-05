#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "tim.h"
// #include "timeout.h"

#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0

/* ----------------------- User defenitions ---------------------------------*/
TIM_HandleTypeDef *tim;
static uint16_t timeout = 0;
volatile uint16_t counter = 0;

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us, void *dHTIM )
{
	tim = (TIM_HandleTypeDef *)dHTIM;
	timeout = usTim1Timerout50us;
	return TRUE;
}

inline void vMBPortTimersEnable(  )
{
	counter=0;
	HAL_TIM_Base_Start_IT(tim);
}

inline void vMBPortTimersDisable(  )
{
	HAL_TIM_Base_Stop_IT(tim);
}

void MBslave_Timeout(void)
{
	if ((++counter) >= timeout)
	{
		pxMBPortCBTimerExpired();
	}
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
// 	if(htim->Instance == tim->Instance)
// 	{
// 		if((++counter) >= timeout)
// 			pxMBPortCBTimerExpired();
// 	}
// }

#endif
