/*
 * logger.h
 *
 *  Created on: Aug 30, 2021
 *      Author: danie
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_


#define LOGGER_EVT_MOVEMENT_DETECTED (0)
#define LOGGER_EVT_LIGHTS_ON (1)
#define LOGGER_EVT_LIGHTS_OFF (2)

void logger_init();

void logger_write_log_entry(uint8_t event);

int logger_read_log_entry(uint8_t* data);

int logger_get_size();

int logger_get_lux();

void logger_set_lux(int lux);


void logger_clear_log();


#endif /* SRC_LOGGER_H_ */
