/*
 * LedChannel.h
 *
 *  Created on: Aug 15, 2022
 *      Author: DShatokhin
 */

#ifndef LEDCHANNEL_H_
#define LEDCHANNEL_H_

#include <AceButton.h>
#include "PwmController.h"
#include "EEConfig.h"
#include "ModbusAdapter.h"

using namespace ace_button;

class LedChannel : public IEventHandler  {
public:
	LedChannel(const uint8_t pin = 0, const uint8_t pwm = 0);
	virtual ~LedChannel();
	//LedChannel(const uint8_t pin,const uint8_t pwm, const uint16_t hreg, PwmController& pwmController, ModbusSerial& modbus);
	void init(const uint8_t pin, const uint8_t pwm, EEConfig* config);
	void init(AceButton* button, ButtonConfig * buttonConfig, const uint8_t pwm_id, EEConfig* config);
	void setModbusAdapter(ModbusAdapter* modbus_adapter);
	void setPWMControl(PwmController* pwm);
	void check();
	void setBrightnessLevel(const uint8_t level);
	uint8_t getBrightnessLevel();

	void handleEvent(AceButton* button, uint8_t eventType,
	        uint8_t buttonState) override;
private:
	void updateModbus();

	ButtonConfig m_buttonConfig;
	AceButton m_button;
	PwmController* m_pwmController;
	ModbusAdapter* m_modbusAdapter;
	uint8_t m_level;
	bool m_state;
	uint8_t m_pwm_id;
	uint8_t m_pin;
	bool m_channelDirectionUp;
	unsigned long m_directionResetCounter;
	bool m_channelMaxBlinkDone;

	static const uint16_t kDirectionResetDelayMs = 1000;
};

#endif /* LEDCHANNEL_H_ */
