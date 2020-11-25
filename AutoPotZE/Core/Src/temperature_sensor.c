/*
 * temperature_sensor.h
 *
 *  Created on: Nov 13, 2020
 *      Author: Dell
 */

#include "temperature_sensor.h"
#include "stm32f4xx_hal.h"

#define TYPE_DHT11    // define according to your sensor
//#define TYPE_DHT22


#define DHT_PORT GPIOA
#define DHT_PIN GPIO_PIN_1
//kradzione
uint32_t timer_flag = 0;
uint32_t counter = 0;
TIM_HandleTypeDef htim3;
uint8_t read_data(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
void change_pin_mode(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin , uint32_t Mode);
void prepare_pin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
uint8_t response_from_sensor(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
uint8_t calculate_checksum(uint8_t humidity, uint8_t temperature, uint8_t humidity_decimal, uint8_t temperature_decimal);
uint8_t get_data_from_sensor(DHT_Data * data, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
//
//uint32_t DWT_Delay_Init(void)
//{
//  /* Disable TRC */
//  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
//  /* Enable TRC */
//  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
//
//  /* Disable clock cycle counter */
//  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
//  /* Enable  clock cycle counter */
//  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
//
//  /* Reset the clock cycle counter value */
//  DWT->CYCCNT = 0;
//
//     /* 3 NO OPERATION instructions */
//     __ASM volatile ("NOP");
//     __ASM volatile ("NOP");
//  __ASM volatile ("NOP");
//
//  /* Check if clock cycle counter has started */
//     if(DWT->CYCCNT)
//     {
//       return 0; /*clock cycle counter started*/
//     }
//     else
//  {
//    return 1; /*clock cycle counter not started*/
//  }
//}
////
//__STATIC_INLINE void delay(volatile uint32_t microseconds)
//{
//  uint32_t clk_cycle_start = DWT->CYCCNT;
//
//  /* Go to number of cycles for system */
//  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
//
//  /* Delay till end */
//  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
//}
//void timer(uint32_t microseconds){
//	timer_flag = 0;
//	htim3.Init.Period=microseconds-1;
//	HAL_TIM_Base_Init(&htim3);
//	if(HAL_OK != HAL_TIM_Base_Start_IT(&htim3)){
//		Error_Handler();
//	}
//}


void delay(uint32_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim3))<time);
}


void prepare_pin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin){
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	 /*Configure GPIO pin to work with sensor */
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void change_pin_mode(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin,uint32_t Mode){
	/*Change pin mode GPIO pin to work with sensor */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = Mode;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
void start_measurement(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin){
		//DWT_Delay_Init();

		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
//		timer(18000);
//		while(!timer_flag){
//			counter =  __HAL_TIM_GET_COUNTER(&htim3);
//		}
		delay(18000);
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
//		timer(40);
//		while(!timer_flag);
		delay(20);
		change_pin_mode(GPIOx, GPIO_Pin, GPIO_MODE_INPUT);
}
uint8_t reponse_from_sensor(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin){

	/*Checking for response*/
	uint8_t response = 0;
//	timer(40);
//	while(!timer_flag);
	delay(40);
	if(!(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))){
//		timer(80);
//		while(!timer_flag);
		//delay(80);
		while(!(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)));
		if ((HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)))
			response = 1;
		else
			response = -1; // 255
	}
	while ((HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)));
	return response;
}
uint8_t read_data(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin){
	uint8_t i=0,j = 0;
	for(j=0; j < 8; j++){
		while(!(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)));
//		timer(40);
//		while(!timer_flag);
		delay(40);
		if(!(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))){
			i&=~(1<<(7-j));
		}else{
			i|=(1<<(7-j));
		}
		while((HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)));
	}

	return i;
}

uint8_t calculate_checksum(uint8_t humidity, uint8_t temperature, uint8_t humidity_decimal, uint8_t temperature_decimal){
	uint8_t temp = (temperature & 1) + (humidity & 1) + (humidity_decimal & 1) + (temperature_decimal & 1);
	return temp;

}
uint8_t get_data_from_sensor(DHT_Data * data, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin){


	  /* USER CODE BEGIN TIM3_Init 0 */

	  /* USER CODE END TIM3_Init 0 */

	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  /* USER CODE BEGIN TIM3_Init 1 */

	  /* USER CODE END TIM3_Init 1 */
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = 84-1;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim3.Init.Period = 0xFFFF-1;
	  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	  {
	  //  Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	  {
	 //  Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
	   // Error_Handler();
	  }
	  /* USER CODE BEGIN TIM3_Init 2 */
	  //HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	 //HAL_NVIC_EnableIRQ(TIM3_IRQn);
	  /* USER CODE END TIM3_Init 2 */
	  HAL_TIM_Base_Start(&htim3);

	uint8_t h_dec = 0;
	uint8_t t_dec = 0;
	uint8_t checksum = 0;
	prepare_pin(GPIOx, GPIO_Pin);
	start_measurement(GPIOx, GPIO_Pin);
	h_dec = reponse_from_sensor(GPIOx, GPIO_Pin);
	if(h_dec == 1){
		data->humidity = read_data(GPIOx, GPIO_Pin);
		h_dec = read_data(GPIOx, GPIO_Pin);
		data->temperature = read_data(GPIOx, GPIO_Pin);
		t_dec = read_data(GPIOx, GPIO_Pin);
		checksum = read_data(GPIOx, GPIO_Pin);
		prepare_pin(GPIOx, GPIO_Pin);
	    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
		if(calculate_checksum(data->humidity, data->temperature, h_dec, t_dec) == checksum){
			return 1;
		}
	}
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)
  {
	  timer_flag = 1;
	  //HAL_TIM_Base_Stop_IT(&htim3);
  }

}
