
#include "LHDimmer.h"

#include <avr/wdt.h>

const static EEConfig default_config PROGMEM = {
		0,
		96, 0, 1,                                        // 96 - 9600, 0 - No parity, 1 - 1 stop bit
		55,                                              // Modbus address
		500,                                             // pwm freq
		10, 10, 10, 10, 10, 10, 10, 10,                   // current brightness in percents
		4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,   // pwm max level
		1, 1, 1, 1, 1, 1, 1, 1,                           // pwm min
		1300, 60, 300,                                    // longpress delay, repeat interval, double click delay
		0};
const static uint8_t CHANNEL_PINS[] PROGMEM = {
		CHANNEL_1_PIN,
		CHANNEL_2_PIN,
		CHANNEL_3_PIN,
		CHANNEL_4_PIN,
		CHANNEL_5_PIN,
		CHANNEL_6_PIN,
		CHANNEL_7_PIN,
		CHANNEL_8_PIN};

ModbusSerial mb;
ModbusAdapter mb_adapter;
LedChannel channels[8];
PwmController pwm_controller;
DS18B20 ds(10);
unsigned long tempus_1sec;
unsigned long tempus_60sec;

void setup()
{
    Serial.begin(SERIAL_DEBUG_SPEED);  // Begin Serial and Wire interfaces
//    while (! Serial);      // Wait until Serial is ready
    delay(100);
    Serial.println(F("setup(): begin"));

	pinMode(RESET_PIN, INPUT_PULLUP);
	checkResetButton();

    Wire.begin();
    setupAdc();
    while(ds.selectNext());

    delay(1000);
    EEConfig device_config = loadConfig();
    setupModbus(device_config.ee_rs485_speed*100,
    		    (uint8_t)device_config.ee_rs485_address,
				8,
				(uint8_t)device_config.ee_rs485_stopbit,
				(uint8_t)device_config.ee_rs485_parity);
    mb_adapter.init(&mb, &device_config);
    pwm_controller.init(&device_config);

	for (int i = 0; i < CHANNELS_USED; ++i) {
		channels[i].setModbusAdapter(&mb_adapter);
		channels[i].setPWMControl(&pwm_controller);
		channels[i].init(pgm_read_byte(&CHANNEL_PINS[i]), i, &device_config);
	}

    pinMode(LED_BUILTIN_RX, OUTPUT);
    pinMode(LED_BUILTIN_TX, OUTPUT);
    digitalWrite(LED_BUILTIN_RX, HIGH);
    digitalWrite(LED_BUILTIN_TX, HIGH);

//    SRamDisplay();
}

// The loop function is called in an endless loop
void loop()
{
	mb.task();
	for (int var = 0; var < CHANNELS_USED; ++var) {
		channels[var].check();
	}
	pwm_controller.pollAnimation();

	if(millis() - tempus_1sec > 1000) {
		digitalWrite(LED_BUILTIN_RX, !digitalRead(LED_BUILTIN_RX));
		tempus_1sec = millis();
		uint32_t uptime = (uint32_t(mb_adapter.getDEVICE_UPTIME_H()) << 16) | mb_adapter.getDEVICE_UPTIME_L();
		++uptime;
		mb_adapter.setDEVICE_UPTIME_L((uint16_t)uptime);
		mb_adapter.setDEVICE_UPTIME_H((uint16_t)(uptime>>16));
		checkReset();
	}

	if(millis() - tempus_60sec > 60000L) {
		updateConfig();

		uint16_t vcc = readVcc();
		mb_adapter.setCPU_VOLTAGE(vcc);
		int16_t signed_temp = ds.getTempC()*10;
		uint16_t regData;
		memcpy(&regData, &signed_temp, sizeof(uint16_t));
		mb_adapter.setDEVICE_TEMP(regData);

		tempus_60sec = millis();
	}
}

EEConfig loadConfig()
{
	Serial.println(F("Read EEPROM"));
	EEConfig temp;
	EEPROM.get(EEPROM_OFFSET, temp);

	if(temp.magic == EEPROM_MAGIC_FLAG
	   && temp.magic_end == EEPROM_MAGIC_FLAG_END) {
		Serial.println(F("EEPROM Read OK!"));
	} else {
		Serial.println(F("EEPROM not initialized!"));
		writeDefaultConfig();
		EEPROM.get(EEPROM_OFFSET, temp);
	}
	return temp;
}

void setupModbus(uint16_t speed, uint8_t slave_id, uint8_t data_bits, uint8_t stop_bits, uint8_t parity) {
    // Config Modbus Serial default params
	byte mode = SERIAL_8N1;
	switch (parity) {
		case 1: // Odd
			switch (stop_bits) {
				case 2:
					switch (data_bits) {
						case 5: mode = SERIAL_5O2; break;
						case 6: mode = SERIAL_6O2; break;
						case 7: mode = SERIAL_7O2; break;
						default:
						case 8: mode = SERIAL_8O2; break;
					}
					break;
				default:
				case 1:
					switch (data_bits) {
						case 5: mode = SERIAL_5O1; break;
						case 6: mode = SERIAL_6O1; break;
						case 7: mode = SERIAL_7O1; break;
						default:
						case 8: mode = SERIAL_8O1; break;
					}
					break;
			}
		break;
		case 2: // Even
			switch (stop_bits) {
				case 2:
					switch (data_bits) {
						case 5: mode = SERIAL_5E2; break;
						case 6: mode = SERIAL_6E2; break;
						case 7: mode = SERIAL_7E2; break;
						default:
						case 8: mode = SERIAL_8E2; break;
					}
					break;
				default:
				case 1:
					switch (data_bits) {
						case 5: mode = SERIAL_5E1; break;
						case 6: mode = SERIAL_6E1; break;
						case 7: mode = SERIAL_7E1; break;
						default:
						case 8: mode = SERIAL_8E1; break;
					}
					break;
			}
		break;
		default:
		case 0: // N
			switch (stop_bits) {
				case 2:
					switch (data_bits) {
						case 5: mode = SERIAL_5N2; break;
						case 6: mode = SERIAL_6N2; break;
						case 7: mode = SERIAL_7N2; break;
						default:
						case 8: mode = SERIAL_8N2; break;
					}
					break;
				default:
				case 1:
					switch (data_bits) {
						case 5: mode = SERIAL_5N1; break;
						case 6: mode = SERIAL_6N1; break;
						case 7: mode = SERIAL_7N1; break;
						default:
						case 8: mode = SERIAL_8N1; break;
					}
					break;
			}
		break;
	}

    mb.config(&Serial1, speed, mode);
    mb.setSlaveId(slave_id);
    mb.addHreg(50, 0x5050);
    Serial.println(F("setupModbus interface Done"));
}

void setupAdc() {
  // Set ADMUX to read 1.1V Bandgap reference against AVcc which
  // is tied to VCC on the Micro. Normally we should wait for Vref
  // to  settle. It only takes about 4 microseconds but it's a good
  // practice to wait 1 millisecond when switching AREF. We don't
  // need to specify settling time in this case because we'll use
  // that time doing other things. Also, since we're not measuring
  // a  pin, there's no need to use the DIDR register.
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADCSRB = 0;
  Serial.println(F("setupAdc Done"));
}

uint16_t readVcc() {
	uint16_t result;

  // Trigger a conversion. The ADSC bit will return to LOW after
  // conversion is complete.
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));

  // The result is stored in the combination of the ADCH and ADCL
  // registers. They can be accessed together using ADCW. ADCW is
  // an  unsigned int. The lowest value read as 0, and the highest
  // value  read is 2n-1 (1023). We must calculate the actual value
  // read.
  result = (1100L * 1023L) / ADCW;
  return result;
}

void checkReset()
{
	uint16_t reboot = mb_adapter.getDEVICE_REBOOT();
	if (reboot)
	{
		mb_adapter.setDEVICE_REBOOT(0);
		if (reboot == 0x000F) {
			Serial.println(F("DEVICE reset and reboot request"));
			writeDefaultConfig();
		} else if (reboot == 0x00FF){
			Serial.println(F("DEVICE save and reboot request"));
			updateConfig();
		}
		Serial.println(F("DEVICE reboot request"));
		Serial.println(F("Reboot in 3 sec"));
		delay(3000);
		reset_sw();
	}
}

void checkResetButton()
{
    delay(40);
	if(digitalRead(RESET_PIN) == RESET_PIN_ACTIVESTATE) {
		delay(40); // debounce
		if(digitalRead(RESET_PIN) == RESET_PIN_ACTIVESTATE) {
			Serial.println(F("Reset active!"));
			writeDefaultConfig();
		}
	}
}
void writeDefaultConfig()
{
	EEConfig temp;
	memcpy_P(&temp, &default_config, sizeof(temp));
	temp.magic = EEPROM_MAGIC_FLAG;
	temp.magic_end = EEPROM_MAGIC_FLAG_END;
	EEPROM.put(EEPROM_OFFSET, temp);
	Serial.println(F("EEPROM write default done!"));
}

void updateConfig()
{
	EEConfig current_config = loadConfig();
	mb_adapter.getEEConfig(&current_config);
	EEPROM.put(EEPROM_OFFSET, current_config);
	Serial.println(F("EEPROM update done!"));
}

void reset_sw()
{
	while(true)
	{
		wdt_enable(WDTO_15MS);
		for(;;) {};
	}
}

// disable software reset after successful reset
void disableWdt() __attribute__((naked)) __attribute__((section(".init3"))) ;
void disableWdt()
{
	MCUSR = 0 ;
	wdt_disable();
}
