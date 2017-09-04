# Arduino-OpenTherm

This project is based on http://ihormelnyk.com/page/arduino_opentherm_controller. I made some little upgrades and created library.

Interface of library is:
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
