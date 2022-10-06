/*
 * ModbusAdapter.h
 *
 *  Created on: Sep 1, 2022
 *      Author: DShatokhin
 */

#ifndef MODBUSADAPTER_H_
#define MODBUSADAPTER_H_

#include "Arduino.h"
#include <Modbus.h>
#include <ModbusSerial.h>
#include "EEConfig.h"

#define addParameter(Param, type, r_type) \
	r_type get##Param() { return getParam<type, r_type>(getLHAddress(Param)); } \
	void set##Param(r_type value) { setParam<type, r_type>(value, getLHAddress(Param));	}

#define initRegMap(Regmap, RegID, RegAddr, RegType) \
		Regmap[RegID].address = RegAddr; \
	    Regmap[RegID].type = RegType;

enum LHModbusRegisterType : uint8_t {
	COIL = 0,
	DISCRETE_INPUT,
	INPUT_REG,
	HOLDING_REG,
};

struct LHModbusRegister {
	uint16_t address;
	LHModbusRegisterType type;
};

enum LHModbusRegisterName : uint8_t {
	RS485_SPEED = 0,
	RS485_PARITY,
	RS485_STOPBIT,
	RS485_ADDRESS,
	DEVICE_REBOOT,
	PWM_FREQ,
	CPU_VOLTAGE,
	DEVICE_UPTIME_L,
	DEVICE_UPTIME_H,
	DEVICE_TEMP,
	BUTTON_STATE_1,
	BUTTON_STATE_2,
	BUTTON_STATE_3,
	BUTTON_STATE_4,
	BUTTON_STATE_5,
	BUTTON_STATE_6,
	BUTTON_STATE_7,
	BUTTON_STATE_8,
	CHANNEL_STATE_1,
	CHANNEL_STATE_2,
	CHANNEL_STATE_3,
	CHANNEL_STATE_4,
	CHANNEL_STATE_5,
	CHANNEL_STATE_6,
	CHANNEL_STATE_7,
	CHANNEL_STATE_8,
	CHANNEL_BRIGHTNESS_1,
	CHANNEL_BRIGHTNESS_2,
	CHANNEL_BRIGHTNESS_3,
	CHANNEL_BRIGHTNESS_4,
	CHANNEL_BRIGHTNESS_5,
	CHANNEL_BRIGHTNESS_6,
	CHANNEL_BRIGHTNESS_7,
	CHANNEL_BRIGHTNESS_8,
	CHANNEL_MAX_PWM_1,
	CHANNEL_MAX_PWM_2,
	CHANNEL_MAX_PWM_3,
	CHANNEL_MAX_PWM_4,
	CHANNEL_MAX_PWM_5,
	CHANNEL_MAX_PWM_6,
	CHANNEL_MAX_PWM_7,
	CHANNEL_MAX_PWM_8,
	CHANNEL_MIN_PWM_1,
	CHANNEL_MIN_PWM_2,
	CHANNEL_MIN_PWM_3,
	CHANNEL_MIN_PWM_4,
	CHANNEL_MIN_PWM_5,
	CHANNEL_MIN_PWM_6,
	CHANNEL_MIN_PWM_7,
	CHANNEL_MIN_PWM_8,
	BUTTON_LONGPRESS_DELAY,
	BRIGHTNESS_SPEED,
	DOUBLE_CLICK_DELAY,
	MR_MAX,
};

class ModbusAdapter {
public:
	ModbusAdapter();
	virtual ~ModbusAdapter();
	void init(ModbusSerial* modbus, EEConfig* config);

	addParameter(RS485_SPEED,            HOLDING_REG,    uint16_t)
	addParameter(RS485_PARITY,           HOLDING_REG,    uint16_t)
	addParameter(RS485_STOPBIT,          HOLDING_REG,    uint16_t)
	addParameter(RS485_ADDRESS,          HOLDING_REG,    uint16_t)
	addParameter(DEVICE_REBOOT,          HOLDING_REG,    uint16_t)
	addParameter(PWM_FREQ,               HOLDING_REG,    uint16_t)
	addParameter(CPU_VOLTAGE,            INPUT_REG,      uint16_t)
	addParameter(DEVICE_UPTIME_L,        INPUT_REG,      uint16_t)
	addParameter(DEVICE_UPTIME_H,        INPUT_REG,      uint16_t)
	addParameter(DEVICE_TEMP,            INPUT_REG,      uint16_t)
	addParameter(BUTTON_STATE_1,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_2,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_3,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_4,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_5,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_6,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_7,         DISCRETE_INPUT, bool)
	addParameter(BUTTON_STATE_8,         DISCRETE_INPUT, bool)
	addParameter(CHANNEL_STATE_1,        COIL,           bool)
	addParameter(CHANNEL_STATE_2,        COIL,           bool)
	addParameter(CHANNEL_STATE_3,        COIL,           bool)
	addParameter(CHANNEL_STATE_4,        COIL,           bool)
	addParameter(CHANNEL_STATE_5,        COIL,           bool)
	addParameter(CHANNEL_STATE_6,        COIL,           bool)
	addParameter(CHANNEL_STATE_7,        COIL,           bool)
	addParameter(CHANNEL_STATE_8,        COIL,           bool)
	addParameter(CHANNEL_BRIGHTNESS_1,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_2,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_3,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_4,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_5,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_6,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_7,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_BRIGHTNESS_8,   HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_1,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_2,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_3,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_4,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_5,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_6,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_7,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MAX_PWM_8,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_1,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_2,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_3,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_4,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_5,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_6,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_7,      HOLDING_REG,    uint16_t)
	addParameter(CHANNEL_MIN_PWM_8,      HOLDING_REG,    uint16_t)
	addParameter(BUTTON_LONGPRESS_DELAY, HOLDING_REG,    uint16_t)
	addParameter(BRIGHTNESS_SPEED,       HOLDING_REG,    uint16_t)
	addParameter(DOUBLE_CLICK_DELAY,     HOLDING_REG,    uint16_t)

	void setChannelBrightness(uint8_t id, uint16_t level);
	uint16_t getChannelBrightness(uint8_t id);

	void setChannelState(uint8_t id, bool state);
	bool getChannelState(uint8_t id);

	void setButtonState(uint8_t id, bool state);
	bool getButtonState(uint8_t id);

	void getEEConfig(EEConfig* data);

private:
	uint16_t getLHAddress(LHModbusRegisterName lh_reg);

	template<LHModbusRegisterType TYPE, typename RETURN_TYPE>
	RETURN_TYPE getParam(uint16_t address) {
		RETURN_TYPE value = 0xFFFF;

		if (mb) {
			switch (TYPE) {
			case COIL:
				value = mb->Coil(address);
				break;
			case DISCRETE_INPUT:
				value = mb->Ists(address);
				break;
			case HOLDING_REG:
				value = mb->Hreg(address);
				break;
			case INPUT_REG:
				value = mb->Ireg(address);
				break;
			default:
				break;

			}
		}
		return value;
	}

	template<LHModbusRegisterType TYPE, typename RETURN_TYPE>
	void setParam(RETURN_TYPE value, uint16_t address) {
		if (mb) {
			switch (TYPE) {
			case COIL:
				value = mb->Coil(address, value);
				break;
			case DISCRETE_INPUT:
				value = mb->Ists(address, value);
				break;
			case HOLDING_REG:
				value = mb->Hreg(address, value);
				break;
			case INPUT_REG:
				value = mb->Ireg(address, value);
				break;
			default:
				break;

			}
		}
	}

	ModbusSerial* mb;
};

#endif /* MODBUSADAPTER_H_ */
