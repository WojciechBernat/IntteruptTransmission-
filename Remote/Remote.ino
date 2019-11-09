/* Main file of progrma */

/* Libraries */
#include "Functions.h"
#include <printf.h>   //Warning! Include to use printDetails()

/* Variables */
String txName = "TX Buffer";
String rxName = "RX Buffer";

/* Pipeline addresses */
uint8_t TxAddresses[PIPE_ADDRESS_SIZE] = {0x0A, 0x0A, 0x0A, 0x0A, 0x01};  //TX pipeline address
uint8_t RxAddresses[PIPE_ADDRESS_SIZE] = {0x0B, 0x0B, 0x0B, 0x0B, 0x02};  //RX pipeline address

/* Transmission buffers */
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxBuffer[BUFFER_SIZE];

RF24 Remote(CE_PIN, CSN_PIN);

void setup() {
  /* UART init */
  uartInit(UART_SPEED_115 ,  SERIAL_8E1);
  printf_begin();                                     //Added to use printDetails();

  /* nRF24L01+ init */
  Remote.begin();
  Remote.setPALevel(RF24_PA_MIN);
  Remote.setDataRate(RF24_250KBPS);
  Remote.setChannel(0x64);             //CH100
  
  Remote.setRetries(1, 4);   
  Remote.setAutoAck(true);     
  Remote.setCRCLength(RF24_CRC_8);
  Remote.enableAckPayload();
  
  Remote.openWritingPipe(TxAddresses);
  Remote.openReadingPipe(0, RxAddresses);

  Remote.printDetails();

  /* Tx and Rx Buffers reset */
  printBufferReset(TxBuffer, sizeof(TxBuffer), txName);
  printBufferReset(TxBuffer, sizeof(TxBuffer), rxName);

  /* ADC intterput init */
 adcInterruptSetup();

 Remote.stopListening();   //TX
}

void loop() {
  if(ToTxFlag == true) {
    bufferCopyMap(AdcVal, TxBuffer, 3);
    Remote.write(TxBuffer, TRANSMIT_DATA_SIZE);
    ToTxFlag = false;
  }
  else {
    if (Remote.isAckPayloadAvailable()) {
      Remote.read(RxBuffer, RX_ACK_PAYLOAD_SIZE);
      Serial.print("\n Received ACK payload: ");
      Serial.print(String(RxBuffer[0]) + "\t" + String(RxBuffer[1]) + "\t" + String(RxBuffer[2]) + "\n");
    }
    
// To Debug only    
    delay(1000);  
    Serial.println("\nTX Buffer[0] data: " + String(TxBuffer[0]));
    Serial.println("\nTX Buffer[1] data: " + String(TxBuffer[1]));
    Serial.println("\nTX Buffer[2] data: " + String(TxBuffer[2]));

  }
}
