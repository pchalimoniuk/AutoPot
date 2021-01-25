/*
 * soil_moisture_sensor.h
 *
 *  Created on: Nov 10, 2020
 *      Author: Dell
 */


#ifndef INC_SOIL_MOISTURE_SENSOR_H_
#define INC_SOIL_MOISTURE_SENSOR_H_
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
uint16_t get_normalized_moisture_level(uint16_t value);
uint8_t start_soil_moisture_measurement(ADC_HandleTypeDef* adc, DMA_HandleTypeDef* hdma_adc, uint32_t* value);
uint8_t stop_soil_measurement(ADC_HandleTypeDef* adc);
#endif /* INC_SOIL_MOISTURE_SENSOR_H_ */
