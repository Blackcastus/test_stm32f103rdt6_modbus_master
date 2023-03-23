#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define ADC_SCALE	4096
#define VREF		3.3
#define DEFAULT_FREQUENCY	50
#define ADC_SAMPLE 	100

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

typedef enum
{
	ACS712_05A,
	ACS712_20A,
	ACS712_30A
} ACS712_Type;

typedef struct
{
	int zero; //4096/2;
	float current;
	float sensitivity;
	uint32_t tick;
	uint16_t adcValue[ADC_SAMPLE];
	uint8_t flag;
	uint8_t sample;
} ACS712_Struct;

void ACS712_Init(ACS712_Type type);
int ACS712_Calibrate(void);
void ACS712_SetZeroPoint(int zero);
void ACS712_SetSensitivity(float sens);
float ACS712_GetCurrentDC(void);
float ACS712_GetCurrentAC(uint16_t freq);
void ACS712_Manager(void);
float ACS712_GetValue(void);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
