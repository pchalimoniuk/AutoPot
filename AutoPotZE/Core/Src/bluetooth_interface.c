/**
 * @file bluetooth_interface.c
 * @brief Functions related to interpreting and executing command received over Bluetooth
 * @author Paweł Chalimoniuk
 */

#include "bluetooth_interface.h"
/**
 * @brief Function gets time to set RTC from command
 * @param command Pointer to command
 * @param hours Pointer to variable to store hours from command
 * @param minutes Pointer to variable to store minutes from command
 * @param day Pointer to variable to store weekday from command
 * @return MAKROS: BLUETOOTH_COMMAND_OK if successful, BLUETOOTH_COMMAND_ERROR otherwise
 */
uint8_t BLUETOOTH_INTERFACE_get_time_from_command(char *command, uint8_t *hours, uint8_t *minutes,
		uint8_t *day);
/**
 * @brief Function gets period between watering from command
 * @param command Pointer to command
 * @param hours Pointer to variable to store hours from command
 * @param freq Pointer to variable to store period from command
 * @return MAKROS: BLUETOOTH_COMMAND_OK if successful, BLUETOOTH_COMMAND_ERROR otherwise
 */
uint8_t BLUETOOTH_INTERFACE_get_watering_period_from_command(char *command, uint8_t *hours, uint8_t *freq);
/**
 * @brief Function gets amount of time that pump should be working for
 * @param command Pointer to command
 * @param amount Pointer to variable to store amount from command
 * @return MAKROS: BLUETOOTH_COMMAND_OK if successful, BLUETOOTH_COMMAND_ERROR otherwise
 */
uint8_t BLUETOOTH_INTERFACE_get_amount_from_command(char *command, uint8_t *amount);
/**
 * @brief Execcute command based on action
 * @param command Pointer to command to be interpreted
 * @param pump	Pointer to water pump structure
 * @param flags	Pointer to flag structure
 * @param alarm_struct	Pointer to RTC alarm structure
 * @return MAKROS: BLUETOOTH_COMMAND_OK if successful, BLUETOOTH_COMMAND_ERROR otherwise
 */
uint8_t BLUETOOTH_INTERFACE_execute_command(char *command, Water_Pump_Struct *pump, Flags_Struct * flags, ALARM_RTC_struct * alarm_struct);
uint8_t BLUETOOTH_INTERFACE_interpret_command(char *command, Water_Pump_Struct *pump, Flags_Struct * flags, ALARM_RTC_struct * alarm_struct) {
	return BLUETOOTH_INTERFACE_execute_command(command, pump, flags, alarm_struct);
}
uint8_t BLUETOOTH_INTERFACE_execute_command(char *command, Water_Pump_Struct *pump, Flags_Struct * flags, ALARM_RTC_struct * alarm_struct) {
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t weekday = 0;
	uint8_t freq = 0;
	uint8_t amount = 10;
	switch (*command) {
	case BLUETOOTH_COMMAND_SET_TIME:
		if(!BLUETOOTH_INTERFACE_get_time_from_command(command, &hours, &minutes, &weekday)){
			return BLUETOOTH_COMMAND_ERROR;
		}
		alarm_struct->hours = hours;
		alarm_struct->minutes = minutes;
		alarm_struct->weekday = weekday;
		if(ALARM_RTC_set_rtc(alarm_struct)){
			return BLUETOOTH_COMMAND_OK;
		}
		break;
	case BLUETOOTH_COMMAND_SET_FREQ: //think abut that
		if(!BLUETOOTH_INTERFACE_get_watering_period_from_command(command, &hours, &freq)){
			return BLUETOOTH_COMMAND_ERROR;
		}
		alarm_struct->alarmTimeHour = hours;
		alarm_struct->wateringPeriod = freq;
		if(ALARM_RTC_set_alarm(alarm_struct)){
			return BLUETOOTH_COMMAND_OK;
		}
		break;
	case BLUETOOTH_COMMAND_DATA_REQUEST:
		flags->refreshRequest = 1;
		return BLUETOOTH_COMMAND_OK;
		break;
	case BLUETOOTH_COMMAND_SET_AMOUNT:
		if(!BLUETOOTH_INTERFACE_get_amount_from_command(command, &amount)){
			return BLUETOOTH_COMMAND_ERROR;
		}
		WATER_PUMP_set_watering_time(pump, amount);
		return BLUETOOTH_COMMAND_OK;
	case BLUETOOTH_COMMAND_SEND_DATA:
		break;
	case BLUETOOTH_COMMAND_FORCE_WATERING:
		WATER_PUMP_start_watering(pump);
		return BLUETOOTH_COMMAND_OK;
		break;
	default:
		return BLUETOOTH_COMMAND_ERROR;
	}
	return BLUETOOTH_COMMAND_ERROR;
}
uint8_t BLUETOOTH_INTERFACE_get_time_from_command(char *command, uint8_t *hours, uint8_t *minutes,
		uint8_t *day) {
	char *command_possition = command + 1;
	char *command_end_possition;
	if (*(command_possition) != BLUETOOTH_COMMAND_SEPARATOR) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition++;
	*hours = (uint8_t) strtol(command_possition, &command_end_possition, 10);
	if (command_possition == command_end_possition) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition = command_end_possition;
	if (*(command_possition) != BLUETOOTH_COMMAND_SEPARATOR) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition++;
	*minutes = (uint8_t) strtol(command_possition, &command_end_possition, 10);
	if (command_possition == command_end_possition) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition = command_end_possition;
	if (*(command_possition) != BLUETOOTH_COMMAND_SEPARATOR) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition++;
	*day = (uint8_t) strtol(command_possition, &command_end_possition, 10);
	if (command_possition == command_end_possition) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	return BLUETOOTH_COMMAND_OK;
}
uint8_t BLUETOOTH_INTERFACE_get_watering_period_from_command(char *command, uint8_t *hours, uint8_t *freq) {
	char *command_possition = command + 1;
	char *command_end_possition;
	if (*(command_possition) != BLUETOOTH_COMMAND_SEPARATOR) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition++;
	*hours = (uint8_t) strtol(command_possition, &command_end_possition, 10);
	if (command_possition == command_end_possition) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition = command_end_possition;
	if (*(command_possition) != BLUETOOTH_COMMAND_SEPARATOR) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition++;
	*freq = (uint8_t) strtol(  command_possition, &command_end_possition, 10);
	if (command_possition == command_end_possition) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	return BLUETOOTH_COMMAND_OK;
}
uint8_t BLUETOOTH_INTERFACE_get_amount_from_command(char *command, uint8_t *amount) {
	char *command_possition = command + 1;
	char *command_end_possition;
	if (*(command_possition) != BLUETOOTH_COMMAND_SEPARATOR) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	command_possition++;
	*amount = (uint8_t) strtol(command_possition, &command_end_possition, 10);
	if (command_possition == command_end_possition) {
		return BLUETOOTH_COMMAND_ERROR;
	}
	return BLUETOOTH_COMMAND_OK;
}

uint8_t BLUETOOTH_INTERFACE_send_data(char *buffer,Sensor_Readings_Struct readings){
	uint8_t temp = 0;
	temp = sprintf(buffer,"S/%2d/%2d/%2d/", readings.soilMoisturePercent,readings.dht11SensorData.Humidity,readings.dht11SensorData.Temperature);
	if(temp < 12){
		return BLUETOOTH_COMMAND_ERROR;
	}
	else{
		return BLUETOOTH_COMMAND_OK;
	}
}
