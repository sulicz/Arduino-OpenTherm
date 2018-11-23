#ifndef __OPENTHERM_H_
#define __OPENTHERM_H_

#include "OpenThermCore.h"

#define OT_ONLY_DHW   0x0200
#define OT_CH_AND_DHW 0x0300

class OpenTherm {

  private:
    OpenThermCore *_ot_core;
    uint8_t _ot_err = OT_OK;
    uint16_t _get_stat_req = OT_ONLY_DHW;
    
    bool isStatus(uint16_t mask);
    bool processErrors(int32_t  response, int expectResponse);
    
  public:
    OpenTherm(uint8_t pin_in, uint8_t pin_out );
    
    void activateOT();
    
    uint8_t getError();

    
    bool isFault(); // je kotel v chybovém stavu
    bool isCHActive(); // central heating
    bool isDHWActive(); // domestic how water
    bool isFlameActive(); // hoří hořák
    
    // nastavi teplotu, vraci err code
    int setTemperatureHeating(float temp);
    
    float getDHWFlowRate();
    float getBoilerWaterTemp(); // výstupní voda z kotle
    float getDHWtemperature();
    
    int16_t getExhaustTemp();
    uint16_t getBurnerStarts();
    uint16_t getBurnerOpHours();
    
    float getDHWSetPoint();
    int setDHWSetPoint(float temp); // vrati err
};

#endif

