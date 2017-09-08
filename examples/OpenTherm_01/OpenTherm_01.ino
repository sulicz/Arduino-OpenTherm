
#define OT_IN_PIN  3 //Arduino UNO
#define OT_OUT_PIN  2 //Arduino UNO


//P MGS-TYPE SPARE DATA-ID  DATA-VALUE
//0 000      0000  00000000 00000000 00000000
uint32_t requests[] = {
  0x300, //0 get status
  0x90014000, //1 set CH temp //64C
  0x80190000, //25 Boiler water temperature
};

#include "OpenTherm.h"

OpenTherm OT(OT_IN_PIN, OT_OUT_PIN);


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
 
  OT.setupOT();
  OT.activateOT(); 

}

void loop() {
  for (int index = 0; index < (sizeof(requests) / sizeof(unsigned long)); index++) {
    OT.printRequest(requests[index]);
    OT.printResponse(OT.sendRequest(requests[index]));
    delay(950);
  }
}

