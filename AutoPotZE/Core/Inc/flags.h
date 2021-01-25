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
	uint8_t soilMeasurementComplete;
	uint8_t bluetoothEvent;
	uint8_t refreshRequest;
	uint8_t wateringInProgress;
}Flags;


#endif /* INC_FLAGS_H_ */
