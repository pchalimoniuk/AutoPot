/**
 * @file DHT11_sensor.c
 * @brief Function used to retrieve data from DHT11 sensor.
 * @author Paweł Chalimoniuk
 */

#include "stm32f4xx_hal.h"
#include "DHT11_sensor.h"
/**
 * @brief Delay in us
 * @param config Handler to DHT_config with dht connection parameters
 * @param microseconds How long timer should last
 */
void delay(DHT_Config *config, uint32_t microseconds)
{
	config->htim->Instance->CNT=0;
	while(config->htim->Instance->CNT < microseconds);
}
/**
 * @brief Function changes pin mode to output
 * @param GPIOx Pointer to GPIO pin connected to sensor
 * @param GPIO_Pin Number of GPIO pin connected to sensor
 */
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
/**
 * @brief Function changes pin mode to input
 * @param GPIOx Pointer to GPIO pin connected to sensor
 * @param GPIO_Pin Number of GPIO pin connected to sensor
 */
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
/**
 * @brief Function timer used for measurement
 * @param config Handler to DHT_config with dht connection parameters
 * @return DHT11_OK if successful, DHT11_ERROR otherwise
 */
uint8_t DHT_Timer_Setup(DHT_Config *config){
	  config->htim->Instance = TIM3;
	  config->htim->Init.Prescaler = config->tim_freq -1;
	  config->htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	  config->htim->Init.Period = 0xFFFF-1;
	  config->htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if( HAL_TIM_Base_Init(config->htim)!= HAL_OK){
	  return DHT11_ERROR;
	 	  }
	  if(HAL_TIM_Base_Start(config->htim) != HAL_OK){
		  return DHT11_ERROR;
	  }
	  return DHT11_OK;
}
/**
 * @brief Function initalize measurement
 * @param config Handler to DHT_config with dht connection parameters
 * @return DHT11_OK if successful, DHT11_ERROR otherwise
 */
uint8_t DHT_Start (DHT_Config *config)
{
	if(DHT_Timer_Setup(config) != DHT11_OK){
		return DHT11_ERROR;
	}
	Set_Pin_Output (config->GPIOx, config->GPIO_Pin);  // set the pin as output
	HAL_GPIO_WritePin (config->GPIOx, config->GPIO_Pin, 0);   // pull the pin low

	delay (config, 18000);   // wait for 18ms
    HAL_GPIO_WritePin (config->GPIOx, config->GPIO_Pin, 1);   // pull the pin high
    delay (config, 30);   // wait for 30us
	Set_Pin_Input(config->GPIOx, config->GPIO_Pin);    // set as input
	return DHT11_OK;
}
/**
 * @brief Function check if sensor is responding correctly
 * @param config Handler to DHT_config with dht connection parameters
 * @return DHT11_OK if successful, DHT11_ERROR otherwise
 */
uint8_t DHT_Check_Response (DHT_Config *config)
{
	uint8_t Response = DHT11_ERROR;
	delay (config, 40);
	if (!(HAL_GPIO_ReadPin (config->GPIOx, config->GPIO_Pin)))
	{
		delay (config, 80);
		if ((HAL_GPIO_ReadPin (config->GPIOx, config->GPIO_Pin))) Response = DHT11_OK;
		else Response = DHT11_ERROR;
	}
	config->htim->Instance->CNT=0;
	while ((HAL_GPIO_ReadPin (config->GPIOx, config->GPIO_Pin))){   // wait for the pin to go low
		if(config->htim->Instance->CNT > 60000){ //check timer to avoid spinning in case of faulty read
			Response = DHT11_ERROR;
			break;
		}
	}

	return Response;
}
/**
 * @brief Function reads data from sensor after correct response
 * @param config Handler to DHT_config with dht connection parameters
 * @return Data read from sensor
 */
uint8_t DHT_Read (DHT_Config *config)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		config->htim->Instance->CNT=0;
		while (!(HAL_GPIO_ReadPin (config->GPIOx, config->GPIO_Pin))){   // wait for the pin to go high
			if(config->htim->Instance->CNT > 60000){ //check timer to avoid spinning in case of faulty read
				break;
			}
		}
		delay (config, 40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (config->GPIOx, config->GPIO_Pin)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		config->htim->Instance->CNT=0;
		while ((HAL_GPIO_ReadPin (config->GPIOx, config->GPIO_Pin))){   // wait for the pin to go low
			if(config->htim->Instance->CNT > 60000){ //check timer to avoid spinning in case of faulty read
				break;
			}
		}
	}
	return i;
}
/**
 * @brief Function used to retrieve data from DHT11
 * @param data Pointer to DHT11_Data structure
 * @param config Handler to DHT_config with dht connection parameters
 * @return DHT11_OK if successful, DHT11_ERROR otherwise
 */
uint8_t DHT11_get_data_from_sensor (DHT_Data *DHT_Data, DHT_Config *config)
{
	uint8_t Rh_byte1=0;
	uint8_t Rh_byte2=0;
	uint8_t Temp_byte1=0;
	uint8_t Temp_byte2=0;
	uint8_t SUM =0;
    if(DHT_Start(config) != DHT11_OK){
    	return DHT11_ERROR;
    }

	if(DHT_Check_Response(config) == DHT11_OK){
	 Rh_byte1 = DHT_Read(config);
	 Rh_byte2 = DHT_Read(config);
	 Temp_byte1 = DHT_Read(config);
	 Temp_byte2 = DHT_Read(config);
	 SUM = DHT_Read(config);
	}else{
		return DHT11_ERROR;
	}
	__NOP();
	if (SUM == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))
	{
			DHT_Data->Temperature = Temp_byte1;
			DHT_Data->Humidity = Rh_byte1;
			return DHT11_OK;
	}
	return DHT11_ERROR;
}

