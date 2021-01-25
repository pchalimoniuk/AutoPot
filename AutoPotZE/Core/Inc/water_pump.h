/**
 * @file water_pump.h
 * @brief Functions related to water_pump
 * @author Paweł Chalimoniuk
 */
#ifndef INC_WATER_PUMP_H_
#define INC_WATER_PUMP_H_
#include "stm32f4xx_hal.h"
#include "stdlib.h"
/**
 * @brief struct holding water pump related information.
 */
typedef struct {
	uint8_t water_level; /**< Minimal water level that won't trigger pump.*/
	TIM_HandleTypeDef *tim; /**< Handler to timer controlling time of turning on and off.*/
	GPIO_TypeDef* GPIOx;	 /**< GPIO controlling relay that turn on the pump.**/
	uint16_t GPIO_Pin;	/**< GPIO Pin controlling relay that trun on the pump.**/
	uint8_t watering_time;	/**< Amount of time in seconds that pump should be working after turning on. MAX 65 sec on 16bit timer**/
	uint16_t timer_freq_kHZ; /**< Frequency of used timer in kHz.**/
	TIM_TypeDef *Instance; /**<Timer instance.**/
}Water_Pump_Struct;
void WATER_PUMP_set_water_level(Water_Pump_Struct* pump, uint8_t water_level);
void WATER_PUMP_set_tim(Water_Pump_Struct* pump,TIM_HandleTypeDef* tim);
void WATER_PUMP_set_tim_freq(Water_Pump_Struct* pump,uint16_t timer_freq_kHz);
void WATER_PUMP_set_GPIO_port(Water_Pump_Struct* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WATER_PUMP_set_timer_instance(Water_Pump_Struct* pump, TIM_TypeDef *Instance);
void WATER_PUMP_init(Water_Pump_Struct* pump);
void WATER_PUMP_set_watering_time(Water_Pump_Struct * pump, uint8_t watering_time);
uint8_t WATER_PUMP_start_watering(Water_Pump_Struct* pump);
uint8_t WATER_PUMP_stop_watering(Water_Pump_Struct* pump);
#endif /* INC_WATER_PUMP_H_ */

