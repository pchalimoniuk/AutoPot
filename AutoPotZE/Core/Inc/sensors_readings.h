/**
 * @file sensors_readings.h
 * @brief Structure with measurements used in project
 * @author Paweł Chalimoniuk
 */

#ifndef INC_SENSORS_READINGS_H_
#define INC_SENSORS_READINGS_H_
#include "stdlib.h"
#include "DHT11_sensor.h"
/**
 * @brief Structure holding readings from sensors
 */
typedef struct{
	uint16_t soilMoisturePercent; /**<Normalized moisture readings from soil**/
	uint32_t soilMoistureRaw;	/**<Raw moisture readings from soil**/
	DHT_Data dht11SensorData; /**<Struture holding data from DHT11 sensor**/
}Sensor_Readings_Struct;


#endif /* INC_SENSORS_READINGS_H_ */
