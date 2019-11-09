/* Functions bodies */

/* Libraries */
#include "Functions.h"


#define BUFFER_SIZE 4

uint8_t adcPinX = 0x00;
uint8_t adcPinY = 0x01;
uint8_t adcPinS = 0x02;

bool ToTxFlag = false;
bool *pToTxFlag = &ToTxFlag;
volatile uint16_t AdcVal[3] = {0, 0 , 0};
volatile uint16_t *pADC[3] = {&AdcVal[0], &AdcVal[1], &AdcVal[2] };


void txISRFunction(void) {
  if ((ADMUX & 0x07) == 0x00 ) {
    AdcVal[0] = ADCL | (ADCH << 8);    //ADC measure on first channel
/*    Serial.println("\nAdcVal[0] data: " + String(ADCL | (ADCH << 8)));
/* To Debug 
    Serial.println("\n ADMUX reg: ");
    Serial.print(ADMUX, BIN);
    Serial.println("\n ADCSRA reg: ");
    Serial.print(ADCSRA, BIN); /*
*/   
    //Change channel
    ADCSRA &= B00101111;   //ADC disenable and stop conversion
    ADMUX  &= B11110000;   //reset channel bits
    ADMUX  |= adcPinY;     //Changed ADC channel
//    delayMicroseconds(100);
    ADCSRA |= B11000000;   //ADC enable
  }
  else if ((ADMUX & 0x07) == 0x01) {
    AdcVal[1] = ADCL | (ADCH << 8);    //ADC measure on first channel
/*    Serial.println("\nAdcVal[1] data: " + String(ADCL | (ADCH << 8)));
/* To Debug
    Serial.println("\n ADMUX reg: ");
    Serial.print(ADMUX, BIN);
    Serial.println("\n ADCSRA reg: ");
    Serial.print(ADCSRA, BIN);
*/    
    //Change channel
    ADCSRA &= B00101111;   //ADC disenable and stop conversion
    ADMUX  &= B11110000;   //reset channel bits
    ADMUX  |= adcPinS;     //Changed ADC channel
//    delayMicroseconds(100);
    ADCSRA |= B11000000;   //ADC enable
  }
  else {
    AdcVal[2] = ADCL | (ADCH << 8);    //ADC measure on first channel
/*    Serial.println("\nAdcVal[2] data: " + String(ADCL | (ADCH << 8)));
/* To Debug 
    Serial.println("\n ADMUX reg: ");
    Serial.print(ADMUX, BIN);
    Serial.println("\n ADCSRA reg: ");
    Serial.print(ADCSRA, BIN); /*
*/    
    //Change channel
    ADCSRA &= B00101111;   //ADC disenable and stop conversion
    ADMUX  &= B11110000;   //reset channel bits
    ADMUX  |= adcPinX;     //Changed ADC channel  - bitwise AND to reset to LSBs
//    delayMicroseconds(100);
    ADCSRA |= B11000000;   //ADC enable

    ToTxFlag = true;
  }
}

void adcInterruptSetup(void) {
  //All bit from 7 to 0
  //REFS1 REFS0 ADLAR [reserved] MUX3 MUX2 MUX1 MUX0
  //REFSx set reference voltage - 00 - AREF, 01 - AVCC, 11 - internal 2.56V or 1.1V
  //ADLAR adjuste for 10bit or 8bits
  ADMUX &= B11011111; //reset bits without 4th bit - it's reserved
  ADMUX |= B01000000; //set reference voltage - AVcc;
  ADMUX &= B11000000; //reset MUX bits also reset ADLAR bit - it should be 0
  ADMUX |= B00000000; //set ADC Channel - 0  (from  0)

  //ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
  ADCSRA |= B00000111 ; //set set prescaler division factor - 128
  ADCSRA |= B00001000;  //set ADC interrupt enable bit - if '1' interrupt enable
  ADCSRA |= B00100000;  //set auto-triggering enable - source of trigger is setting in ADCSRB

  // [reserved] ACME [reserved] [reserved] [reserved] ADTS2 ADTS1 ADTS0
  ADCSRB &= B00000000;  //reset ADCSRB also set ADC auto trigger source - 000 on MSBs
  sei();                  //set global interrupt enable - turn on interrupts
  ADCSRA |= B10000000;   //ADC enable
  ADCSRA |= B01000000;   //ADC start conversion
}


ISR(ADC_vect) {
  txISRFunction();
}

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
    for (int i = 0; i < 62; i += 2) {
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

void bufferCopyMap(volatile uint16_t *source, uint8_t *buf, uint8_t bufSize) {
  for (int i = 0; i < bufSize; i ++) {
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




//struct nRF24Settings {
//  rf24_pa_dbm_e PowerLevel;
//  rf24_datarate_e DataRete;
//  rf24_crclength_e CRCLength;
//  uint8_t Channel;
//  uint8_t retriesDelay;
//  uint8_t retriesCount;
//  bool    autoAckFlag;
//};
