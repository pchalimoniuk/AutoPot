/*
 * soil_moisture_sensor.c
 *
 *  Created on: Nov 10, 2020
 *      Author: Dell
 */
#include "soil_moisture_sensor.h"
const uint16_t DRY_SOIL_LEVEL = 30;
const uint16_t_SOIL_LEVEL = 70;
void soil_moistutre_sensor_prepare_ADC(ADC_HandleTypeDef* adc );
void  soil_moistutre_sensor_prepare_DMA(DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc);

 uint16_t  soil_moistutre_sensor_read(ADC_HandleTypeDef* adc, uint32_t delay, uint32_t* adc_value){
	 soil_moistutre_sensor_prepare_ADC(adc);
	 DMA_HandleTypeDef dma ={0};
	 soil_moistutre_sensor_prepare_DMA(&dma, adc);
	 HAL_ADC_Start_DMA(adc, adc_value, sizeof(adc_value));
	 return 1;
 }
 float normalized_moisture_level(uint16_t value){
	 return (100.0f);
 }

void  soil_moistutre_sensor_prepare_DMA(DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc){
	 dma->Instance = DMA_CHANNEL_0;
	 dma->Init.Direction = DMA_PERIPH_TO_MEMORY;
	 dma->Init.PeriphInc = DMA_PINC_DISABLE;
	 dma->Init.MemInc = DMA_MINC_DISABLE;
	 dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	 dma->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	 dma->Init.Mode = DMA_NORMAL;
	 dma->Init.Priority = DMA_PRIORITY_LOW;
	 HAL_DMA_Init(dma);
	 __HAL_LINKDMA(adc, DMA_Handle, *dma);

}
void Error_Handler_ADC(){
	while(1);
}

void soil_moistutre_sensor_prepare_ADC(ADC_HandleTypeDef* adc ){

    adc->Instance = ADC1;
    adc->Init.ContinuousConvMode = ENABLE;
    adc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc->Init.ScanConvMode = DISABLE;
    adc->Init.NbrOfConversion = 1;
    adc->Init.DiscontinuousConvMode = DISABLE;
    adc->Init.NbrOfDiscConversion = 1;
	HAL_ADC_Init(adc);

	if (HAL_ADC_Init(adc) != HAL_OK)
	{
		Error_Handler_ADC();
	}
}
