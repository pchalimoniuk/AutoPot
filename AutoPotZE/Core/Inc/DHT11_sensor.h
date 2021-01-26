/**
 * @file DHT11_sensor.h
 * @brief Function used to retrieve data from DHT11 sensor and struct associated with it.
 * @author Paweł Chalimoniuk
 */

#ifndef INC_DHT11_SENSOR_H_
#define INC_DHT11_SENSOR_H_
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#define DHT11_OK 1 /**<Makro indicating successful reading from sensor.**/
#define DHT11_ERROR 0 /**<Makro indicating error in  reading from sensor.**/
/**
 * @brief Structure holding readings from DHT11 sensor
 */
typedef struct
{
	uint8_t Temperature;/**<Hummidity in air**/
	uint8_t Humidity;/**<Temperature of air**/
}DHT_Data;
/**
 * @brief Structure used to config DHT11 sensor
 */
typedef struct{
	TIM_HandleTypeDef * htim; /**< Handler to timer controlling sensor.*/
	GPIO_TypeDef* GPIOx;	 /**< GPIO connected to sensor.**/
	uint16_t GPIO_Pin;	/**< GPIO Pin connected to sensor.**/
	uint16_t tim_freq; /**<Timer frequency.**/
	TIM_TypeDef *Instance; /**<Timer instance.**/
}DHT_Config;
uint8_t DHT11_get_data_from_sensor(DHT_Data *DHT_Data, DHT_Config *config);
#endif /* INC_DHT11_SENSOR_H_ */
