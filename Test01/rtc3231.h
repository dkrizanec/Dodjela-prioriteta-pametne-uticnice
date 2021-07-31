/* DS3231 Real Time Clock AVR Lirary
 *
 * Copyright (C) 2016-2017 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by Adafruit Industries. MIT license.
 */

#ifndef __DS3231_H__
#define __DS3231_H__

#include <stdio.h>
#include <avr/eeprom.h>

#define RTC_WADDR 0b11010000
#define RTC_RADDR 0b11010001

uint8_t *ptr;

struct rtc_time
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
};

struct rtc_date
{
	uint8_t wday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};

struct unesenoVrijeme
{
	uint8_t horizontalno; 
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
};



struct uneseniDatum
{
	uint8_t	wday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};


struct vrijemeTrajanja
{
	uint8_t trajanjeHorizontalno; 
	uint8_t stanjeZaOutputCheck;
	uint8_t brojReleja; 
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t relay1; 
	uint8_t relay2; 
	uint8_t relay3; 
	uint8_t relay4; 
};


/*
 * Clock initialization
 */
void rtc3231_init(void);

/**
 * Reading time from clock
 * @time: time structure with reading data
 */
void rtc3231_read_time(struct rtc_time *time);

/**
 * Reading date from clock
 * @date: date structure with reading data
 */
void rtc3231_read_date(struct rtc_date *date);

/**
 * Reading date and time from clock
 * @time: time structure with reading data
 * @date: date structure with reading data
 */
void rtc3231_read_datetime(struct rtc_time *time, struct rtc_date *date);

/**
 * Writing new time in clock
 * @time: time structure for writing data
 */
void rtc3231_write_time(struct rtc_time *time);

/**
 * Writing new date in clock
 * @date: date structure for writing data
 */
void rtc3231_write_date(struct rtc_date *date);

#endif
