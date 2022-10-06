/*
 * EEConfig.h
 *
 *  Created on: Aug 21, 2022
 *      Author: DShatokhin
 */

#ifndef EECONFIG_H_
#define EECONFIG_H_

#include <Arduino.h>

struct __attribute__ ((packed)) EEConfig {
	uint16_t magic;
	uint16_t ee_rs485_speed;
	uint16_t ee_rs485_parity;
	uint16_t ee_rs485_stopbit;
	uint16_t ee_rs485_address;
	uint16_t ee_pwm_freq;
	uint16_t ee_channel_brightness_1;
	uint16_t ee_channel_brightness_2;
	uint16_t ee_channel_brightness_3;
	uint16_t ee_channel_brightness_4;
	uint16_t ee_channel_brightness_5;
	uint16_t ee_channel_brightness_6;
	uint16_t ee_channel_brightness_7;
	uint16_t ee_channel_brightness_8;
	uint16_t ee_channel_max_pwm_1;
	uint16_t ee_channel_max_pwm_2;
	uint16_t ee_channel_max_pwm_3;
	uint16_t ee_channel_max_pwm_4;
	uint16_t ee_channel_max_pwm_5;
	uint16_t ee_channel_max_pwm_6;
	uint16_t ee_channel_max_pwm_7;
	uint16_t ee_channel_max_pwm_8;
	uint16_t ee_channel_min_pwm_1;
	uint16_t ee_channel_min_pwm_2;
	uint16_t ee_channel_min_pwm_3;
	uint16_t ee_channel_min_pwm_4;
	uint16_t ee_channel_min_pwm_5;
	uint16_t ee_channel_min_pwm_6;
	uint16_t ee_channel_min_pwm_7;
	uint16_t ee_channel_min_pwm_8;
	uint16_t ee_button_longpress_delay;
	uint16_t ee_button_brightness_speed;
	uint16_t ee_button_double_click_delay;
	uint16_t magic_end;
};

#endif /* EECONFIG_H_ */
