/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bluetooth_interface.h"
#include "soil_moisture_sensor.h"
#include "water_pump.h"
#include "RTC_alarm.h"
#include "flags.h"
#include "sensors_readings.h"
#include "DHT11_sensor.h"
//#include "temperature_sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WATERING_TIME  10 /**<Default watering time **/
#define DAYS_BETWEEN_WATERING 1 /**<Default  period between measurements**/
#define DEFUALT_WATER_LEVEL 30 /**<Default watering level that triggers watering **/
#define TIMER_FREQ_MHZ 21 /**<TIMER2 freq in MHZ**/
#define TIMER_FREQ_KHZ 21000 /**<TIMER3 freq in kHz**/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;	/**<Handler to ADC used by soil moisture sensor.**/
DMA_HandleTypeDef hdma_adc1; /**<Handler to ADC DMA used by soil moisture sensor.**/
RTC_HandleTypeDef hrtc; /**<Handler to RTC clock.**/
TIM_HandleTypeDef htim2; /**<Handler to TIM2 used by pump.**/
TIM_HandleTypeDef htim3; /**<Handler to TIM3 used by DHT11 sensor.**/
UART_HandleTypeDef huart2; /**<Handler to UART used by HC-05 Bluetooth module.**/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void handle_measurement(void);
void handle_bluetooth(void);
void handle_wattering(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	uint8_t command_buffer[18]; /**<Buffer for incoming bluetooth comand.**/
	uint8_t out_buffer[20]; /**<Out buffer for sending data over bluetooth.**/
	Flags_Struct flags = {0}; /**<Struct holding flags.**/
	Sensor_Readings_Struct sensor_readings = {0}; /**<Struct holding all sensor readings.**/
	Water_Pump_Struct pump; /**<Struct controling pump.**/
	ALARM_RTC_struct alarm_struct; /**<Struct holding alarm setting .**/
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	RTC_AlarmTypeDef sAlarm = { 0 };
	DHT_Config config; /**<Struct holding DHT11 settings.**/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* Initialize DHT11 sensor*/
  __HAL_RCC_TIM3_CLK_ENABLE();
  config.htim=&htim3;
  config.GPIO_Pin=GPIO_PIN_9;
  config.GPIOx = GPIOC;
  config.tim_freq = TIMER_FREQ_MHZ;
  config.Instance= TIM3;

  /*Initialize rtc and alarm */
  alarm_struct.hrtc= &hrtc;
  alarm_struct.alarmTypeX = RTC_ALARM_A;
  alarm_struct.alarmTimeHour=10;
  ALARM_RTC_init_rtc(&alarm_struct);
  HAL_RTC_GetTime(alarm_struct.hrtc, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(alarm_struct.hrtc, &sDate, RTC_FORMAT_BIN);
  ALARM_RTC_set_alarm(&alarm_struct);
  alarm_struct.wateringPeriod=DAYS_BETWEEN_WATERING;

  /** Initalize water pump */
  __HAL_RCC_TIM2_CLK_ENABLE();
  sensor_readings.soilMoisturePercent = 100;
  WATER_PUMP_set_tim(&pump, &htim2);
  WATER_PUMP_set_GPIO_port(&pump,GPIOF, GPIO_PIN_7);
  WATER_PUMP_set_water_level(&pump,DEFUALT_WATER_LEVEL);
  WATER_PUMP_set_tim_freq(&pump, TIMER_FREQ_KHZ);
  WATER_PUMP_set_timer_instance(&pump, TIM2);
  WATER_PUMP_set_watering_time(&pump, WATERING_TIME);
  WATER_PUMP_init(&pump);
  WATER_PUMP_start_watering(&pump);
  WATER_PUMP_stop_watering(&pump);
  /*Prepare to recive command over bluetooth*/
  HAL_UART_Receive_IT(&huart2, command_buffer, sizeof(command_buffer));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    	handle_bluetooth();
    	handle_measurement();
    	handle_wattering();
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OverCurrent_Pin PG10 */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**@brief Wrapper function to handle measurement logic
 *
 */
void handle_measurement(void) {
	/* Start measurement if time has passed or user requested */
	if (1 == flags.alarm || flags.refreshRequest) {
		if (!start_soil_moisture_measurement(&hadc1, &hdma_adc1,
				&sensor_readings.soilMoistureRaw)) {
			Error_Handler();
		}
		flags.alarm = 0;
	}
	if(flags.soilMeasurementComplete){
  		sensor_readings.soilMoisturePercent = get_normalized_moisture_level(sensor_readings.soilMoistureRaw);
  		flags.soilMeasurementComplete = 0;
  		if(DHT11_get_data_from_sensor(&sensor_readings.dht11SensorData,&config) != DHT11_OK){
  			sensor_readings.dht11SensorData.Humidity = 0;  /* If measurement was faulty alarm user*/
  			sensor_readings.dht11SensorData.Temperature = 0;
  		}
 		 if(flags.refreshRequest){ /*Send data if user made a request*/
 			 BLUETOOTH_INTERFACE_send_data((char *)out_buffer, sensor_readings);
 			 HAL_UART_Transmit(&huart2, out_buffer,sizeof(out_buffer),HAL_MAX_DELAY);
 			__HAL_UART_FLUSH_DRREGISTER(&huart2);
 			 flags.refreshRequest = 0;
 			 return;
 		 }
 	/*Start waterinng if water level is below set level */
 	 if(sensor_readings.soilMoisturePercent < pump.water_level){
 			WATER_PUMP_start_watering(&pump);
 	 	}
 	 /*Start next alarm*/
  		ALARM_RTC_set_alarm(&alarm_struct);

	}
}

/**@brief Wrapper function to handle bluetooth logic
 *
 */
void handle_bluetooth(void) {
	if (flags.bluetoothEvent == 1) {
		if (BLUETOOTH_INTERFACE_interpret_command((char*) command_buffer, &pump, &flags,
				&alarm_struct) == BLUETOOTH_COMMAND_OK) {
			__NOP();
		}
		 HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
		__HAL_UART_FLUSH_DRREGISTER(&huart2);
		 HAL_UART_Receive_IT(&huart2, command_buffer, sizeof(command_buffer));
		flags.bluetoothEvent = 0;
	}

}
/**@brief Wrapper function to handle wattering logic
 *
 */
void handle_wattering(void) {
	/*Stop watering if complete */
	if (flags.wateringComplete) {
		if(WATER_PUMP_stop_watering(&pump)){
			flags.wateringComplete = 0;
		}
	}

}
/**
 * @brief ADC Handler callback for soil moisture sensor
 * @param hadc ADC Handler
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(!stop_soil_measurement(&hadc1)){
		Error_Handler();
	}
		flags.soilMeasurementComplete = 1;


}
/**
 * @brief RTC Alarm callback setting measurenet flag
 * @param hrtc RTC Handler
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
		flags.alarm = 1;
}
/**
 * @brief TIM callback controlling Water Pump flag
 * @param htim TIM Handler
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if (htim->Instance==TIM2)
	 {
		 flags.wateringComplete = 1;
	 }
}
/**
 * @brief HAL_UART callback triggering bluetooth event
 * @param huart UART Handler
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		flags.bluetoothEvent = 1;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
