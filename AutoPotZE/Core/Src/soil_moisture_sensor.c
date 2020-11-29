/*
 * soil_moisture_sensor.c
 *
 *  Created on: Nov 10, 2020
 *      Author: Dell
 */
#include "soil_moisture_sensor.h"

 float get_normalized_moisture_level(uint16_t value){
	return (((float)value)/4096)*100;
 }

uint16_t start_soil_moisture_measurement(ADC_HandleTypeDef* adc, DMA_HandleTypeDef* hdma_adc, uint16_t* value){
	  __HAL_LINKDMA(adc,DMA_Handle,*hdma_adc);
		if(HAL_DMA_Init(hdma_adc) != HAL_OK){
			return 0;
		}
		if(HAL_ADC_Start_DMA(adc, value, 1) != HAL_OK){
			return 0;
		}
		return 1;
}
uint16_t stop_soil_measurement(ADC_HandleTypeDef* adc){
	if(HAL_ADC_Stop_DMA(adc) != HAL_OK){
		return 0;
	}
	return 1;
}


