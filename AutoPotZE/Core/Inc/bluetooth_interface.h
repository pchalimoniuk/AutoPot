/**
 *@file bluetooth_interface.h
 *@brief Functions and makros related to interpreting and executing command received over Bluetooth<br>
 *
 *Command should follow specific format, explained below:<br>
 *In general command starts with capital letter indicating specific action followed, every meaningful information should be separated by '/'.<br>
 *Command list:<br>
 *Setting RTC time - "T/HOURS/MINUTES/WEEKDAY/" For instance "T/9/59/3/" sets current time to 9:59 Wednesday.<br>
 *Setting days between measurements - "F/HOUR_OF_MEASUREMENT/DAYS_BETWEEN_MEASUREMENTS/". Only 1-7 day periods are allowed. For instance "F/12/2/" will perform automatic measurements every other day at 12 AM.<br>
 *Setting amount of time that pump should water the plant - "A/AMOUNT/".For instance "A/20/59/3/" will make pump turn on for 20s.<br>
 *Requesting data from sensors - "D/" performs asynchronic measuremnet.For instance "D/" will start asynchronic measurement.<br>
 *Sending data - Sending data form uc to Bluetooth device - "S/SOIL_MOISTURE/TEMPERTURE/AIR_MOISTURE/" For instance "S/70/29/30/" means 70% soil moisture, 29 Celcius degrees and 30% air humidity.<br>
 *@author Paweł Chalimoniuk
 **/
#ifndef INC_BLUETOOTH_INTERFACE_H_
#define INC_BLUETOOTH_INTERFACE_H_
#include "stdio.h"
#include "RTC_alarm.h"
#include "water_pump.h"
#include "flags.h"
#include "sensors_readings.h"
#define   BLUETOOTH_COMMAND_SET_TIME  'T' /**<Makro in command indicating that user wants to change RTC time.**/
#define   BLUETOOTH_COMMAND_SET_FREQ  'F' /**<Makro in command indicating that user wants to change how often measurement should be performed.**/
#define   BLUETOOTH_COMMAND_DATA_REQUEST 'D'	/**<Makro in command indicating that user wants to receive current soil measurements.**/
#define   BLUETOOTH_COMMAND_SET_AMOUNT  'A'	/**<Makro in command indicating that user wants to change amount of water used to water the plant.**/
#define   BLUETOOTH_COMMAND_SEND_DATA  'S'	/**<Makro in command indicating that data was send.**/
#define   BLUETOOTH_COMMAND_FORCE_WATERING  'P'	/**<Makro in command indicating that user want to force watering of the plant regardless of soil moisture level.**/
#define   BLUETOOTH_COMMAND_SEPARATOR  '/'	/**<Separator used in command.**/
#define   BLUETOOTH_COMMAND_ERROR  0	/**<Makro indicating setting time in command.**/
#define   BLUETOOTH_COMMAND_OK  1	/**<Makro indicating successful execution of command.**/
/**
 * @brief Function used to interpret command according to rules of custom protocol
 * @param command Pointer to command to be interpreted
 * @param pump	Pointer to water pump structure
 * @param flags	Pointer to flag structure
 * @param alarm_struct	Pointer to RTC alarm structure
 * @return MAKROS: BLUETOOTH_COMMAND_OK if successful, BLUETOOTH_COMMAND_ERROR otherwise
 */
uint8_t BLUETOOTH_INTERFACE_interpret_command(char *command, Water_Pump_Struct *pump, Flags_Struct * flags, ALARM_RTC_struct * alarm_struct);
/**
 * @brief Prepares command with sensor data according to rules of protocol
 * @param buffer Pointer to char array that will be filled with data - should be at least 12 characters long
 * @param readings struct with readings from sensors
 * @return MAKROS: BLUETOOTH_COMMAND_OK if successful, BLUETOOTH_COMMAND_ERROR otherwise
 */
uint8_t BLUETOOTH_INTERFACE_send_data(char * buffer, Sensor_Readings_Struct readings);
#endif /* INC_BLUETOOTH_INTERFACE_H_ */
