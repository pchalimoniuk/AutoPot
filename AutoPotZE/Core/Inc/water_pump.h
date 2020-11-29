/*
 * water_pump.h
 *
 *  Created on: 29 Nov 2020
 *      Author: Dell
 */

#ifndef INC_WATER_PUMP_H_
#define INC_WATER_PUMP_H_
typedef struct {
	uint8_t water_level;
	RTC_HandleTypeDef *hrtc;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}water_pump;
void set_water_level(water_pump* pump, uint8_t water_level);
void set_rtc(water_pump* pump,RTC_HandleTypeDef* hrtc);
void set_GPIO_port(water_pump* pump, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void water_pump_init(water_pump* pump);
void start_watering(water_pump* pump);
void stop_watering(water_pump* pump);
#endif /* INC_WATER_PUMP_H_ */
