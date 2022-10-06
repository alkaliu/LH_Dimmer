/*
 * ConfigHW.h
 *
 *  Created on: Sep 10, 2022
 *      Author: DShatokhin
 */

#ifndef CONFIGHW_H_
#define CONFIGHW_H_

#define SERIAL_DEBUG_SPEED 115200
#define EEPROM_MAGIC_FLAG  0x7CE2
#define EEPROM_MAGIC_FLAG_END 0x1A91
#define EEPROM_OFFSET 16 // some of chips has corrupted first blocks

//#define TEST_BOARD
#define V1_BOARD

#ifdef TEST_BOARD
#define CHANNEL_1_PIN  4
#define CHANNEL_2_PIN  5
#define CHANNEL_3_PIN  0
#define CHANNEL_4_PIN  0
#define CHANNEL_5_PIN  0
#define CHANNEL_6_PIN  0
#define CHANNEL_7_PIN  0
#define CHANNEL_8_PIN  0
#define CHANNELS_USED  2

#define CHANNEL_PIN_ACTIVESTATE LOW
#define RESET_PIN  6
#define RESET_PIN_ACTIVESTATE LOW
#endif

#ifdef V1_BOARD
#define CHANNEL_1_PIN  4
#define CHANNEL_2_PIN  5
#define CHANNEL_3_PIN  6
#define CHANNEL_4_PIN  7
#define CHANNEL_5_PIN  8
#define CHANNEL_6_PIN  21
#define CHANNEL_7_PIN  20
#define CHANNEL_8_PIN  19
#define CHANNELS_USED  8
#define CHANNEL_PIN_ACTIVESTATE LOW
#define RESET_PIN  9
#define RESET_PIN_ACTIVESTATE LOW
#endif

#endif /* CONFIGHW_H_ */
