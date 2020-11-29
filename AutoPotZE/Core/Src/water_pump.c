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
void set_rtc(water_pump* pump,RTC_HandleTypeDef* hrtc){
	pump->hrtc = hrtc;
}
void set_GPIO_port(water_pump* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	pump->GPIOx = GPIOx;
	pump->GPIO_Pin = GPIO_Pin;
}
void water_pump_init(water_pump* pump){
	 HAL_GPIO_WritePin(&(pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = pump->GPIO_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(pump->GPIOx, &GPIO_InitStruct);
	 //init RTC;

}
uint16_t get_time(water_pump pump){
	return (pump->water_level*60)/pump_efficency;
}
uint16_t set_end_timer(water_pump * pump){ //only for now
	//HEAVY WIP
	uint16_t time = get_time(*pump);
	if(time == 0){
		 return 0;
	}
	RTC_AlarmTypeDef sAlarm = {0};
	if(time < 60){ //seconds
		sAlarm.AlarmTime.Seconds = time;
		sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES;
	}
	else if(time>60 && time < 3600){ //minutes
		sAlarm.AlarmTime.Minutes = time;
		sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_SECONDS;
	}
	else{
		return 0; //more than 1 hour is no-no
	}

	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_B;
	if (HAL_RTC_SetAlarm_IT(pump->hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
	    return 0;
	}
	return 1;
}
void start_watering(water_pump* pump){
	//HEAVY WIP
	if(!set_timer(pump)){
		return;
	}
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_SET);
}
void stop_watering(water_pump* pump){
	HAL_GPIO_WritePin((pump->GPIOx), (pump->GPIO_Pin), GPIO_PIN_RESET);
}

/*
 * if(time>1 && time < 60){
 *
 * }
 */
