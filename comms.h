#include "mbed.h"
#include "board.h"
#include "LoRaMac.h"

//---------------------------------------------
//LoRaWAN RF Parameters 
//---------------------------------------------
#define LORAWAN_PUBLIC_NETWORK true

#define LORAWAN_ADR_ON false

#define LORAWAN_APP_PORT 15

#define OVER_THE_AIR_ACTIVATION                     0

#define LORAWAN_DEFAULT_DATARATE DR_0

//---------------------------------------------
//Activation By Personalisation Parameters
//---------------------------------------------
#define LORAWAN_NETWORK_ID                          ( uint32_t )7

#define LORAWAN_DEVICE_ADDRESS                      ( uint32_t )0x00009070

#define LORAWAN_NWKSKEY                             { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }

#define LORAWAN_APPSKEY                             { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }
//---------------------------------------------

typedef struct
{
    LoRaMacPrimitives_t LoRaMacPrimitives;
    LoRaMacCallback_t LoRaMacCallbacks;
    MibRequestConfirm_t mibReq;   
}Comms;

//Initialises the Callback Functions are configures all of the LoRaWAN Communication Parameters
void initComms(Comms *comm);

//Function used to modify default channel parameters
void addChannels(Comms *comm);

//Transmits a Frame to the Gateway
bool sendFrame(uint8_t dataSize ,uint8_t *Data,int dataRate, bool isTxConfirmed);

/*--------------------------------------------------
//NOTE: For the sendFrame dataRate function arg 
//these are the LoRaMAC Datarate Definitions
DR_0 = SF12 - BW125
DR_1 = SF11 - BW125
DR_2 = SF10 - BW125
DR_3 = SF9  - BW125
DR_4 = SF8  - BW125
DR_5 = SF7  - BW125
DR_6 = SF7  - BW250
DR_7 = FSK
--------------------------------------------------*/

