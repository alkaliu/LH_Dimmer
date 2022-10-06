// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _LHDimmer_H_
#define _LHDimmer_H_

#include "Arduino.h"

#include "EEConfig.h"

#include "ConfigHW.h"

#include <Modbus.h>
#include <ModbusSerial.h>
#include <AceButton.h>
#include <EEPROM.h>
#include <DS18B20.h>

#include "PwmController.h"
#include "LedChannel.h"
#include "ModbusAdapter.h"
#include <MemoryUsage.h>


void setupModbus(uint16_t speed, uint8_t slave_id, uint8_t data_bytes, uint8_t stop_bits, uint8_t parity);
EEConfig loadConfig();
void setupAdc();
uint16_t readVcc();
void checkReset();
void checkResetButton();
void updateConfig();
void writeDefaultConfig();
void reset_sw();
void disableWdt();

#endif /* _LHDimmer_H_ */
