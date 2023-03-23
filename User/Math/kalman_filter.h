/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __KALMAN_FILTER_H
#define __KALMAN_FILTER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

typedef struct __Kalman_filter_TypeDef
{
	float _err_measure; 		// sai so do duoc
	float _err_estimate;		// sai so uoc luong
	float _q;					// 
	float _current_estimate;	// uoc luong hien tai
	float _last_estimate;		// uoc luong truoc do
	float _kalman_gain;			//
	float buffer[10];			
	uint8_t cnt;
	uint8_t first_get;
} Kalman_filter_TypeDef;

void Simple_Kalman_Filter(Kalman_filter_TypeDef *kalman,float mea_e, float est_e, float q);
float Update_Estimate(Kalman_filter_TypeDef *kalman,float mea);
void Set_Measurement_Error(Kalman_filter_TypeDef *kalman,float mea_e);
void Set_Estimate_Error(Kalman_filter_TypeDef *kalman,float est_e);
void Set_Process_Noise(Kalman_filter_TypeDef *kalman,float q);
float Get_Kalman_Gain(Kalman_filter_TypeDef *kalman);
float Get_EstimateError(Kalman_filter_TypeDef *kalman);
#endif
