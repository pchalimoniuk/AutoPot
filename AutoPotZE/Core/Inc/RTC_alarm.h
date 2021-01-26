/**
 * @file RTC_alarm.h
 * @brief Functions and structure related to RTC and RTC alarm
 * @author Paweł Chalimoniuk
 */

#ifndef INC_RTC_ALARM_H_
#define INC_RTC_ALARM_H_
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
/**
 * @brief Structure used to control RTC alarm related variables
 *
 **/
typedef struct {
	RTC_HandleTypeDef *hrtc;	/**<Handler to RTC used to trigger measurement.**/
	uint8_t wateringPeriod;	/**<Variable controlling how often (in days) measurement should be performed. Only values accepted are 1-7.**/
	uint8_t alarmTimeHour;	/**<Variable controlling hour when measurement should be performed. Only accepted values are 0-23.**/
	uint32_t alarmTypeX;	/**<Type of alarm used.**/
	uint8_t weekday;	/**<Variable used to during setting up time, representing current weekday .**/
	uint8_t hours;	/**<Variable used to during setting up time, representing current hour.**/
	uint8_t minutes;	/**<Variable used to during setting up time, representing current minute .**/
}ALARM_RTC_struct;
uint8_t ALARM_RTC_init_rtc(ALARM_RTC_struct * alarm_struct);
uint8_t ALARM_RTC_set_rtc(ALARM_RTC_struct * alarm_struct);
uint8_t ALARM_RTC_set_alarm(ALARM_RTC_struct * alarm_struct);
#endif /* INC_RTC_ALARM_H_ */
