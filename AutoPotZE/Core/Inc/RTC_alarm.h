/*
 * RTC_alarm.h
 *
 *  Created on: 10 Dec 2020
 *      Author: Dell
 */

#ifndef INC_RTC_ALARM_H_
#define INC_RTC_ALARM_H_
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
typedef struct {
	RTC_HandleTypeDef *hrtc;
	uint8_t wateringPeriod;
	uint8_t alarmTimeHour;
	uint32_t alarmTypeX;
	uint8_t weekday;
	uint8_t hours;
	uint8_t minutes;
}ALARM_RTC_struct;
uint8_t ALARM_RTC_init_rtc(ALARM_RTC_struct * alarm_struct);
uint8_t ALARM_RTC_set_rtc(ALARM_RTC_struct * alarm_struct);
uint8_t ALARM_RTC_set_alarm(ALARM_RTC_struct * alarm_struct);
#endif /* INC_RTC_ALARM_H_ */
