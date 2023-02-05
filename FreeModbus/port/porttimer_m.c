#include "port.h"
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "tim.h"
#include "timeout.h"
#include "porttimer_m.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0

/* ----------------------- User defenitions ---------------------------------*/
extern TIM_HandleTypeDef *tim;
extern uint16_t timeout;
extern volatile uint16_t counter;

TIM_HandleTypeDef *tim_m;
static uint16_t timeout_m = 0;
volatile uint16_t counter_m = 0;
static TO_TypeDef _to_master;
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit( USHORT usTimeOut50us, void *dHTIM )
{
	tim_m = (TIM_HandleTypeDef *)dHTIM;
	timeout_m = usTimeOut50us;
    return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
	counter_m = 0;
	// TO_Start(&_to_master, 5);
	HAL_TIM_Base_Start_IT(tim_m);
}

void vMBMasterPortTimersConvertDelayEnable()
{
	vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
	vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
}

void vMBMasterPortTimersDisable()
{
	HAL_TIM_Base_Stop_IT(tim_m);
	// TO_Stop(&_to_master);
}

void MBmaster_Timeout(void)
{
	if ((++counter_m) >= timeout_m){
		pxMBMasterPortCBTimerExpired();
	}
	// printf("b\n");
}


#endif
