/*
 * bluetooth_logic.h
 *
 *  Created on: 10 Dec 2020
 *      Author: Dell
 */

#ifndef INC_BLUETOOTH_LOGIC_H_
#define INC_BLUETOOTH_LOGIC_H_
#include "stdio.h"
#include "RTC_alarm.h"
#include "water_pump.h"
#include "flags.h"
#include "sensors_readings.h"
#define  BLUETOOTH_COMMAND_SET_TIME  'T'
#define   BLUETOOTH_COMMAND_SET_FREQ  'F'
#define   BLUETOOTH_COMMAND_DATA_REQUEST 'D'
#define   BLUETOOTH_COMMAND_SET_AMOUNT  'A'
#define   BLUETOOTH_COMMAND_SEND_DATA  'S'
#define   BLUETOOTH_COMMAND_FORCE_WATERING  'P'
#define   BLUETOOTH_COMMAND_SEPARATOR  '/'
#define   BLUETOOTH_COMMAND_ERROR  0
#define   BLUETOOTH_COMMAND_OK  1
uint8_t bluetooth(char *command, Water_pump_struct *pump, Flags * flags, ALARM_RTC_struct * alarm_struct);
uint8_t BLUETOOTH_INTERFACE_send_data(char * buffer, sensorReadings readings);
#endif /* INC_BLUETOOTH_LOGIC_H_ */
