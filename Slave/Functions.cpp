/* Functions bodies */

/* Libraries */
#include "Functions.h"

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BUFFER_SIZE 4

uint8_t adcPinX = 0x0;
uint8_t adcPinY = 0x01;
uint8_t adcPinS = 0x02;

bool ToTxFlag = false;
bool *pToTxFlag = &ToTxFlag;
uint16_t AdcVal[3];
uint16_t *pADC[3] = {&AdcVal[0], &AdcVal[1], &AdcVal[2] };

//struct nRF24Settings {
//  rf24_pa_dbm_e PowerLevel;
//  rf24_datarate_e DataRete;
//  rf24_crclength_e CRCLength;
//  uint8_t Channel;
//  uint8_t retriesDelay;
//  uint8_t retriesCount;
//  bool    autoAckFlag;
//};



struct defaultUartSettings {
  bool speedFlag;
  bool formatFlag;
  uint32_t uartSpeedDef;
  uint8_t  uartFormatDef;
} ;

void uartInit(uint32_t Speed, uint8_t Format) {
  struct defaultUartSettings defSet = {false, false, 115200, 38};
  struct defaultUartSettings *defSetPtr = &defSet;

  Speed = uartSpeedCheck(Speed, defSetPtr);
  Format = uartFormatCheck(Format, defSetPtr);

  Serial.begin(Speed, Format );                                    //UART 8 bits with EVEN mode - że bit parzystości

  Serial.println("\nUART init OK");   //
  if ((defSetPtr) -> speedFlag) {
    Serial.println("\nCorrect speed value \nSet speed: " + String(Speed));
  }
  else {
    Serial.println("\nInorrect speed value \nSet default speed: " + String(Speed));
    defSetPtr -> speedFlag = true;
  }

  if (defSetPtr -> formatFlag) {
    Serial.println("\nCorrect format \nSet frame format: " + String(Format));
  }
  else {
    Serial.println("\nIncorrect format \nSet default frame format: " + String(Format));
    defSetPtr -> formatFlag = true;
  }
}

uint32_t uartSpeedCheck(uint32_t Speed, struct defaultUartSettings *ptr) {
  switch (Speed) {
    case 2500:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 4800:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 9600:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 14400:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 19200:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 28800:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 38400:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 57600:
      ptr -> speedFlag = true;
      return Speed;
      break;
    case 115200:
      ptr -> speedFlag = true;
      return Speed;
      break;
    default:
      //  Serial.print("\nUART speed incorrect. \nSet default speed: 115200");
      ptr -> speedFlag = false;
      return 115200;
      break;
  }
  //

}

uint8_t uartFormatCheck(uint8_t Format, struct defaultUartSettings *ptr) {
  if (Format % 2) {
    //    Serial.print("Incorrect frame format\n Set deafult: " + String(SERIAL_8E1));
    ptr -> formatFlag = false;
    return SERIAL_8E1;
  }
  else {
    for (int i = 0; i <= 62; i += 2) {
      if (i == 16) {
        i = 32;
      }
      if (Format == i) {
        //        Serial.print("\nSet frame format: " + String(Format));
        ptr -> formatFlag = true;
        return Format;
      }
      if (i == 62 ) {
        //        Serial.print("Incorrect frame format\n Set deafult: " + String(SERIAL_8E1));
        ptr -> formatFlag = false;
        return SERIAL_8E1;
      }
    }
  }
}

void bufferCopyMap(uint16_t *source, uint8_t *buf, uint8_t bufSize) {
  for(int i = 0; i < bufSize; i ++) {
    buf[i] = map(source[i], 0 , 1023, 0, 255);
  }
}

void resetBuffer(uint8_t *buf, uint8_t bufSize) {
  for (int i = 0; i < bufSize; i++) {
    buf[i] = 0;
  }
}

void printBufferReset(uint8_t *buf, uint8_t bufSize, String bufName) {
  if (bufSize <= 0) {
    Serial.print("\nBuffer size error");
    bufSize = BUFFER_SIZE;
  }
  else {
    resetBuffer(buf, bufSize);
  }
  Serial.print("\nBuffer " + bufName + " reset correct");
}


void adcInterruptSetup(void) {
  ADMUX &= B11011111;     //ADMUX reset
  ADMUX |= B01000000;     //Vref choose
  ADMUX &= B11110000;     //Reset lower bits
  ADMUX |= adcPinX;       //Set ADC MUX channel - CH0

  ADCSRA |= B10000000;    //ADC enable
  ADCSRA |= B00100000;    //Enable auto triggering of ADC

  ADCSRB &= B01000000;    //Set free running mode

  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);  //set prescaler 128
  ADCSRA |= B0001000;     //Enable ADC interrupt

  sei();      //set global interrupts flag

  ADCSRA |= B01000000; //start conversion
}

ISR(ADC_vect) {
  if ((ADMUX & 0x07) == 0x00 ) {
    *pADC[0] = ADCL | (ADCH << 8);    //ADC measure on first channel
    ADMUX |= adcPinY;
  }
  else if ((ADMUX & 0x07) == 0x01) {
    *pADC[1] = ADCL | (ADCH << 8);    //ADC measure on first channel
    ADMUX |= adcPinS;
  }
  else {
    *pADC[2] = ADCL | (ADCH << 8);    //ADC measure on first channel
    ADMUX |= adcPinX;
    ToTxFlag = true;
  }

}
