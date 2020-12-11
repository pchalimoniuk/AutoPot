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
	uint8_t freq;
	uint8_t hours_alarm;
	uint32_t ALARM_TYPEx;
	uint8_t weekday;
	uint8_t hours;
	uint8_t minutes;
}alarm;
uint8_t init_rtc(alarm * alarm_struct);
uint8_t set_rtc(alarm * alarm_struct);
uint8_t set_alarm(alarm * alarm_struct);
#endif /* INC_RTC_ALARM_H_ */
