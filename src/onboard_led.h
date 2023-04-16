#include <Arduino.h>
#include "mserial.h"

#ifndef ONBOARD_LED_CLASS_DEF
  #define ONBOARD_LED_CLASS_DEF

class ONBOARD_LED_CLASS {
  private:
      mSerial* mserial=nullptr;
             
  public:
      // ________________ Onborad LED  _____________
    int8_t LED_RED ;
    int8_t LED_BLUE;
    int8_t LED_GREEN ;

    int8_t LED_RED_CH ;
    int8_t LED_BLUE_CH ;
    int8_t LED_GREEN_CH ;
    
    int8_t led[3];

    ONBOARD_LED_CLASS();

    void init();
    void turnOffAllStatusLED();
    void statusLED(  uint8_t brightness, float time=0);
    void blinkStatusLED( uint8_t brightness, float time=0, uint8_t numberBlinks=1);
};

#endif