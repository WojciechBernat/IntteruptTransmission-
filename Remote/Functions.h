/* File with functions prototypes */

/* Libraries */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


void uartInit(uint32_t Speed = 115200, uint8_t Format = SERIAL_8E1);
uint32_t uartSpeedCheck(uint32_t Speed, struct defaultSettings *ptr);
uint8_t  uartFormatCheck(uint8_t Format, struct defaultSettings *ptr);

void printBufferReset(uint8_t *buf, uint8_t bufSize, String bufName);
void resetBuffer(uint8_t *buf, uint8_t bufSize);
