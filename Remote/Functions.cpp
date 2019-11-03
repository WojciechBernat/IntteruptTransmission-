/* Functions bodies */

/* Libraries */
#include <Arduino.h>
#include "Functions.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


struct defaultSettings {
  bool speedFlag;
  bool formatFlag;
  uint32_t uartSpeedDef;
  uint8_t  uartFormatDef;
} ;

void uartInit(uint32_t Speed, uint8_t Format) {
  struct defaultSettings defSet = {false, false, 115200, 38};
  struct defaultSettings *defSetPtr = &defSet;

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

uint32_t uartSpeedCheck(uint32_t Speed, struct defaultSettings *ptr) {
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

uint8_t uartFormatCheck(uint8_t Format, struct defaultSettings *ptr) {
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
