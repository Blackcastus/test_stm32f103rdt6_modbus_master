/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "kalman_filter.h"

void Simple_Kalman_Filter(Kalman_filter_TypeDef *kalman, float mea_e, float est_e, float q)
{
	kalman->_err_measure = mea_e;
	kalman->_err_estimate = est_e;
	kalman->_q = q;
}

float Update_Estimate(Kalman_filter_TypeDef *kalman, float mea)
{
	kalman->_kalman_gain = kalman->_err_estimate/(kalman->_err_estimate + kalman->_err_measure);
	kalman->_current_estimate = kalman->_last_estimate + kalman->_kalman_gain * (mea - kalman->_last_estimate);
	kalman->_err_estimate = (1.0 - kalman->_kalman_gain)*kalman->_err_estimate + fabs(kalman->_last_estimate - kalman->_current_estimate)*kalman->_q;
	kalman->_last_estimate = kalman->_current_estimate;

	return kalman->_current_estimate;
}

void Set_Measurement_Error(Kalman_filter_TypeDef *kalman, float mea_e)
{
	kalman->_err_measure = mea_e;
}

void Set_Estimate_Error(Kalman_filter_TypeDef *kalman, float est_e)
{
	kalman->_err_estimate = est_e;
}

void Set_Process_Noise(Kalman_filter_TypeDef *kalman, float q)
{
	kalman->_q = q;
}

float Get_Kalman_Gain(Kalman_filter_TypeDef *kalman)
{
	return kalman->_kalman_gain;
}

float Get_EstimateError(Kalman_filter_TypeDef *kalman)
{
	return kalman->_err_estimate;
}
