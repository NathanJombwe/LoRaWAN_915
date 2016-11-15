#include <Timeout.h>
#include <DigitalOut.h>
#include "comms.h"

DigitalOut sendLED(PA_13);

int main(void)
{
    sendLED = 1;
    
    Comms comm;
    initComms(&comm);
    //addChannels(&comm);
    
    /*uint8_t dataSize = 20;
    uint8_t data[20] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
                        0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,
                        0x13,0x14}; */ 
    uint8_t dataSize = 11;
    uint8_t data[11] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
                        0x0A,0x0B};                 
    
    while(1)
    {
        wait(1.0);
        sendFrame(dataSize,data,DR_0,true); //DR_0 is the data rate, true means 
                                            //that downlink confirmations will 
                                            //be recieved
    }
    
    return 0;
}


    


