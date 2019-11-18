/* Structures, variables and marcors */

/* Libraries and include files */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/* Directives and Macros */
#define PIPE_ADDRESS_SIZE     5    //Size of pipeline address buffer
#define BUFFER_SIZE           4    //Size of transmission buffers 
#define RECEIVE_DATA_SIZE     4    //Size of data which can be receive
#define TX_ACK_PAYLOAD_SIZE   3    //Size of data send to TX while send ACK with payload

#define TEMPERATURE_SENSOR_CHANNEL 0x08 //number of adc channel to measure temperature

/* Pins definition */
#define TX_PIN_LED        6            //Pins numbers
#define RX_PIN_LED        5
#define CE_PIN            7
#define CSN_PIN           8

/* UART speed definition */
#define UART_SPEED_96    9600
#define UART_SPEED_115   115200


/* Pipeline addresses */
extern uint8_t TxAddresses[PIPE_ADDRESS_SIZE];
extern uint8_t RxAddresses[PIPE_ADDRESS_SIZE];

/* Transmission buffers */
extern uint8_t TxBuffer[BUFFER_SIZE];
extern uint8_t RxBuffer[BUFFER_SIZE];

/* Buffers names */
extern String txName;
extern String rxName;

/* ACK pipeline NO. */
extern uint8_t ACKpipe;

/* Definitions od structures */

/* RF settings structure */
struct rfSettings {
  rf24_pa_dbm_e   Power;
  rf24_datarate_e Datarate;
  uint8_t         RFchannel;
};

/* RF transmission structure */
struct rfTransmitSettings {
  uint8_t           RetriesDelay;
  uint8_t           RetriesCount;   
  boolean           AutoAckState; 
  rf24_crclength_e  CRClenght;
};

/* Pipeline address structure */
struct rfAddr {
    uint8_t TxAddr[5];
    uint8_t RxAddr[5];
};

/* RF buffers structure */
struct rfBuffers {
  uint8_t TxBuf[TX_ACK_PAYLOAD_SIZE];
  uint8_t RxBuf[RECEIVE_DATA_SIZE];
};

/* Temperature measure structure */
struct tempMeasure {
    uint8_t tempChannel;
    uint8_t volatile tempCounter;   
    uint8_t volatile tempMean;
    uint8_t volatile tempSens[8];
};

/* UART settings structure */
struct defaultUartSettings {
  bool speedFlag;
  bool formatFlag;
  uint32_t uartSpeedDef;
  uint8_t  uartFormatDef;
};



/* Extern create structures */
extern tempMeasure        tempStruct;

extern rfSettings         RFpipe_0;
extern rfTransmitSettings TrasnmitPipe_0;
extern rfBuffers          BufPipe_0;
extern rfAddr            AddrPipe_0;
