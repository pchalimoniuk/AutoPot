/*
 * temperature_sensor.h
 *
 *  Created on: Nov 13, 2020
 *      Author: Dell
 */

#ifndef INC_TEMPERATURE_SENSOR_H_
#define INC_TEMPERATURE_SENSOR_H_
#include "stm32f4xx_hal.h"
#include "stdlib.h"
typedef struct{
	float temperature;
	float humidity;
}DHT_Data;
uint8_t get_data_from_sensor(DHT_Data * data, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
void start_measurement(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
#endif /* INC_TEMPERATURE_SENSOR_H_ */


/*
 * get_data_from_sensor(&DHT11_Data, GPIOA, GPIO_PIN_1);
	  Temperature = DHT11_Data.temperature;
	  Humidity = DHT11_Data.humidity;
	  HAL_Delay(300);
	  sprintf(msg, "%f\r\n", Temperature);
	  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

* */
