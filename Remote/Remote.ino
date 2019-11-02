/* Main file of progrma */

/* Libraries */
#include "Functions.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/* Directives and Macros */
#define PIPE_ADDRESS_SIZE  5
#define BUFFER_SIZE        4

#define UART_SPEED_96    9600
#define UART_SPEED_115   115200

#define TX_PIN_LED 6            //Pins numbers
#define RX_PIN_LED 5

/* Variables */
/* Arrays */
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxBuffer[BUFFER_SIZE];

uint8_t TxAddresses[PIPE_ADDRESS_SIZE] = {0x0A, 0x0A, 0x0A, 0x0A, 0x01};  //TX pipeline address
uint8_t RxAddresses[PIPE_ADDRESS_SIZE] = {0x0B, 0x0B, 0x0B, 0x0B, 0x02};  //RX pipeline address

RF24 remote(7, 8);

void setup() {
  uartInit(115200,  SERIAL_8E1);


}

void loop() {

}
