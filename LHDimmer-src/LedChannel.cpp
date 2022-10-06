/*
 * LedChannel.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: DShatokhin
 */

#include "LedChannel.h"
#include "ConfigHW.h"

LedChannel::~LedChannel() {
	// TODO Auto-generated destructor stub
}

LedChannel::LedChannel(const uint8_t pin, const uint8_t pwm)
	: m_buttonConfig()
	, m_button(&m_buttonConfig)
    , m_pwmController(NULL)
	, m_modbusAdapter(NULL)
	, m_level(0)
	, m_state(false)
	, m_pwm_id(pwm)
	, m_pin(pin)
	, m_channelDirectionUp(true)
	, m_directionResetCounter(0)
	, m_channelMaxBlinkDone(false)
{

}

void LedChannel::init(const uint8_t pin, const uint8_t pwm, EEConfig* ee_config)
{
	m_pin = pin;
	m_pwm_id = pwm;

	m_buttonConfig.setFeature(ButtonConfig::kFeatureClick);
	m_buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
	  // These suppressions not really necessary but cleaner.
	m_buttonConfig.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
	m_buttonConfig.setRepeatPressDelay(ee_config->ee_button_longpress_delay); // delay to first Repeat event - was 1500
	m_buttonConfig.setRepeatPressInterval(ee_config->ee_button_brightness_speed); // further Repeat events interval was 40
	m_buttonConfig.setClickDelay(1500);
	m_buttonConfig.setDebounceDelay(30);

	pinMode(pin, INPUT_PULLUP); // should be INPUT for prod
	m_button.init(pin, !CHANNEL_PIN_ACTIVESTATE, pwm);
	m_buttonConfig.setIEventHandler(this);

	if (m_modbusAdapter && m_pwmController) {
		m_pwmController->enable(m_pwm_id, false);                           // turn off on startup
		setBrightnessLevel(m_modbusAdapter->getChannelBrightness(pwm));     // init with stored value
		m_modbusAdapter->setButtonState(m_pwm_id, m_button.isPressedRaw()); // init with current buttons state
	}
	Serial.println(F("button params [longpress speed doubleclick]"));
	Serial.println(ee_config->ee_button_longpress_delay);
	Serial.println(ee_config->ee_button_brightness_speed);
	Serial.println(ee_config->ee_button_double_click_delay);
	Serial.println(F("LedChannel init done"));
}

void LedChannel::check() {
	// check modbus request
	if (m_modbusAdapter && m_pwmController) {
		if (m_modbusAdapter->getChannelBrightness(m_pwm_id) != m_level) {
			setBrightnessLevel((uint8_t)(m_modbusAdapter->getChannelBrightness(m_pwm_id)));
			Serial.println(F("Modbus req->set setBrightnessLevel"));
		}
		if (m_modbusAdapter->getChannelState(m_pwm_id) != m_state) {
			m_pwmController->enable(m_pwm_id, m_modbusAdapter->getChannelState(m_pwm_id));
			Serial.println(F("Modbus req->set state"));
		}
		m_modbusAdapter->setButtonState(m_pwm_id, m_button.isPressedRaw());
	}

	// reset dimming direction after defined delay
	if (!m_channelDirectionUp) {
		if(millis() - m_directionResetCounter > (kDirectionResetDelayMs + m_buttonConfig.getRepeatPressDelay())
				&& m_button.getLastButtonState() == AceButton::kEventReleased ) {
			m_directionResetCounter = millis();
			m_channelDirectionUp = true;
			Serial.println(F("Reset longpress"));
		}
	}
	updateModbus();
	m_button.check();
}

void LedChannel::setBrightnessLevel(const uint8_t level) {
	m_level = level;
	if (m_pwmController) {
		m_pwmController->setDimmingLevel(m_pwm_id, m_level);
	}
}

uint8_t LedChannel::getBrightnessLevel() {
	return m_level;
}

void LedChannel::setModbusAdapter(ModbusAdapter* modbus_adapter) {
	m_modbusAdapter = modbus_adapter;
}

void LedChannel::setPWMControl(PwmController *pwm_control) {
	m_pwmController = pwm_control;
}

void LedChannel::handleEvent(AceButton* /*button*/, uint8_t eventType,
        uint8_t /*buttonState*/) {

	switch (eventType) {
	case AceButton::kEventClicked:
		Serial.println(F("Clicked for ch1:"));
		// short click - on/off channel
		if (m_pwmController) {
			m_pwmController->toggleEnable(m_pwm_id);
		}
		break;
	case AceButton::kEventRepeatPressed:
		Serial.println(F("RepeatPressed for ch1:"));
		if (!m_pwmController) {
			Serial.println(F("pwmController not init"));
			break;
		}
		if (m_pwmController->isEnabled(m_pwm_id)) {
			// increase or decrease brightness by longpress
//			Serial.println(m_pwmController->getDimmingLevel(m_pwm_id));
			if (m_channelDirectionUp) {
				if(m_pwmController->getDimmingLevel(m_pwm_id) != 100) { // MAX 100%
					m_pwmController->increaseBrightness(m_pwm_id);
				} else if (!m_channelMaxBlinkDone && !m_pwmController->isAnimationActive(m_pwm_id)){
					Serial.println(F("Blink"));
					m_pwmController->blink(m_pwm_id);
					m_channelMaxBlinkDone = true;
				}
			} else {
				m_pwmController->decreaseBrightness(m_pwm_id);
			}
		} else {
			Serial.println(F("Longpress from OFF"));
			// once channel in off state - start dimming from minimum brightness
			m_pwmController->enable(m_pwm_id, true);
			m_pwmController->setDimmingLevel(m_pwm_id, 1);
		}
		break;
	case AceButton::kEventReleased:
		Serial.println(F("Toggle direction"));
		// release after longpress - toggle direction for next longpress
		m_channelDirectionUp = !(m_channelDirectionUp);
		m_directionResetCounter = millis();
		m_channelMaxBlinkDone = false;
		break;
	}

}

void LedChannel::updateModbus() {
	if (m_pwmController) {
		m_level = m_pwmController->getDimmingLevel(m_pwm_id);
		m_state = m_pwmController->isEnabled(m_pwm_id);
	}

	if (m_modbusAdapter) {
		m_modbusAdapter->setChannelBrightness(m_pwm_id, m_level);
		m_modbusAdapter->setChannelState(m_pwm_id, m_state);
		m_modbusAdapter->setButtonState(m_pwm_id, m_button.isPressedRaw());
	}
}
