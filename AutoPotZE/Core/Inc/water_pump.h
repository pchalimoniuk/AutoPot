/*
 * water_pump.h
 *
 *  Created on: 29 Nov 2020
 *      Author: Dell
 */

#ifndef INC_WATER_PUMP_H_
#define INC_WATER_PUMP_H_
#include "stm32f4xx_hal.h"
#include "stdlib.h"
typedef struct {
	uint8_t water_level;
	TIM_HandleTypeDef *tim;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint8_t counter;
	uint8_t watering_time;
}Water_pump_struct;
void WATER_PUMP_set_water_level(Water_pump_struct* pump, uint8_t water_level);
void WATER_PUMP_set_tim(Water_pump_struct* pump,TIM_HandleTypeDef* tim);
void WATER_PUMP_set_GPIO_port(Water_pump_struct* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WATER_PUMP_init(Water_pump_struct* pump);
void WATER_PUMP_set_watering_time(Water_pump_struct * pump, uint8_t watering_time);
void WATER_PUMP_increase_counter(Water_pump_struct * pump);
uint8_t WATER_PUMP_start_watering(Water_pump_struct* pump);
uint8_t WATER_PUMP_stop_watering(Water_pump_struct* pump);
uint8_t WATER_PUMP_is_watering_complete(Water_pump_struct* pump);
#endif /* INC_WATER_PUMP_H_ */

