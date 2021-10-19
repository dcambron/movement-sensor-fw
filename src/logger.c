/*
 * logger.c
 *
 *  Created on: Aug 30, 2021
 *      Author: danie
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BDK.h>
#include <I2CEeprom.h>
#include <api/led_api.h>
#include <stimer.h>

#include "logger.h"

static I2CEeprom m24rf64;

static int index1 = 0;
static int lux_threshold = 750;

#define CSN_BSEC_EEPROM_I2C_ADDR       (0x50)
#define CSN_BSEC_EEPROM_PAGE_SIZE      (4)
#define CSN_BSEC_EEPROM_ADDR           (4)
#define LOG_ENTRY_SIZE                 (5)
#define CSN_BSEC_EEPROM_MAGIC          {'C', 'S', 'N', ' ', 'B', 'S', 'E', 'C', 0}
#define CSN_BSEC_EEPROM_MAGIC_SIZE     (9)

void logger_init()
{
	char buf[4];
	I2CEeprom_Initialize(CSN_BSEC_EEPROM_I2C_ADDR, 4, &m24rf64);

	I2CEeprom_Read(0,buf,4,&m24rf64);

	memcpy(&index1,buf,4);

	if(index1 > 1600)
	{
		index1 = 0;
	}
}


void logger_write_log_entry(uint8_t event)
{
	char data[LOG_ENTRY_SIZE] = {0};
	uint32_t secs = HAL_RTC_Get_Seconds()/10;
	memcpy(data,&secs,4);
	data[4] = event;
	//log entry format: 4 bytes timestamp,1 bytes event code


	I2CEeprom_Write(CSN_BSEC_EEPROM_ADDR + (index1 * LOG_ENTRY_SIZE),data,LOG_ENTRY_SIZE,&m24rf64);

	index1++;

	memcpy(data,&index1,4);

	I2CEeprom_Write(0,data,4,&m24rf64);

}

int logger_read_log_entry(uint8_t* data)
{
	static int seq_num = 0;

	I2CEeprom_Read(CSN_BSEC_EEPROM_ADDR + (seq_num * LOG_ENTRY_SIZE),data,LOG_ENTRY_SIZE,&m24rf64);

	seq_num++;

	if(seq_num >= index1)
	{
		seq_num = 0;
	}


	return seq_num;
}

void logger_clear_log()
{
	char data[LOG_ENTRY_SIZE] = {0};

	I2CEeprom_Write(0,data,4,&m24rf64);
	index1 = 0;

}

int logger_get_size()
{
	return index1;
}

int logger_get_lux()
{
	return lux_threshold;
}

void logger_set_lux(int lux)
{
	lux_threshold = lux;
}
