/*
   AVR640 code for 31070 project
*/

#include "project.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "SPI.h"
#include "timers.h"
#include "CSchip.h"
#include "utils.h"
#include "eeprom.h"
#include "eepromMemoryMap.h"

volatile int newSerialData = 0;

void halt()
// halts execution (currently simply busy loop - could change to some low-power shutdown,
// but only occurs on rare errors (SD-card not found etc)
{
  printf("\r\n!!! EXECUTION STOPPED !!!");
  while (1) {}    
}

void setupAVR164()
{
  // Enable external int 7 (AVR164 asking for a timestamp)
  EICRB |= (1 << ISC70);
  EICRB &= ~(1 << ISC71);
  EIMSK |= (1 << INT7);

}


void ms_sleep(uint32_t ms)
{
  uint32_t stopTime;
  stopTime = getRTCcounter() + ms;

  // stopTime has overflowed, wait until current time overflows too:
  if (stopTime < ms)
  {
    do {}
    while (getRTCcounter() > stopTime);
  }

  do {}
  while (stopTime > getRTCcounter());
}

void sleep(uint8_t seconds)
{
  ms_sleep(seconds*1024);
}

// Uncomment this macro to disable reading calibration params from the EEPROM
// #define EEPROMread3Bytes(x) 0xFFFFFF

void setupCS()
{

  // initialize CS chip:
  CSwrite(0,0,0,4);  // set K=4 because input CLK is 16 MHz, all others default

  CSwrite(31,0,0,0); // goto page 0

  CSwrite(18,0,0,96); //Set IHPF, VHPF and IIR filters. (Mainly to remove DC-content, no need of DC-calibration). Disable AFC
  CSwrite(1,0,0,0); //Set current DC offset zero (should be set zero, when IHPF active)
  CSwrite(3,0,0,0); //Set voltage DC offset zero (should be set zero, when VHPF active)
  CSwrite24(17,0);    //Voltage AC Offset value
  CSwrite(5, 0, 0x0F, 0xA0); // n=4000


  // Values are read from the EEPROM, if not initialized, take a guess...
  // Some clever programmers make functions/macros out of this kind of repetative tasks....
  if (EEPROMread3Bytes(EEPROM_CS_CAL_CURRENT_AC_OFFSET) == 0xFFFFFF)
    CSwrite24(16,1000); //Current AC Offset value
  else
    CSwrite24(16,EEPROMread3Bytes(EEPROM_CS_CAL_CURRENT_AC_OFFSET));
  
  
  if (EEPROMread3Bytes(EEPROM_CS_CAL_VOLTAGE_GAIN) == 0xFFFFFF)
    CSwrite24(4,4019600); //Voltage Gain (Last Calibration)
  else
	CSwrite24(4,EEPROMread3Bytes(EEPROM_CS_CAL_VOLTAGE_GAIN));

  if(EEPROMread3Bytes(EEPROM_CS_CAL_ACTIVE_PWR_OFFSET) == 0xFFFFFF)
    CSwrite24(14,604); //Active Power Offset Calibration
  else
  	CSwrite24(14, EEPROMread3Bytes(EEPROM_CS_CAL_ACTIVE_PWR_OFFSET));

  if (EEPROMread3Bytes(EEPROM_CS_CAL_CURRENT_GAIN) == 0xFFFFFF)
    CSwrite24(2,4600000); //Current Gain
  else
    CSwrite24(2,EEPROMread3Bytes(EEPROM_CS_CAL_CURRENT_GAIN));

  printf("CS calibration data: Current Gain: %ld   Current AC offset: %ld    Voltage Gain: %ld    Power offset: %ld Epsilon:%ld\r\n", CSread(2), CSread(16), CSread(4), CSread(14), CSread(13));

  CScmd(0xE8);  // start continuous conversions 
}



void processSample()
{

	uint32_t I_rms = CSread(11);
	uint32_t V_rms = CSread(12);
	uint32_t P_Active = CSread(10);
	uint32_t H_active = CSread(20); //Reactive power //Harmonic active power
	uint32_t P_Reactive = CSread(25); //Power factor //Average reactive power
	long signed int Pact;
	
	// If the power measurement has gone negative (I don't know why it does this), convert back to positive value
	if (P_Active > 0x800000)
		Pact = 0x1000000 - P_Active;
	else 
		Pact = P_Active;
	
	// Prints in Decimal, with labels, somewhat human readable
	// printf("AT %08lu Vrms; %08lu Irms; %08lu P\r\n", V_rms, I_rms, P_Active);

	// Prints in HEX without labels
 	printf("AT 0x%08lx; 0x%08lx; 0x%08lx; 0x%08lx; 0x%08lx\r\n", V_rms, I_rms, Pact, H_active, P_Reactive); 
}


int main(void){
  DDRA = 0x83;
  PORTA = 0x83;
  SetupRTCtimer();
  SerSetupSTDOUT();
  CSchipInit();
  SPIinit();
 
  RELAY_PORT |= (1<<RELAY_PIN);
  STATUS_LED_PORT &= ~(1 << STATUS_LED_PIN);

  sei();      // ready to roll

  printf("\r\n\r\nFrmwareinitializing v1.0.0.0.3!!!!! \r\n");

  setupCS();

  for(;;) {
  	processSample();

	if (newSerialData)
	{
		printf("Ping!\r\n");
		newSerialData = 0;
	}

	sleep(1);
  }
}
