/* */
/* Libraries and include files */

#include "Structures.h"


/* Pipeline addresses */
uint8_t TxAddresses[PIPE_ADDRESS_SIZE] = {0x0B, 0x0B, 0x0B, 0x0B, 0x02};  //TX pipeline address
uint8_t RxAddresses[PIPE_ADDRESS_SIZE] = {0x0A, 0x0A, 0x0A, 0x0A, 0x01};  //RX pipeline address

/* Transmission buffers */
uint8_t TxBuffer[BUFFER_SIZE];
uint8_t RxBuffer[BUFFER_SIZE];

/* Buffers names */
String txName = "TX Buffer";
String rxName = "RX Buffer";

/* ACK pipeline NO. */
uint8_t ACKpipe = 0;

/* Create structere inst. */
struct tempMeasure tempStruct;
struct rfSettings RFpipe0;
struct rfBuffers  BufPipe0;
