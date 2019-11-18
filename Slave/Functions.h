/* File with functions prototypes */

/* Libraries and include files */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "Structures.h"

/* Functions prototypes */
void uartInit(uint32_t Speed = 115200, uint8_t Format = SERIAL_8E1);
uint32_t uartSpeedCheck(uint32_t Speed, struct defaultUartSettings *ptr);
uint8_t  uartFormatCheck(uint8_t Format, struct defaultUartSettings *ptr);

void printBufferReset(uint8_t *buf, uint8_t bufSize, String bufName);
void resetBuffer(uint8_t *buf, uint8_t bufSize);
void bufferCopyMap(uint16_t *source, uint8_t *buf, uint8_t bufSize);

void adcInterruptSetup(uint8_t Channel);
void rxISRFunction(void);
void tempMeasure(uint8_t volatile *tab, uint8_t volatile counter, uint8_t volatile meanTemp);
void setAdcChannel(uint8_t AdcChannel);

uint8_t meanVal(uint8_t volatile *tab, uint8_t tabSize);

void tempStructDefSetup();    



/* Functions to handling RF structure */
void setTransmitStruct(uint8_t retDelay, uint8_t retDelayVar, uint8_t ret, uint8_t retVar, boolean AAckState, boolean AAckStateVar, rf24_crclength_e crc, rf24_crclength_e crcVar);
void setRFStruct(rf24_pa_dbm_e powerLevel, rf24_pa_dbm_e powerVar, rf24_datarate_e dataRate, rf24_datarate_e dataRateVar, uint8_t channel, uint8_t channelVar );
