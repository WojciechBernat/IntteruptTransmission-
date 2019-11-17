/* Functions bodies */

/* Libraries and include files */
#include "Functions.h"


void tempStructDefSetup() {
  tempStruct.tempChannel = TEMPERATURE_SENSOR_CHANNEL ; 
  tempStruct.tempCounter = 0;
  tempStruct.tempMean = 0;
  //resetBuffer(tempStruct.tempSens, sizeof(tempStruct.tempSens));
}


void adcInterruptSetup(uint8_t Channel) {
  //All bit from 7 to 0
  //REFS1 REFS0 ADLAR [reserved] MUX3 MUX2 MUX1 MUX0
  //REFSx set reference voltage - 00 - AREF, 01 - AVCC, 11 - internal 2.56V or 1.1V
  //ADLAR adjuste for 10bit or 8bits
  ADMUX &= B11011111; //reset bits without 4th bit - it's reserved
  ADMUX |= B01000000; //set reference voltage - AVcc;
  ADMUX &= B11000000; //reset MUX bits also reset ADLAR bit - it should be 0
  ADMUX |= Channel; //set ADC Channel - 0  (from  0)

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

void tempMeasure(uint8_t volatile *tab, uint8_t volatile counter, uint8_t volatile meanTemp ) {
  tab[counter] = map((ADCL | (ADCH << 8)), 0, 1023, 0, 255);
  if (counter == 0x08) {
    counter = 0;
    meanTemp = meanVal(tab, sizeof(tab));
  }
  else {
    counter++;
  }
}

uint8_t meanVal(uint8_t volatile *tab, uint8_t tabSize) {
  if (tabSize > 0) {
    uint8_t meanVal = 0;
    for (uint8_t i = 0; i < tabSize; i++) {
      meanVal += tab[i];
    }
    meanVal /= tabSize;
    return meanVal;
  }
  else {
    return 0;
  }
}

void rxISRFunction()  {
  tempMeasure(tempStruct.tempSens, tempStruct.tempCounter, tempStruct.tempMean);  //temperature measure
  TxBuffer[0] = tempStruct.tempMean;
}

ISR(ADC_vect) {
  rxISRFunction();
}

void setAdcChannel(uint8_t AdcChannel) {
  if (AdcChannel >= 0 || AdcChannel <= 8 ) {
    ADCSRA &= B00101111;   //ADC disenable and stop conversion
    ADMUX  &= B11110000;   //reset channel bits
    ADMUX  |= AdcChannel;     //Changed ADC channel
  }
  else {
    ADCSRA &= B00101111;   //ADC disenable and stop conversion
    ADMUX  &= B11110000;   //reset channel bits
    ADMUX  |= TEMPERATURE_SENSOR_CHANNEL;     //Changed ADC channel
  }
  ADCSRA |= B11000000;   //ADC enable
}



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
