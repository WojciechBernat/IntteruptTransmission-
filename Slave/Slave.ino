  /* Main file of progrma */

/* Libraries */
#include "Functions.h"
#include <printf.h>   //Warning! Include to use printDetails()

/* Variables */
String txName = "TX Buffer";
String rxName = "RX Buffer";

/* Pipeline addresses */
uint8_t TxAddresses[PIPE_ADDRESS_SIZE] = {0x0B, 0x0B, 0x0B, 0x0B, 0x02};  //TX pipeline address
uint8_t RxAddresses[PIPE_ADDRESS_SIZE] = {0x0A, 0x0A, 0x0A, 0x0A, 0x01};  //RX pipeline address

/* Transmission buffers */
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxBuffer[BUFFER_SIZE];

RF24 Slave(CE_PIN, CSN_PIN);

void setup() {
  /* UART init */
  uartInit(UART_SPEED_115 ,  SERIAL_8E1);
  printf_begin();                                     //Added to use printDetails();

  /* nRF24L01+ init */
  Slave.begin();
  Slave.setPALevel(RF24_PA_MIN);
  Slave.setDataRate(RF24_250KBPS);
  Slave.setChannel(0x64);             //CH100
  
  Slave.setRetries(1, 4);   
  Slave.setAutoAck(true);     
  Slave.setCRCLength(RF24_CRC_8);
  Slave.enableAckPayload();

  Slave.openWritingPipe(TxAddresses);
  Slave.openReadingPipe(0, RxAddresses);

  Slave.printDetails();

  /* Tx and Rx Buffers reset */
  printBufferReset(TxBuffer, sizeof(TxBuffer), txName);
  printBufferReset(TxBuffer, sizeof(TxBuffer), rxName);

  /* ADC intterput init */
  adcInterruptSetup();

  Slave.startListening();                                      //RX
  Slave.writeAckPayload(ACKpipe, TxBuffer, TX_ACK_PAYLOAD_SIZE );    //RX
}

void loop() {
  
  if(Slave.available()) {     //Yeah! Data received
    Slave.writeAckPayload(0, TxBuffer, TX_ACK_PAYLOAD_SIZE );
    while(Slave.available()) {
      Slave.read(RxBuffer, RECEIVE_DATA_SIZE);
    }
  }
  else {
    Slave.writeAckPayload( ACKpipe, TxBuffer, TX_ACK_PAYLOAD_SIZE );
    Serial.print("\nTx Buffer[0] " + String(TxBuffer[0]));
    Serial.print("\nRx Buffer[0] " + String(RxBuffer[0]));
  }

}
