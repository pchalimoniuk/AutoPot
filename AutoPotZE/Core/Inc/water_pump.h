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
}water_pump;
void set_water_level(water_pump* pump, uint8_t water_level);
void set_tim(water_pump* pump,TIM_HandleTypeDef* tim);
void set_GPIO_port(water_pump* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void water_pump_init(water_pump* pump);
void set_watering_time(water_pump * pump, uint8_t watering_time);
void increase_counter(water_pump * pump);
uint8_t start_watering(water_pump* pump);
uint8_t stop_watering(water_pump* pump);
uint8_t is_watering_complete(water_pump* pump);
#endif /* INC_WATER_PUMP_H_ */

