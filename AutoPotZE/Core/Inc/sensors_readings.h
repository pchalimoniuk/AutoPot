/*
 * sensors_readings.h
 *
 *  Created on: 11 Dec 2020
 *      Author: Dell
 */

#ifndef INC_SENSORS_READINGS_H_
#define INC_SENSORS_READINGS_H_
#include "stdlib.h"
typedef struct{
	uint16_t soilMoisturePercent;
	uint32_t soilMoistureRaw;
	uint8_t airMoisture;
	uint8_t airTemperature;
}sensorReadings;


#endif /* INC_SENSORS_READINGS_H_ */
