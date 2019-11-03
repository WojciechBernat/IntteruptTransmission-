/* File with functions prototypes */

/* Libraries */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

extern bool ToTxFlag;
extern bool *pToTxFlag;
extern uint16_t AdcVal[3];
extern uint16_t *pADC[3];

void uartInit(uint32_t Speed = 115200, uint8_t Format = SERIAL_8E1);
uint32_t uartSpeedCheck(uint32_t Speed, struct defaultUartSettings *ptr);
uint8_t  uartFormatCheck(uint8_t Format, struct defaultUartSettings *ptr);

void printBufferReset(uint8_t *buf, uint8_t bufSize, String bufName);
void resetBuffer(uint8_t *buf, uint8_t bufSize);
void bufferCopyMap(uint16_t *source, uint8_t *buf, uint8_t bufSize);

void adcInterruptSetup(void);
