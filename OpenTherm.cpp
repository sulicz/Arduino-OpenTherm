#include "OpenTherm.h"

OpenTherm::OpenTherm(uint8_t pin_in, uint8_t pin_out ){
  _ot_core =  new OpenThermCore(pin_in, pin_out);
}

void OpenTherm::activateOT(){
  _ot_core->setupOT();
  _ot_core->activateOT();

}

uint8_t OpenTherm::getError() {
  return _ot_err;
}

bool OpenTherm::processErrors(int32_t  response, int expectResponse){
  if( _ot_err != OT_OK) {
    return false;
  } 
  if (_ot_core->parseOTDataBlockMsgType(response) != expectResponse ) {
     _ot_err = OT_BAD_RESPONSE;
     return false;
  }
  return true;
}


bool OpenTherm::isStatus(uint16_t mask){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 0, _get_stat_req ); //0 get status
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 

  uint16_t result_data = _ot_core->parseOTDataBlockDataValue(response);
  
  if ( (result_data & mask ) == 0)
    return false;
  else
    return true;
}


bool OpenTherm::isFault(){
  return isStatus(0x0001);
}

bool OpenTherm::isCHActive(){
  return isStatus(0x0002);
}

bool OpenTherm::isDHWActive(){
  return isStatus(0x0004);
}

bool OpenTherm::isFlameActive(){
  return isStatus(0x0008);
}


int OpenTherm::setTemperatureHeating(float temp){
  if (temp >= 20) {
    _get_stat_req = OT_CH_AND_DHW;
  } else {
    _get_stat_req = OT_ONLY_DHW;
  }
  
  isFault(); // propíšme nastavení get_status
  delay(200);
  uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_WRITE, 1, _ot_core->floatToOT16(temp) ); //1 set temp
  uint32_t  response  = _ot_core->sendRequest(request);
  _ot_err = _ot_core->getError();
  
  
  if(processErrors(response, OT_MSG_TYPE_SM_WRITE_ACK) == false) {
    return false;
  }
  delay(200);
  isFault(); // propíšme nastavení get_status

  
  return _ot_err;
}

int OpenTherm::setDHWSetPoint(float temp){
 
  isFault(); // probudíme komunikaci
  delay(200);
  
  uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_WRITE, 56, _ot_core->floatToOT16(temp) ); //1 set temp
  uint32_t  response  = _ot_core->sendRequest(request);
  _ot_err = _ot_core->getError();
  
  if(processErrors(response, OT_MSG_TYPE_SM_WRITE_ACK) == false) {
    return false;
  } 
  
  return _ot_err;
}

float OpenTherm::getDHWFlowRate(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 19, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  uint16_t result_data = _ot_core->parseOTDataBlockDataValue(response);
  
  return _ot_core->OT16ToFloat(result_data);
}

    
float OpenTherm::getBoilerWaterTemp(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 25, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  uint16_t result_data = _ot_core->parseOTDataBlockDataValue(response);
  
  return _ot_core->OT16ToFloat(result_data);
}

    
float OpenTherm::getDHWtemperature(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 26, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  uint16_t result_data = _ot_core->parseOTDataBlockDataValue(response);
  
  return _ot_core->OT16ToFloat(result_data);
}

int16_t OpenTherm::getExhaustTemp(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 33, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  uint16_t result_data = _ot_core->parseOTDataBlockDataValue(response);
  
  return (int16_t)result_data;
}



uint16_t OpenTherm::getBurnerStarts(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 116, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  return _ot_core->parseOTDataBlockDataValue(response);
} 


uint16_t OpenTherm::getBurnerOpHours(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 120, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  return _ot_core->parseOTDataBlockDataValue(response);  
}

float OpenTherm::getDHWSetPoint(){

   uint32_t  request = _ot_core->makeOTDataBlock(OT_MSG_TYPE_MS_READ, 56, 0x00 );
   uint32_t  response  = _ot_core->sendRequest(request);
   _ot_err = _ot_core->getError();
   
  if(processErrors(response, OT_MSG_TYPE_SM_READ_ACK) == false) {
    return false;
  } 
  
  uint16_t result_data = _ot_core->parseOTDataBlockDataValue(response);
  
  return _ot_core->OT16ToFloat(result_data);
}


