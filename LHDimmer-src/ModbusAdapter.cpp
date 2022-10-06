/*
 * ModbusAdapter.cpp
 *
 *  Created on: Sep 1, 2022
 *      Author: DShatokhin
 */

#include "ModbusAdapter.h"

const LHModbusRegister LHREGMAP_T[] PROGMEM = {
		     { 100,  HOLDING_REG},          // RS485_SPEED
			 { 101,  HOLDING_REG},          // RS485_PARITY
			 { 102,  HOLDING_REG},          // RS485_STOPBIT
			 { 103,  HOLDING_REG},          // RS485_ADDRESS
			 { 104,  HOLDING_REG},          // DEVICE_REBOOT
			 { 105,  HOLDING_REG},          // PWM_FREQ
			 { 120,  INPUT_REG},            // CPU_VOLTAGE
			 { 121,  INPUT_REG},            // DEVICE_UPTIME_L
			 { 122,  INPUT_REG},            // DEVICE_UPTIME_H
			 { 123,  INPUT_REG},            // DEVICE_TEMP
			 { 0,    DISCRETE_INPUT},       // BUTTON_STATE_1
			 { 1,    DISCRETE_INPUT},       // BUTTON_STATE_2
			 { 2,    DISCRETE_INPUT},       // BUTTON_STATE_3
			 { 3,    DISCRETE_INPUT},       // BUTTON_STATE_4
			 { 4,    DISCRETE_INPUT},       // BUTTON_STATE_5
			 { 5,    DISCRETE_INPUT},       // BUTTON_STATE_6
			 { 6,    DISCRETE_INPUT},       // BUTTON_STATE_7
			 { 7,    DISCRETE_INPUT},       // BUTTON_STATE_8
			 { 0,    COIL},                 // CHANNEL_STATE_1
			 { 1,    COIL},                 // CHANNEL_STATE_2
			 { 2,    COIL},                 // CHANNEL_STATE_3
			 { 3,    COIL},                 // CHANNEL_STATE_4
			 { 4,    COIL},                 // CHANNEL_STATE_5
			 { 5,    COIL},                 // CHANNEL_STATE_6
			 { 6,    COIL},                 // CHANNEL_STATE_7
			 { 7,    COIL},                 // CHANNEL_STATE_8
			 { 200,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_1
			 { 201,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_2
			 { 202,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_3
			 { 203,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_4
			 { 204,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_5
			 { 205,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_6
			 { 206,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_7
			 { 207,  HOLDING_REG},          // CHANNEL_BRIGHTNESS_8
			 { 210,  HOLDING_REG},          // CHANNEL_MAX_PWM_1
			 { 211,  HOLDING_REG},          // CHANNEL_MAX_PWM_2
			 { 212,  HOLDING_REG},          // CHANNEL_MAX_PWM_3
			 { 213,  HOLDING_REG},          // CHANNEL_MAX_PWM_4
			 { 214,  HOLDING_REG},          // CHANNEL_MAX_PWM_5
			 { 215,  HOLDING_REG},          // CHANNEL_MAX_PWM_6
			 { 216,  HOLDING_REG},          // CHANNEL_MAX_PWM_7
			 { 217,  HOLDING_REG},          // CHANNEL_MAX_PWM_8
			 { 220,  HOLDING_REG},          // CHANNEL_MIN_PWM_1
			 { 221,  HOLDING_REG},          // CHANNEL_MIN_PWM_2
			 { 222,  HOLDING_REG},          // CHANNEL_MIN_PWM_3
			 { 223,  HOLDING_REG},          // CHANNEL_MIN_PWM_4
			 { 224,  HOLDING_REG},          // CHANNEL_MIN_PWM_5
			 { 225,  HOLDING_REG},          // CHANNEL_MIN_PWM_6
			 { 226,  HOLDING_REG},          // CHANNEL_MIN_PWM_7
			 { 227,  HOLDING_REG},          // CHANNEL_MIN_PWM_8
			 { 230,  HOLDING_REG},          // BUTTON_LONGPRESS_DELAY
			 { 231,  HOLDING_REG},          // BRIGHTNESS_SPEED
			 { 232,  HOLDING_REG}           // DOUBLE_CLICK_DELAY
			 };

ModbusAdapter::ModbusAdapter() : mb(NULL) {
	// TODO Auto-generated constructor stub

}

ModbusAdapter::~ModbusAdapter() {
	// TODO Auto-generated destructor stub
}

void ModbusAdapter::init(ModbusSerial* modbus, EEConfig* config) {
	mb = modbus;
	if (!mb) {
		Serial.println(F("ModbusSerial is NULL"));
		return;
	}

	uint8_t reg_size =  sizeof(LHREGMAP_T)/sizeof(LHModbusRegister);
	for (uint8_t i = 0; i < reg_size; ++i) {
		uint8_t type = pgm_read_byte(&(LHREGMAP_T[i].type));
		uint16_t addr = pgm_read_word(&(LHREGMAP_T[i].address));
		Serial.println(addr);
		switch (type) {
		case COIL:
			mb->addCoil(addr);
			Serial.println(F("ModbusAdapter(): add COIL"));
			break;
		case DISCRETE_INPUT:
			mb->addIsts(addr);
			Serial.println(F("ModbusAdapter(): add DISCRETE_INPUT"));
			break;
		case HOLDING_REG:
			mb->addHreg(addr);
			Serial.println(F("ModbusAdapter(): add HOLDING_REG"));
			break;
		case INPUT_REG:
			mb->addIreg(addr);
			Serial.println(F("ModbusAdapter(): add INPUT_REG"));
			break;
		default:
			break;

		}
	}

	setRS485_SPEED(config->ee_rs485_speed);
	setRS485_PARITY(config->ee_rs485_parity);
	setRS485_STOPBIT(config->ee_rs485_stopbit);
	setRS485_ADDRESS(config->ee_rs485_address);
	setPWM_FREQ(config->ee_pwm_freq);
	setCHANNEL_BRIGHTNESS_1(config->ee_channel_brightness_1);
	setCHANNEL_BRIGHTNESS_2(config->ee_channel_brightness_2);
	setCHANNEL_BRIGHTNESS_3(config->ee_channel_brightness_3);
	setCHANNEL_BRIGHTNESS_4(config->ee_channel_brightness_4);
	setCHANNEL_BRIGHTNESS_5(config->ee_channel_brightness_5);
	setCHANNEL_BRIGHTNESS_6(config->ee_channel_brightness_6);
	setCHANNEL_BRIGHTNESS_7(config->ee_channel_brightness_7);
	setCHANNEL_BRIGHTNESS_8(config->ee_channel_brightness_8);
	setCHANNEL_MAX_PWM_1(config->ee_channel_max_pwm_1);
	setCHANNEL_MAX_PWM_2(config->ee_channel_max_pwm_2);
	setCHANNEL_MAX_PWM_3(config->ee_channel_max_pwm_3);
	setCHANNEL_MAX_PWM_4(config->ee_channel_max_pwm_4);
	setCHANNEL_MAX_PWM_5(config->ee_channel_max_pwm_5);
	setCHANNEL_MAX_PWM_6(config->ee_channel_max_pwm_6);
	setCHANNEL_MAX_PWM_7(config->ee_channel_max_pwm_7);
	setCHANNEL_MAX_PWM_8(config->ee_channel_max_pwm_8);
	setCHANNEL_MIN_PWM_1(config->ee_channel_min_pwm_1);
	setCHANNEL_MIN_PWM_2(config->ee_channel_min_pwm_2);
	setCHANNEL_MIN_PWM_3(config->ee_channel_min_pwm_3);
	setCHANNEL_MIN_PWM_4(config->ee_channel_min_pwm_4);
	setCHANNEL_MIN_PWM_5(config->ee_channel_min_pwm_5);
	setCHANNEL_MIN_PWM_6(config->ee_channel_min_pwm_6);
	setCHANNEL_MIN_PWM_7(config->ee_channel_min_pwm_7);
	setCHANNEL_MIN_PWM_8(config->ee_channel_min_pwm_8);
	setBUTTON_LONGPRESS_DELAY(config->ee_button_longpress_delay);
	setBRIGHTNESS_SPEED(config->ee_button_brightness_speed);
	setDOUBLE_CLICK_DELAY(config->ee_button_double_click_delay);
	setDEVICE_REBOOT(0);
	setDEVICE_TEMP(0xFFFF);

	Serial.println(F("ModbusAdapter init done!!!"));
}

void  ModbusAdapter::getEEConfig(EEConfig* data)
{
	if(data) {
		data->ee_rs485_speed =                getRS485_SPEED           ();
		data->ee_rs485_parity =               getRS485_PARITY          ();
		data->ee_rs485_stopbit =              getRS485_STOPBIT         ();
		data->ee_rs485_address =              getRS485_ADDRESS         ();
		data->ee_pwm_freq =                   getPWM_FREQ              ();
		data->ee_channel_brightness_1 =       getCHANNEL_BRIGHTNESS_1  ();
		data->ee_channel_brightness_2 =       getCHANNEL_BRIGHTNESS_2  ();
		data->ee_channel_brightness_3 =       getCHANNEL_BRIGHTNESS_3  ();
		data->ee_channel_brightness_4 =       getCHANNEL_BRIGHTNESS_4  ();
		data->ee_channel_brightness_5 =       getCHANNEL_BRIGHTNESS_5  ();
		data->ee_channel_brightness_6 =       getCHANNEL_BRIGHTNESS_6  ();
		data->ee_channel_brightness_7 =       getCHANNEL_BRIGHTNESS_7  ();
		data->ee_channel_brightness_8 =       getCHANNEL_BRIGHTNESS_8  ();
		data->ee_channel_max_pwm_1 =          getCHANNEL_MAX_PWM_1     ();
		data->ee_channel_max_pwm_2 =          getCHANNEL_MAX_PWM_2     ();
		data->ee_channel_max_pwm_3 =          getCHANNEL_MAX_PWM_3     ();
		data->ee_channel_max_pwm_4 =          getCHANNEL_MAX_PWM_4     ();
		data->ee_channel_max_pwm_5 =          getCHANNEL_MAX_PWM_5     ();
		data->ee_channel_max_pwm_6 =          getCHANNEL_MAX_PWM_6     ();
		data->ee_channel_max_pwm_7 =          getCHANNEL_MAX_PWM_7     ();
		data->ee_channel_max_pwm_8 =          getCHANNEL_MAX_PWM_8     ();
		data->ee_channel_min_pwm_1 =          getCHANNEL_MIN_PWM_1     ();
		data->ee_channel_min_pwm_2 =          getCHANNEL_MIN_PWM_2     ();
		data->ee_channel_min_pwm_3 =          getCHANNEL_MIN_PWM_3     ();
		data->ee_channel_min_pwm_4 =          getCHANNEL_MIN_PWM_4     ();
		data->ee_channel_min_pwm_5 =          getCHANNEL_MIN_PWM_5     ();
		data->ee_channel_min_pwm_6 =          getCHANNEL_MIN_PWM_6     ();
		data->ee_channel_min_pwm_7 =          getCHANNEL_MIN_PWM_7     ();
		data->ee_channel_min_pwm_8 =          getCHANNEL_MIN_PWM_8     ();
		data->ee_button_longpress_delay =     getBUTTON_LONGPRESS_DELAY();
		data->ee_button_brightness_speed =    getBRIGHTNESS_SPEED      ();
		data->ee_button_double_click_delay =  getDOUBLE_CLICK_DELAY    ();
	}
}

uint16_t ModbusAdapter::getLHAddress(LHModbusRegisterName lh_reg)
{
	return pgm_read_word(&(LHREGMAP_T[lh_reg].address));
}

void ModbusAdapter::setChannelBrightness(uint8_t id, uint16_t level) {
	switch (id) {
		case 0:
			setCHANNEL_BRIGHTNESS_1(level);
			break;
		case 1:
			setCHANNEL_BRIGHTNESS_2(level);
			break;
		case 2:
			setCHANNEL_BRIGHTNESS_3(level);
			break;
		case 3:
			setCHANNEL_BRIGHTNESS_4(level);
			break;
		case 4:
			setCHANNEL_BRIGHTNESS_5(level);
			break;
		case 5:
			setCHANNEL_BRIGHTNESS_6(level);
			break;
		case 6:
			setCHANNEL_BRIGHTNESS_7(level);
			break;
		case 7:
			setCHANNEL_BRIGHTNESS_8(level);
			break;

		default:
			break;
	}
}

void ModbusAdapter::setChannelState(uint8_t id, bool state) {
	switch (id) {
		case 0:
			setCHANNEL_STATE_1(state);
			break;
		case 1:
			setCHANNEL_STATE_2(state);
			break;
		case 2:
			setCHANNEL_STATE_3(state);
			break;
		case 3:
			setCHANNEL_STATE_4(state);
			break;
		case 4:
			setCHANNEL_STATE_5(state);
			break;
		case 5:
			setCHANNEL_STATE_6(state);
			break;
		case 6:
			setCHANNEL_STATE_7(state);
			break;
		case 7:
			setCHANNEL_STATE_8(state);
			break;

		default:
			break;
	}
}

uint16_t ModbusAdapter::getChannelBrightness(uint8_t id) {
	uint16_t result = 0xffff;
	switch (id) {
		case 0:
			result = getCHANNEL_BRIGHTNESS_1();
			break;
		case 1:
			result = getCHANNEL_BRIGHTNESS_2();
			break;
		case 2:
			result = getCHANNEL_BRIGHTNESS_3();
			break;
		case 3:
			result = getCHANNEL_BRIGHTNESS_4();
			break;
		case 4:
			result = getCHANNEL_BRIGHTNESS_5();
			break;
		case 5:
			result = getCHANNEL_BRIGHTNESS_6();
			break;
		case 6:
			result = getCHANNEL_BRIGHTNESS_7();
			break;
		case 7:
			result = getCHANNEL_BRIGHTNESS_8();
			break;

		default:
			break;
	}
	return result;
}

bool ModbusAdapter::getChannelState(uint8_t id) {
	bool result = false;
	switch (id) {
		case 0:
			result = getCHANNEL_STATE_1();
			break;
		case 1:
			result = getCHANNEL_STATE_2();
			break;
		case 2:
			result = getCHANNEL_STATE_3();
			break;
		case 3:
			result = getCHANNEL_STATE_4();
			break;
		case 4:
			result = getCHANNEL_STATE_5();
			break;
		case 5:
			result = getCHANNEL_STATE_6();
			break;
		case 6:
			result = getCHANNEL_STATE_7();
			break;
		case 7:
			result = getCHANNEL_STATE_8();
			break;

		default:
			break;
	}
	return result;
}

void ModbusAdapter::setButtonState(uint8_t id, bool state) {
	switch (id) {
		case 0:
			setBUTTON_STATE_1(state);
			break;
		case 1:
			setBUTTON_STATE_2(state);
			break;
		case 2:
			setBUTTON_STATE_3(state);
			break;
		case 3:
			setBUTTON_STATE_4(state);
			break;
		case 4:
			setBUTTON_STATE_5(state);
			break;
		case 5:
			setBUTTON_STATE_6(state);
			break;
		case 6:
			setBUTTON_STATE_7(state);
			break;
		case 7:
			setBUTTON_STATE_8(state);
			break;

		default:
			break;
	}
}

bool ModbusAdapter::getButtonState(uint8_t id) {
	bool result = false;
	switch (id) {
		case 0:
			result = getBUTTON_STATE_1();
			break;
		case 1:
			result = getBUTTON_STATE_2();
			break;
		case 2:
			result = getBUTTON_STATE_3();
			break;
		case 3:
			result = getBUTTON_STATE_4();
			break;
		case 4:
			result = getBUTTON_STATE_5();
			break;
		case 5:
			result = getBUTTON_STATE_6();
			break;
		case 6:
			result = getBUTTON_STATE_7();
			break;
		case 7:
			result = getBUTTON_STATE_8();
			break;

		default:
			break;
	}
	return result;
}
