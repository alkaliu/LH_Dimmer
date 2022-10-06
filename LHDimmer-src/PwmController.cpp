/*
 * PwmController.cpp
 *
 *  Created on: Jul 6, 2021
 *      Author: DShatokhin
 */

#include "PwmController.h"

#define PWM_STEP 20
#define LEVEL_STEP 1
#define DIMMING_MAX_REQUEST 100
#define ANIM_FRAME_MS 10
#define MAX_BLINK_MS  400
#define PWM_MAX 4095
#define PWM_MIN 1
#define PWM_FREQ 500
#define BLINK_LOWER_BRIGHTNESS 60

const uint8_t logTransform[] PROGMEM =
{0x1  ,0x1  ,0x1  ,0x1  ,0x1  ,0x1  ,0x1  ,0x2  ,0x2  ,0x2  ,0x2  ,0x3  ,0x3  ,0x3  ,0x4  ,0x4  ,0x4  ,0x4  ,0x5  ,0x5
,0x5  ,0x6  ,0x6  ,0x6  ,0x7  ,0x7  ,0x7  ,0x8  ,0x8  ,0x8  ,0x9  ,0x9  ,0x9  ,0xA  ,0xA  ,0xA  ,0xB  ,0xB  ,0xB  ,0xC
,0xC  ,0xD  ,0xD  ,0xD  ,0xE  ,0xE  ,0xE  ,0xF  ,0xF  ,0x10 ,0x10 ,0x10 ,0x11 ,0x11 ,0x12 ,0x12 ,0x12 ,0x13 ,0x13 ,0x14
,0x14 ,0x15 ,0x15 ,0x16 ,0x16 ,0x16 ,0x17 ,0x17 ,0x18 ,0x18 ,0x19 ,0x19 ,0x1A ,0x1A ,0x1B ,0x1B ,0x1C ,0x1C ,0x1D ,0x1D
,0x1E ,0x1E ,0x1F ,0x1F ,0x20 ,0x21 ,0x21 ,0x22 ,0x22 ,0x23 ,0x23 ,0x24 ,0x25 ,0x25 ,0x26 ,0x26 ,0x27 ,0x28 ,0x28 ,0x29
,0x29 ,0x2A ,0x2B ,0x2B ,0x2C ,0x2D ,0x2D ,0x2E ,0x2F ,0x2F ,0x30 ,0x31 ,0x31 ,0x32 ,0x33 ,0x34 ,0x34 ,0x35 ,0x36 ,0x37
,0x37 ,0x38 ,0x39 ,0x3A ,0x3A ,0x3B ,0x3C ,0x3D ,0x3E ,0x3E ,0x3F ,0x40 ,0x41 ,0x42 ,0x43 ,0x43 ,0x44 ,0x45 ,0x46 ,0x47
,0x48 ,0x49 ,0x4A ,0x4B ,0x4C ,0x4D ,0x4D ,0x4E ,0x4F ,0x50 ,0x51 ,0x52 ,0x53 ,0x54 ,0x55 ,0x56 ,0x57 ,0x59 ,0x5A ,0x5B
,0x5C ,0x5D ,0x5E ,0x5F ,0x60 ,0x61 ,0x62 ,0x64 ,0x65 ,0x66 ,0x67 ,0x68 ,0x69 ,0x6B ,0x6C ,0x6D ,0x6E ,0x70 ,0x71 ,0x72
,0x73 ,0x75 ,0x76 ,0x77 ,0x79 ,0x7A ,0x7B ,0x7D ,0x7E ,0x80 ,0x81 ,0x82 ,0x84 ,0x85 ,0x87 ,0x88 ,0x8A ,0x8B ,0x8D ,0x8E
,0x90 ,0x91 ,0x93 ,0x95 ,0x96 ,0x98 ,0x9A ,0x9B ,0x9D ,0x9E ,0xA0 ,0xA2 ,0xA4 ,0xA5 ,0xA7 ,0xA9 ,0xAB ,0xAC ,0xAE ,0xB0
,0xB2 ,0xB4 ,0xB6 ,0xB8 ,0xBA ,0xBB ,0xBD ,0xBF ,0xC1 ,0xC3 ,0xC5 ,0xC7 ,0xCA ,0xCC ,0xCE ,0xD0 ,0xD2 ,0xD4 ,0xD6 ,0xD9
,0xDB ,0xDD ,0xDF ,0xE2 ,0xE4 ,0xE6 ,0xE8 ,0xEB ,0xED ,0xF0 ,0xF2 ,0xF4 ,0xF7 ,0xF9 ,0xFC ,0xFF};

//const uint8_t logTransform100[] PROGMEM =
//{   0x1   ,0x1    ,0x2   ,0x2   ,0x3   ,0x4   ,0x4   ,0x5   ,0x6   ,0x6
//   ,0x8   ,0x8    ,0x9   ,0xA   ,0xB   ,0xC   ,0xD   ,0xD   ,0xF   ,0x10
//   ,0x10  ,0x12   ,0x13  ,0x14  ,0x15  ,0x16  ,0x17  ,0x18  ,0x19  ,0x1B
//   ,0x1C  ,0x1D   ,0x1E  ,0x1F  ,0x22  ,0x23  ,0x24  ,0x26  ,0x28  ,0x29
//   ,0x2A  ,0x2B   ,0x2E  ,0x2F  ,0x31  ,0x34  ,0x35  ,0x37  ,0x38  ,0x3A
//   ,0x3D  ,0x3E   ,0x40  ,0x43  ,0x45  ,0x47  ,0x49  ,0x4B  ,0x4E  ,0x50
//   ,0x52  ,0x56   ,0x59  ,0x5B  ,0x5D  ,0x5F  ,0x64  ,0x66  ,0x68  ,0x6D
//   ,0x70  ,0x72   ,0x75  ,0x77  ,0x7D  ,0x80  ,0x82  ,0x88  ,0x8B  ,0x8E
//   ,0x91  ,0x95   ,0x9B  ,0x9E  ,0xA2  ,0xA9  ,0xAC  ,0xB0  ,0xB4  ,0xB8
//   ,0xBF  ,0xC3   ,0xD0  ,0xD4  ,0xD9  ,0xDD  ,0xE2  ,0xF0  ,0xF4  ,0xFF};


PwmController::PwmController() :
	 animationCounter(0)
	, channelsAnimationActiveFlags(0)
	, channelsAnimationDirFlags(0)
	, channelsOnFlags(0)
	, channelsMaxBlinkAnimationFlags(0) { }

void PwmController::init(EEConfig* ee_config) {
//	LOG("PwmController init enter");
    pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer

    for (uint8_t ch = 0; ch < PWM_CH; ++ch) {
    	 dimmingRawMaxPWMStorage[ch] = PWM_MAX;
    	 dimmingRawMinPWMStorage[ch] = PWM_MIN;
    }
    pwmController.setPWMFrequency(PWM_FREQ);

	if (ee_config) {
		pwmController.setPWMFrequency(ee_config->ee_pwm_freq);

		dimmingRawMaxPWMStorage[0] = ee_config->ee_channel_max_pwm_1;
		dimmingRawMaxPWMStorage[1] = ee_config->ee_channel_max_pwm_2;
		dimmingRawMaxPWMStorage[2] = ee_config->ee_channel_max_pwm_3;
		dimmingRawMaxPWMStorage[3] = ee_config->ee_channel_max_pwm_4;
		dimmingRawMaxPWMStorage[4] = ee_config->ee_channel_max_pwm_5;
		dimmingRawMaxPWMStorage[5] = ee_config->ee_channel_max_pwm_6;
		dimmingRawMaxPWMStorage[6] = ee_config->ee_channel_max_pwm_7;
		dimmingRawMaxPWMStorage[7] = ee_config->ee_channel_max_pwm_8;

		dimmingRawMinPWMStorage[0] = ee_config->ee_channel_min_pwm_1;
		dimmingRawMinPWMStorage[1] = ee_config->ee_channel_min_pwm_2;
		dimmingRawMinPWMStorage[2] = ee_config->ee_channel_min_pwm_3;
		dimmingRawMinPWMStorage[3] = ee_config->ee_channel_min_pwm_4;
		dimmingRawMinPWMStorage[4] = ee_config->ee_channel_min_pwm_5;
		dimmingRawMinPWMStorage[5] = ee_config->ee_channel_min_pwm_6;
		dimmingRawMinPWMStorage[6] = ee_config->ee_channel_min_pwm_7;
		dimmingRawMinPWMStorage[7] = ee_config->ee_channel_min_pwm_8;
	}

    for (uint8_t ch = 0; ch < PWM_CH; ++ch) {
    	setRawPWM(ch, dimmingRawMinPWMStorage[ch]);
    	pwmController.setChannelOff(ch);
    }

    delay(500);
}

void PwmController::setDimmingLevel(uint8_t ch, uint8_t level) {
	if (ch > PWM_CH) {
		return;
	}

	if(level > DIMMING_MAX_REQUEST) {
		level = DIMMING_MAX_REQUEST;
	}

	dimmingStorage[ch] = level;
	Serial.print(F("setDimmingLevel: "));
	Serial.println(dimmingStorage[ch]);
//	LOG("PwmController setDimmingLevel %d", dimmingStorage[ch]);
}

uint8_t PwmController::getDimmingLevel(uint8_t ch) {
	if (ch > PWM_CH) {
		return 0;
	}
	return dimmingStorage[ch];
}

void PwmController::enable(uint8_t ch, bool on) {
	if (ch > PWM_CH) {
		return;
	}
    bitWrite(channelsOnFlags, ch, on);
}

bool PwmController::isEnabled(uint8_t ch) {
	if (ch > PWM_CH) {
		return false;
	}
    return bitRead(channelsOnFlags, ch);
}

void PwmController::decreaseBrightness(uint8_t ch) {
//	LOG("decreaseBrightness:%d",ch);
	if (ch > PWM_CH) {
		return;
	}
	if(dimmingStorage[ch] > LEVEL_STEP) {
		setDimmingLevel(ch, (dimmingStorage[ch] - LEVEL_STEP));
	} else {
		setDimmingLevel(ch, 0);
	}
}

void PwmController::increaseBrightness(uint8_t ch) {
	//LOG("increaseBrightness:%x current:%x",ch, dimmingStorage_0);
	if (ch > PWM_CH) {
		return;
	}

	if((DIMMING_MAX_REQUEST - dimmingStorage[ch]) > LEVEL_STEP ) {
		setDimmingLevel(ch, (dimmingStorage[ch] + LEVEL_STEP));
	} else {
		setDimmingLevel(ch, DIMMING_MAX_REQUEST);
	}
}

void PwmController::blink(uint8_t ch) {
	// blink
//	LOG("blink");
	pwmController.setChannelPWM(ch,scale(ch, BLINK_LOWER_BRIGHTNESS));
	bitSet(channelsMaxBlinkAnimationFlags, ch);
	maxBlinkCounterStorage[ch] = MAX_BLINK_MS / ANIM_FRAME_MS;
}

void PwmController::toggleEnable(uint8_t ch) {
	bitToggle(channelsOnFlags, ch);
//	LOG("toggleEnable:%d, new state:%x", ch, bitRead(channelsOnFlags,ch));
}

void PwmController::pollAnimation() {
	checkForChanges();
	bool new_frame_tick = false;
	if(millis() - animationCounter > ANIM_FRAME_MS) {
		animationCounter = millis();
		new_frame_tick = true;
	}
	if (channelsAnimationActiveFlags && new_frame_tick) {
		for (uint8_t ch = 0; ch < PWM_CH; ++ch) {
			if (bitRead(channelsAnimationActiveFlags,ch)) {
				uint16_t endValue = scale(ch, dimmingStorage[ch]) * bitRead(channelsOnFlags,ch);
				bitRead(channelsAnimationDirFlags, ch) ? increasePWM(ch, endValue) :
						decreasePWM(ch, endValue);

			}
		}
	}
	if (channelsMaxBlinkAnimationFlags && new_frame_tick) {
		for (uint8_t ch = 0; ch < PWM_CH; ++ch) {
			if (maxBlinkCounterStorage[ch] > 0) {
				maxBlinkCounterStorage[ch]--;
			}
			if (bitRead(channelsMaxBlinkAnimationFlags,ch) && maxBlinkCounterStorage[ch] <= 0) {
				Serial.println(F("channelsMaxBlinkAnimation finish for ch"));
				setRawPWM(ch,dimmingRawMaxPWMStorage[ch]);
				bitClear(channelsMaxBlinkAnimationFlags, ch);
			}
		}
	}
}

bool PwmController::isAnimationActive(uint8_t ch)
{
	return bitRead(channelsAnimationActiveFlags,ch);
}

void PwmController::checkForChanges() {

	channelsAnimationDirFlags = 0;
	channelsAnimationActiveFlags = 0;

	for (uint8_t ch = 0; ch < PWM_CH; ++ch) {
		uint16_t endValue = scale(ch, dimmingStorage[ch]) * bitRead(channelsOnFlags,ch);
		//LOG("checkForChanges(): %d: endValue:%d, current:%d", ch, endValue, dimmingRawPWMStorage[ch]);
		if (endValue != dimmingRawPWMStorage[ch]) {
			bitSet(channelsAnimationActiveFlags, ch);
			bitWrite(channelsAnimationDirFlags, ch, endValue > dimmingRawPWMStorage[ch] );
		}
		// clear MaxBlinkAnimation once any other animation active for channel
//		if (bitRead(channelsMaxBlinkAnimationFlags,ch) && bitRead(channelsAnimationActiveFlags,ch)) {
//			setRawPWM(ch,scale(UINT8_MAX));
//			bitClear(channelsMaxBlinkAnimationFlags, ch);
//			maxBlinkCounterStorage[ch] = 0;
//		}
	}

}

void PwmController::setRawPWM(uint8_t ch, uint16_t pwm) {
//	LOG("setRawPWM, %d:%d", ch, pwm);
	dimmingRawPWMStorage[ch] = pwm;
	pwmController.setChannelPWM(ch, pwm);
}

void PwmController::decreasePWM(uint8_t ch, uint16_t endValue) {
//	LOG("decreasePWM, %d:%d", ch, endValue);
	if (ch > PWM_CH) {
		return;
	}

	uint16_t pwmChVal = dimmingRawPWMStorage[ch];
	if(pwmChVal > PWM_STEP) {
		pwmChVal -= PWM_STEP;
	} else {
		pwmChVal = endValue;
	}
	setRawPWM(ch, pwmChVal);
}

void PwmController::increasePWM(uint8_t ch, uint16_t endValue) {
//	LOG("increasePWM, %d:%d", ch, endValue);
	if (ch > PWM_CH) {
		return;
	}

	uint16_t pwmChVal = dimmingRawPWMStorage[ch];
	if(pwmChVal + PWM_STEP <= endValue) {
		pwmChVal += PWM_STEP;
	} else {
		pwmChVal = endValue;
	}

	setRawPWM(ch, pwmChVal);
}

/**
 * channel - channel number
 * level   - percent value of brightness
 *
 * return - scaled to 12 bit (0..4095) pwm value using log function
 */
uint16_t PwmController::scale(uint8_t channel, const uint8_t level) {
	if (channel > PWM_CH) {
		return 0;
	}
	uint8_t bright_level = level;
	if (level > 100) {
		bright_level = 100;
	}
	uint16_t percent_step = (dimmingRawMaxPWMStorage[channel] - dimmingRawMinPWMStorage[channel])/100 + 1;
	uint16_t scaled_linear = dimmingRawMinPWMStorage[channel] + bright_level * percent_step;
	if(scaled_linear > (2<<11) - 1) {
		scaled_linear = (2<<11) - 1;
	}
	uint16_t log_value_8bit = pgm_read_byte(&logTransform[scaled_linear>>4]);
	uint16_t scaled = (log_value_8bit << 4) | 0xF;

	if (scaled > dimmingRawMaxPWMStorage[channel]) {
		scaled = dimmingRawMaxPWMStorage[channel];
	}

	return scaled;
}
