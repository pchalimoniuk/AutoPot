/*
 * flags.h
 *
 *  Created on: 11 Dec 2020
 *      Author: Dell
 */

#ifndef INC_FLAGS_H_
#define INC_FLAGS_H_
#include "stdlib.h"
typedef struct{
	uint8_t alarm;
	uint8_t soil_sensor;
	uint8_t bluetooth;
	uint8_t refresh;
	uint8_t watering;
}flags_struct;


#endif /* INC_FLAGS_H_ */
