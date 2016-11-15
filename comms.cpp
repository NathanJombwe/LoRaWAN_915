#include "comms.h"

//---------------------------------------------
//LoRaMAC API Callback Functions
//---------------------------------------------
static void McpsConfirm( McpsConfirm_t *mcpsConfirm ){}

static void McpsIndication( McpsIndication_t *mcpsIndication ){}

static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm ){}
//---------------------------------------------


void initComms(Comms *comm)
{    
    comm->LoRaMacPrimitives.MacMcpsConfirm = McpsConfirm;
    comm->LoRaMacPrimitives.MacMcpsIndication = McpsIndication;
    comm->LoRaMacPrimitives.MacMlmeConfirm = MlmeConfirm;
    comm->LoRaMacCallbacks.GetBatteryLevel = BoardGetBatteryLevel;
    LoRaMacInitialization(&comm->LoRaMacPrimitives, &comm->LoRaMacCallbacks);
    
    comm->mibReq.Type = MIB_ADR;
    comm->mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);

    comm->mibReq.Type = MIB_PUBLIC_NETWORK;
    comm->mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);
    
    comm->mibReq.Type = MIB_NET_ID;
    comm->mibReq.Param.NetID = LORAWAN_NETWORK_ID;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);

    comm->mibReq.Type = MIB_DEV_ADDR;
    comm->mibReq.Param.DevAddr = (uint32_t)LORAWAN_DEVICE_ADDRESS;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);

    static uint8_t NwkSKey[] = LORAWAN_NWKSKEY;
    comm->mibReq.Type = MIB_NWK_SKEY;
    comm->mibReq.Param.NwkSKey = NwkSKey;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);

    static uint8_t AppSKey[] = LORAWAN_APPSKEY;
    comm->mibReq.Type = MIB_APP_SKEY;
    comm->mibReq.Param.AppSKey = AppSKey;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);

    comm->mibReq.Type = MIB_NETWORK_JOINED;
    comm->mibReq.Param.IsNetworkJoined = true;
    LoRaMacMibSetRequestConfirm(&comm->mibReq);  
}


void addChannels(Comms *comm)
{
    #define LC4                { 868850000, { ( ( DR_5 << 4 ) | DR_0 ) }, 2 }
    #define LC5                { 869050000, { ( ( DR_5 << 4 ) | DR_0 ) }, 2 }
    
    LoRaMacChannelAdd( 3, ( ChannelParams_t )LC4 );
    LoRaMacChannelAdd( 4, ( ChannelParams_t )LC5 );
                
    comm->mibReq.Type = MIB_RX2_CHANNEL;
    comm->mibReq.Param.Rx2Channel = ( Rx2ChannelParams_t ){ 869525000, DR_3 };
    LoRaMacMibSetRequestConfirm(&comm->mibReq);
}
    
bool sendFrame(uint8_t dataSize ,uint8_t *data,int dataRate, bool isTxConfirmed)
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
    
    if(LoRaMacQueryTxPossible(dataSize, &txInfo) != LORAMAC_STATUS_OK)
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;        
    }
    else
    {
        if(isTxConfirmed)
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = LORAWAN_APP_PORT;
            mcpsReq.Req.Confirmed.fBuffer = data;
            mcpsReq.Req.Confirmed.fBufferSize = dataSize;
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = LORAWAN_DEFAULT_DATARATE;    
        }
        else
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = LORAWAN_APP_PORT;
            mcpsReq.Req.Unconfirmed.fBuffer = data;
            mcpsReq.Req.Unconfirmed.fBufferSize = dataSize;
            mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
    }
    
    if(LoRaMacMcpsRequest(&mcpsReq) != LORAMAC_STATUS_OK)
    {
        /*INSERT ERROR STATUS HERE!!!*/  
        return false; 
    }
    
    return true;
}