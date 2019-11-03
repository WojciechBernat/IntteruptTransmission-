/* Functions bodies */

/* Libraries */
#include "Functions.h"


//uint8_t adcPinX = 0x00;    //TX
//uint8_t adcPinY = 0x01;
//uint8_t adcPinS = 0x02;

uint8_t adcPin = 0x00;      //RX

//bool ToTxFlag = false;        //TX
//bool *pToTxFlag = &ToTxFlag;
//uint16_t AdcVal[3];
//uint16_t *pADC[3] = {&AdcVal[0], &AdcVal[1], &AdcVal[2] };

uint16_t volatile AdcVal = 0;    //RX
uint16_t volatile *pAdcVal = &AdcVal;

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


void adcInterruptSetup(void) {
  ADMUX &= B11011111;     //ADMUX reset
  ADMUX |= B01000000;     //Vref choose
  ADMUX &= B11110000;     //Reset lower bits
  //  ADMUX |= adcPinX;       //Set ADC MUX channel - CH0   //TX
  ADMUX |= adcPin;       //Set ADC MUX channel - CH0    //RX

  ADCSRA |= B10000000;    //ADC enable
  ADCSRA |= B00100000;    //Enable auto triggering of ADC

  ADCSRB &= B01000000;    //Set free running mode

  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);  //set prescaler 128
  ADCSRA |= B0001000;     //Enable ADC interrupt

  sei();      //set global interrupts flag

  ADCSRA |= B01000000; //start conversion
}

void rxISRFunction()  {
  AdcVal = map((ADCL | (ADCH << 8)), 0, 1023, 0, 255);
  TxBuffer[0] = AdcVal;
}

ISR(ADC_vect) {
  rxISRFunction();
}
