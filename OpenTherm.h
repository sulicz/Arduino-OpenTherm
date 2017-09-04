#ifndef __OPENTHERM_H_
#define __OPENTHERM_H_


// Messgae type definition
#define OT_MSG_TYPE_MS_READ       B000
#define OT_MSG_TYPE_MS_WRITE      B001
#define OT_MSG_TYPE_MS_INVALID    B010
#define OT_MSG_TYPE_SM_READ_ACK   B100
#define OT_MSG_TYPE_SM_WRITE_ACK  B101
#define OT_MSG_TYPE_SM_INVALID    B110
#define OT_MSG_TYPE_SM_UNK_DATAID B111

// Opentherm bit period
#if !defined OT_BIT_PERIOD
  #define OT_BIT_PERIOD 1000 //1020 //microseconds, 1ms -10%+15%
#endif

#include "Arduino.h"


class OpenTherm {

  private:
    uint8_t _pin_in;
    uint8_t _pin_out;
    
    void setIdleState();
    void setActiveState();
    void sendBit(bool high);
    void sendFrame(uint32_t request);
    bool waitForResponse();
    uint32_t readResponse();
  
  public:
    OpenTherm(uint8_t pin_in, uint8_t pin_out );
        
    // setup pins
    void setupOT();
    
    void activateOT();
    
    // send request and read response    
    uint32_t sendRequest(uint32_t request);
    
    // print formated - DEBUG on Serial
    void printRequest(uint32_t request);
    void printResponse(uint32_t request);
    void printBinary(uint32_t val);
    
    // create Opentherm datagram from msgType, dataID and dataValue
    uint32_t makeOTDataBlock(uint8_t msgType, uint8_t dataID, uint16_t dataValue);
    
    // convert float number to Opentherm coding
   int16_t floatToOT16 (float in);
    // convert Opentherm coding to float number 
    float OT16ToFloat (int16_t in);
};

#endif
