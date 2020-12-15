/*
 * water_pump.c
 *
 *  Created on: 29 Nov 2020
 *      Author: Dell
 */
#include "water_pump.h"
uint8_t pump_efficency = 1200; // mililiters per minute
void set_water_level(water_pump* pump, uint8_t water_level){
	pump->water_level = water_level;
}
void set_tim(water_pump* pump,TIM_HandleTypeDef* tim){
	pump->tim = tim;
}
void set_GPIO_port(water_pump* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	pump->GPIOx = GPIOx;
	pump->GPIO_Pin = GPIO_Pin;
}
void water_pump_init(water_pump* pump){
	 HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = pump->GPIO_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(pump->GPIOx, &GPIO_InitStruct);

}
void set_watering_time(water_pump* pump, uint8_t watering_time){ //convert time in seconds to 10s
	uint8_t temp = watering_time/10;
	pump->watering_time = temp;
}
uint16_t get_time(water_pump pump){  //to be delted
	return pump_efficency/6;
}
uint16_t set_end_timer(water_pump * pump){ //only for now
}
uint8_t start_watering(water_pump* pump){
	if(HAL_TIM_Base_Start_IT(pump->tim) != HAL_OK){
		return 0;
	}
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin((pump->GPIOx), (pump->GPIO_Pin)) != GPIO_PIN_SET){
		return 0;
	}
	return 1;

}
uint8_t stop_watering(water_pump* pump){
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin((pump->GPIOx), (pump->GPIO_Pin)) != GPIO_PIN_RESET){
		return 0;
	}
	pump->counter = 0;
	if(HAL_TIM_Base_Stop_IT(pump->tim) != HAL_OK){
		return 0;
	}
	return 1;
}
void increase_counter(water_pump * pump){
	pump->counter++;
}

uint8_t is_watering_complete(water_pump* pump){
	if(pump->counter>= pump->watering_time){
		return 1;
	}else{
		return 0;
	}
}

