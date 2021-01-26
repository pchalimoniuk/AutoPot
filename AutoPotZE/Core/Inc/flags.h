/**
 * @file flags.h
 * @brief Flags used by project
 * @author Paweł Chalimoniuk
 */


#ifndef INC_FLAGS_H_
#define INC_FLAGS_H_
#include "stdlib.h"
/**
 * @brief Structure holding flags related to this project
 */
typedef struct{
	uint8_t alarm; /**<Flag used by alarm event.**/
	uint8_t soilMeasurementComplete; /**<Flag used by ADC after completing measurement.**/
	uint8_t bluetoothEvent; /**<Flag used after receiving command over Bluetooth.**/
	uint8_t refreshRequest; /**<Flag used after user request data.**/
	uint8_t wateringComplete; /**<Flag indicating that pump is turned on.**/
}Flags_Struct;


#endif /* INC_FLAGS_H_ */
