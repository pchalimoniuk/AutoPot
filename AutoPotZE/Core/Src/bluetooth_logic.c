/*
 * bluetooth_logic.c
 *
 *  Created on: 10 Dec 2020
 *      Author: Dell
 */
#include "bluetooth_logic.h"
uint8_t get_time_from_command(char *command, uint8_t *hours, uint8_t *minutes,
		uint8_t *day);
uint8_t get_freq_from_command(char *command, uint8_t *hours, uint8_t *freq);
uint8_t get_amount_from_command(char *command, uint8_t *amount);
uint8_t interpet_command(char *command, water_pump *pump, uint8_t *watering_flag,
		uint8_t *refresh_flag, alarm * alarm_struct);
uint8_t bluetooth(char *command, water_pump *pump, uint8_t *watering_flag,
		uint8_t *refresh_flag, alarm * alarm_struct) {
	return interpet_command(command, pump, watering_flag,
			refresh_flag, alarm_struct);
}
uint8_t interpet_command(char *command, water_pump *pump, uint8_t *watering_flag,
		uint8_t *refresh_flag, alarm * alarm_struct) {
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t weekday = 0;
	uint8_t freq = 0;
	uint8_t amount = 10;
	switch (*command) {
	case BLUETOOTH_COMMAND_SET_TIME:
		if(!get_time_from_command(command, &hours, &minutes, &weekday)){
			return BLUETOOTH_COMMAND_ERROR;
		}
		alarm_struct->hours = hours;
		alarm_struct->minutes = minutes;
		alarm_struct->weekday = weekday;
		if(set_rtc(alarm_struct)){
			return BLUETOOTH_COMMAND_OK;
		}
		break;
	case BLUETOOTH_COMMAND_SET_FREQ: //think abut that
		if(!get_freq_from_command(command, &hours, &freq)){
			return BLUETOOTH_COMMAND_ERROR;
		}
		alarm_struct->hours_alarm = hours;
		alarm_struct->freq = freq;
		if(set_alarm(alarm_struct)){
			return BLUETOOTH_COMMAND_OK;
		}
		break;
	case BLUETOOTH_COMMAND_DATA_REQUEST:
		*refresh_flag = 1;
		break;
	case BLUETOOTH_COMMAND_SET_AMOUNT:
		if(!get_amount_from_command(command, &amount)){
			return BLUETOOTH_COMMAND_ERROR;
		}
		set_watering_time(pump, amount);
		return BLUETOOTH_COMMAND_OK;
	case BLUETOOTH_COMMAND_SEND_DATA:
		break;
	case BLUETOOTH_COMMAND_FORCE_WATERING:
		*watering_flag = 1;
		return BLUETOOTH_COMMAND_OK;
		break;
	default:
		return BLUETOOTH_COMMAND_ERROR;
	}
	return BLUETOOTH_COMMAND_ERROR;
}
uint8_t get_time_from_command(char *command, uint8_t *hours, uint8_t *minutes,
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
uint8_t get_freq_from_command(char *command, uint8_t *hours, uint8_t *freq) {
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
uint8_t get_amount_from_command(char *command, uint8_t *amount) {
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
