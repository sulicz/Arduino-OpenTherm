

//P MGS-TYPE SPARE DATA-ID  DATA-VALUE
//0 000      0000  00000000 00000000 00000000

#include "OpenThermCore.h"

OpenThermCore::OpenThermCore(uint8_t pin_in, uint8_t pin_out ){
  _pin_in = pin_in;
  _pin_out = pin_out;
}

void OpenThermCore::setupOT(){
  pinMode(_pin_in, INPUT);
  pinMode(_pin_out, OUTPUT);
}

void OpenThermCore::activateOT() {
  setIdleState();
  delay(1000);
}

void OpenThermCore::setIdleState() {
  digitalWrite(_pin_out, HIGH);
}

void OpenThermCore::setActiveState() {
  digitalWrite(_pin_out, LOW);
}


void OpenThermCore::sendBit(bool high) {
  if (high) setActiveState(); else setIdleState();
  delayMicroseconds(500);
  if (high) setIdleState(); else setActiveState();
  delayMicroseconds(500);
}

void OpenThermCore::sendFrame(uint32_t request) {
  sendBit(HIGH); //start bit
  for (int i = 31; i >= 0; i--) {
    sendBit(bitRead(request, i));
  }
  sendBit(HIGH); //stop bit  
  setIdleState();
}

uint32_t OpenThermCore::sendRequest(uint32_t request) {
  sendFrame(request);

  if (!waitForResponse()) return 0;

  return readResponse();
}


/* orig
 * bool OpenThermCore::waitForResponse() {
  unsigned long time_stamp = micros();
  while (digitalRead(_pin_in) != HIGH) { //start bit
    if (micros() - time_stamp >= 1000000) {
      _ot_err = OT_RESPONSE_TIMOUT;
      return false;
    }
  }
  _ot_err = OT_OK;
  delayMicroseconds(OT_BIT_PERIOD * 1.25); //wait for first bit
  return true;
}
*/

bool OpenThermCore::waitForResponse() {
  unsigned long time_stop = millis() + 1000 ;
  while (digitalRead(_pin_in) != HIGH) { //start bit
    if (millis() >= time_stop) {
      _ot_err = OT_RESPONSE_TIMOUT;
      return false;
    }
  }
  _ot_err = OT_OK;
  delayMicroseconds(OT_BIT_PERIOD * 1.25); //wait for first bit
  return true;
}

uint32_t OpenThermCore::readResponse() {
  unsigned long response = 0;
  for (int i = 0; i < 32; i++) {
    response = (response << 1) | digitalRead(_pin_in);
    delayMicroseconds(OT_BIT_PERIOD);
  }
  
  delay(100);
  
  // test parity
  if (!testParity(response)) {
    _ot_err = OT_PARITY_ERROR;
    return 0;
  }
  
  _ot_err = OT_OK;
  
  return response;
}

// vraci true pokud je parita OK
boolean OpenThermCore::testParity(uint32_t response) {
  uint8_t parity = 0;
  uint32_t bit;
  
  for (bit=0; bit < 32; bit++) {
    parity ^= (response & ( ((uint32_t) 1) <<bit)) ? 1 : 0;
  }
  
  if (parity == 0)   // = parita je v poradku
    return true;
  else
    return false;
}

uint8_t OpenThermCore::getError() {
  return _ot_err;
}

void OpenThermCore::printBinary(uint32_t val) {
  for (int i = 31; i >= 0; i--) {
    Serial.print(bitRead(val, i));
  }
}

void OpenThermCore::printRequest(uint32_t request) {
  Serial.println();
  Serial.print("Request:  ");
  printBinary(request);
  Serial.print(" / "); 
  Serial.print(request, HEX);
  Serial.println();
}

void OpenThermCore::printResponse(uint32_t response) {
  Serial.print("Response: ");
  printBinary(response);
  Serial.print(" / ");
  Serial.print(response, HEX);
  Serial.println();

  if ((response >> 16 & 0xFF) == 25) {
    Serial.print("t=");
    Serial.print(response >> 8 & 0xFF);
    Serial.println("");
  }
}


//P MGS-TYPE SPARE DATA-ID  DATA-VALUE
//0 000      0000  00000000 00000000 00000000
uint32_t OpenThermCore::makeOTDataBlock(uint8_t msgType, uint8_t dataID, uint16_t dataValue){
  
  uint8_t parity = 0;
  uint32_t bit;
  uint32_t result = 0;
  
  result = result | ( ((uint32_t) msgType)  << 28);
  result = result | ( ((uint32_t) dataID)  << 16);
  result = result | ( ((uint32_t) dataValue) );

  parity=0;
  
  for (bit=0; bit < 31; bit++) {
    parity ^= (result & ( ((uint32_t) 1) <<bit)) ? 1 : 0;
  }
  
  result = result | ( ((uint32_t) parity ) << 31 );
   
  return result;
}

uint8_t OpenThermCore::parseOTDataBlockMsgType(uint32_t response){
  return ( (B0111) & (response >> 28));
}

uint8_t OpenThermCore::parseOTDataBlockDataID(uint32_t response){
  return ( (0xFF) & (response >> 16));
}

uint16_t OpenThermCore::parseOTDataBlockDataValue(uint32_t response){
  return ( (0xFFFF) & (response));
}


// convet float to  OpenThermCore format 16bit
// 1 sing bit; 7bit int; 8bitu faction bits = 16bit integer 1/256 unit  
int16_t OpenThermCore::floatToOT16 (float in) {
  int16_t ret = (in * 256.0) ;  

  return ret;
}


// convet  OpenThermCore format 16bit to float
// 1 sing bit; 7bit int; 8bitu faction bits = 16bit integer 1/256 unit  
float OpenThermCore::OT16ToFloat (int16_t in) {
  float ret = (in / 256.0) ;  

  return ret;
}

