/*
 * bluetooth_logic.c
 *
 *  Created on: 10 Dec 2020
 *      Author: Dell
 */
#include "bluetooth_logic.h"
uint8_t BLUETOOTH_INTERFACE_get_time_from_command(char *command, uint8_t *hours, uint8_t *minutes,
		uint8_t *day);
uint8_t BLUETOOTH_INTERFACE_get_watering_period_from_command(char *command, uint8_t *hours, uint8_t *freq);
uint8_t BLUETOOTH_INTERFACE_get_amount_from_command(char *command, uint8_t *amount);
uint8_t BLUETOOTH_INTERFACE_interpet_command(char *command, Water_pump_struct *pump, Flags * flags, ALARM_RTC_struct * alarm_struct);
uint8_t bluetooth(char *command, Water_pump_struct *pump, Flags * flags, ALARM_RTC_struct * alarm_struct) {
	return BLUETOOTH_INTERFACE_interpet_command(command, pump, flags, alarm_struct);
}
uint8_t BLUETOOTH_INTERFACE_interpet_command(char *command, Water_pump_struct *pump, Flags * flags, ALARM_RTC_struct * alarm_struct) {
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
		flags->wateringInProgress = 1;
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

uint8_t BLUETOOTH_INTERFACE_send_data(char *buffer,sensorReadings readings){
	sprintf(buffer,"S/%2d/%2d/%3d/", readings.soilMoisturePercent,readings.airMoisture,readings.airTemperature);
	return (uint8_t)sizeof(buffer);
}
