/* File with functions prototypes */

/* Libraries */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


/* Directives and Macros */
#define BUFFER_SIZE           4    //Size of transmission buffers 
#define PIPE_ADDRESS_SIZE     5    //Size of pipeline address array
#define RECEIVE_DATA_SIZE     4    //Size of data which can be receive
#define TX_ACK_PAYLOAD_SIZE   3    //Size of data send to TX while send ACK with payload

/* Pins definition */
#define TX_PIN_LED        6            //Pins numbers
#define RX_PIN_LED        5
#define CE_PIN            7
#define CSN_PIN           8

/* UART speed definition */
#define UART_SPEED_96    9600
#define UART_SPEED_115   115200

/* extern variables */
extern volatile uint16_t AdcVal;
extern volatile uint16_t *pAdcVal;

extern String txName;
extern String rxName;

/* Pipeline addresses */
extern uint8_t TxAddresses[PIPE_ADDRESS_SIZE];
extern uint8_t RxAddresses[PIPE_ADDRESS_SIZE];

/* Transmission buffers */
extern uint8_t TxBuffer[BUFFER_SIZE];
extern uint8_t RxBuffer[BUFFER_SIZE];

/* Functions prototypes */
void uartInit(uint32_t Speed = 115200, uint8_t Format = SERIAL_8E1);
uint32_t uartSpeedCheck(uint32_t Speed, struct defaultUartSettings *ptr);
uint8_t  uartFormatCheck(uint8_t Format, struct defaultUartSettings *ptr);

void printBufferReset(uint8_t *buf, uint8_t bufSize, String bufName);
void resetBuffer(uint8_t *buf, uint8_t bufSize);
void bufferCopyMap(uint16_t *source, uint8_t *buf, uint8_t bufSize);

void adcInterruptSetup(void);
void rxISRFunction(void);
