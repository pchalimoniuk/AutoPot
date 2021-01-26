/**
 * @file water_pump.c
 * @brief Functions related to water_pump
 * @author Paweł Chalimoniuk
 */
#include "water_pump.h"
/**
  * @brief Sets minimal moisture
  * @param pump Pointer to Water_pump_struct struct
  * @param water_level Minimal soil moisture
  * @retval None
  */
void WATER_PUMP_set_water_level(Water_Pump_Struct* pump, uint8_t water_level){
	pump->water_level = water_level;
}
/**
  * @brief Sets timer timer
  * @param pump Pointer to Water_pump_struct struct
  * @param tim Handler to timer
  * @retval None
  */
void WATER_PUMP_set_tim(Water_Pump_Struct* pump,TIM_HandleTypeDef* tim){
	pump->tim = tim;
}
/**
  * @brief Sets GPIO PIN used to drive pump
  * @param pump Pointer to Water_pump_struct struct
  * @param GPIOx GPIO_TypeDef Pointer to selected pin
  * @param GPIO_Pin Pin number to selected pin
  * @retval Return 1 if pump stopped successfully, 0 otherwise
  */
void WATER_PUMP_set_GPIO_port(Water_Pump_Struct* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	pump->GPIOx = GPIOx;
	pump->GPIO_Pin = GPIO_Pin;
}
/**
  * @brief Initalize pump's timer with parameters in @param
  * @param pump Pointer to Water_pump_struct struct
  * @retval None
  */
void WATER_PUMP_timer_setup(Water_Pump_Struct* pump){
	  pump->tim->Instance = pump->Instance;
	  pump->tim->Init.Prescaler = pump->timer_freq_kHZ -1;
	  pump->tim->Init.CounterMode = TIM_COUNTERMODE_UP;
	  pump->tim->Init.Period = pump->watering_time*1000-1;
	  pump->tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  HAL_TIM_Base_Init(pump->tim);

}
/**
  * @brief Initalize pump with parameters in @param
  * @param pump Pointer to Water_pump_struct struct
  * @retval None
  */
void WATER_PUMP_init(Water_Pump_Struct* pump){
	 HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = pump->GPIO_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(pump->GPIOx, &GPIO_InitStruct);
	 HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_SET);
	 WATER_PUMP_timer_setup(pump);
}
/**
 * @brief Stops pump
 * @param pump Pointer to Water_pump_struct struct
 * @param watering_time Time in seconds for how long pump should be on (minimum 10s)
 * @retval None
 */
void WATER_PUMP_set_watering_time(Water_Pump_Struct* pump, uint8_t watering_time){ //convert time in seconds to 10s
	pump->watering_time = watering_time;
	WATER_PUMP_timer_setup(pump);
}
/**
  * @brief Sets timer frequency
  * @param pump Pointer to Water_pump_struct struct
  * @param timer_freq_kHz frequency that timer is working on
  * @retval None
  */
void WATER_PUMP_set_tim_freq(Water_Pump_Struct* pump,uint16_t timer_freq_kHz){
	pump->timer_freq_kHZ=timer_freq_kHz;
}
/**
  * @brief Sets timer instance
  * @param pump Pointer to Water_pump_struct struct
  * @param Instance instance of timer
  * @retval None
  */
void WATER_PUMP_set_timer_instance(Water_Pump_Struct* pump, TIM_TypeDef *Instance){
	pump->Instance = Instance;
}

/**
  * @brief Starts pump and pumps timer
  * @param pump Pointer to Water_pump_struct struct
  * @retval Return 1 if pump started successfully, 0 otherwise
  */
uint8_t WATER_PUMP_start_watering(Water_Pump_Struct* pump){
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
  * @param pump Pointer to Water_pump_struct struct
  * @retval Return 1 if pump stopped successfully,  0 otherwise
  */
uint8_t WATER_PUMP_stop_watering(Water_Pump_Struct* pump){
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin((pump->GPIOx), (pump->GPIO_Pin)) != GPIO_PIN_SET){
		return 0;
	}
	if(HAL_TIM_Base_Stop_IT(pump->tim) != HAL_OK){
		return 0;
	}
	return 1;
}
/**
  * @brief Increase pump inner counter by one
  * @param pump Pointer to Water_pump_struct struct
  * @retval None
  */
//void WATER_PUMP_increase_counter(Water_Pump_Struct * pump){
//	pump->counter++;
//}
///**
//  * @brief Check whether watering is complete
//  * @param pump pointer to Water_pump_struct struct
//  * @retval Return 1 if watering is complete, 0 otherwise
//  */
//uint8_t WATER_PUMP_is_watering_complete(Water_Pump_Struct* pump){
//	if(pump->counter > pump->watering_time){
//		return 1;
//	}else{
//		return 0;
//	}
//}

