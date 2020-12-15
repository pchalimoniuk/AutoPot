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
	uint16_t soil_moisture_percent;
	uint32_t soil_moisture_raw;
	uint8_t air_moisture;
	uint8_t aiir_temperature;
}sensor_readings_struct;


#endif /* INC_SENSORS_READINGS_H_ */
