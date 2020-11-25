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
uint16_t  soil_moistutre_sensor_read(ADC_HandleTypeDef* adc, uint32_t delay, uint32_t* adc_value);
float normalized_moisture_level(uint16_t value);
#endif /* INC_SOIL_MOISTURE_SENSOR_H_ */
