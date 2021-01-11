/*
 * water_pump.c
 *
 *  Created on: 29 Nov 2020
 *      Author: Dell
 */
#include "water_pump.h"
/**
  * @brief sets minimal moisture
  * @param pointer to Water_pump_struct struct
  * @param minimal soil moisture
  * @retval NONE
  */
void WATER_PUMP_set_water_level(Water_pump_struct* pump, uint8_t water_level){
	pump->water_level = water_level;
}
/**
  * @brief Sets timer timer
  * @param pointer to Water_pump_struct struct
  * @param Handler to timmer
  * @retval NONE
  */
void WATER_PUMP_set_tim(Water_pump_struct* pump,TIM_HandleTypeDef* tim){
	pump->tim = tim;
}
/**
  * @brief Sets GPIO PIN used to drive pump
  * @param pointer to Water_pump_struct struct
  * @param GPIO_TypeDef pointer to selected pin
  * @param GPIO_Pin pin number to selected pin
  * @retval return 1 if pump stopped successfully
  */
void WATER_PUMP_set_GPIO_port(Water_pump_struct* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	pump->GPIOx = GPIOx;
	pump->GPIO_Pin = GPIO_Pin;
}
/**
  * @brief Initalize pump with default parameters
  * @param pointer to Water_pump_struct struct
  * @retval NONE
  */
void WATER_PUMP_init(Water_pump_struct* pump){
	 HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = pump->GPIO_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(pump->GPIOx, &GPIO_InitStruct);
	 HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_SET);


}/**
 * @brief Stops pump
 * @param pointer to Water_pump_struct struct
 * @param time in seconds for how long pump should be on (minimum 10s)
 * @retval None
 */
void WATER_PUMP_set_watering_time(Water_pump_struct* pump, uint8_t watering_time){ //convert time in seconds to 10s
	uint8_t temp = watering_time/10;
	pump->watering_time = temp;
}
/**
  * @brief Starts pump and pumps timer
  * @param pointer to Water_pump_struct struct
  * @retval return 1 if pump started successfully
  */
uint8_t WATER_PUMP_start_watering(Water_pump_struct* pump){
	if(HAL_TIM_Base_Start_IT(pump->tim) != HAL_OK){
		return 0;
	}
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin((pump->GPIOx), (pump->GPIO_Pin)) != GPIO_PIN_RESET){
		return 0;
	}
	return 1;

}
/**
  * @brief Stops pump
  * @param pointer to Water_pump_struct struct
  * @retval return 1 if pump stopped successfully
  */
uint8_t WATER_PUMP_stop_watering(Water_pump_struct* pump){
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin((pump->GPIOx), (pump->GPIO_Pin)) != GPIO_PIN_SET){
		return 0;
	}
	pump->counter = 0;
	if(HAL_TIM_Base_Stop_IT(pump->tim) != HAL_OK){
		return 0;
	}
	return 1;
}
/**
  * @brief Increase pump inner counter by one
  * @param pointer to Water_pump_struct struct
  * @retval NONE
  */
void WATER_PUMP_increase_counter(Water_pump_struct * pump){
	pump->counter++;
}
/**
  * @brief Check whether watering is complete
  * @param pointer to Water_pump_struct struct
  * @retval 1 if watering is complete 0 otherwise
  */
uint8_t WATER_PUMP_is_watering_complete(Water_pump_struct* pump){
	if(pump->counter > pump->watering_time){
		return 1;
	}else{
		return 0;
	}
}

