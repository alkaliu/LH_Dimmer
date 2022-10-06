/*
 * PwmController.h
 *
 *  Created on: Jul 6, 2021
 *      Author: DShatokhin
 */

#ifndef PWMCONTROLLER_H_
#define PWMCONTROLLER_H_

#include <Arduino.h>
#include "PCA9685.h"
#include "EEConfig.h"

#define PWM_CH 8

class PwmController {
public:
	PwmController();

	void init(EEConfig* ee_config);
	void setDimmingLevel(uint8_t ch, uint8_t level);
	uint8_t getDimmingLevel(uint8_t ch) ;
	void enable(uint8_t ch, bool on);
	bool isEnabled(uint8_t ch);
	void toggleEnable(uint8_t ch);
	void decreaseBrightness(uint8_t ch);
	void increaseBrightness(uint8_t ch);
	void blink(uint8_t ch);
	void pollAnimation();
	bool isAnimationActive(uint8_t ch);

private:
	void decreasePWM(uint8_t ch, uint16_t endValue);
	void increasePWM(uint8_t ch, uint16_t endValue);
	void checkForChanges();
	void setRawPWM(uint8_t ch, uint16_t pwm);
	uint16_t scale(uint8_t ch, const uint8_t level);
private:
	PCA9685 pwmController;
	uint8_t dimmingStorage[PWM_CH];
	uint16_t dimmingRawPWMStorage[PWM_CH];
	uint16_t dimmingRawMaxPWMStorage[PWM_CH];
	uint16_t dimmingRawMinPWMStorage[PWM_CH];
	unsigned long animationCounter;
	uint16_t channelsAnimationActiveFlags;
	uint16_t channelsAnimationDirFlags;
	uint16_t channelsOnFlags;
	uint16_t channelsMaxBlinkAnimationFlags;
	uint8_t maxBlinkCounterStorage[PWM_CH];
};

#endif /* PWMCONTROLLER_H_ */
