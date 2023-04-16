#include <Arduino.h>
#include "interface_class.h"

#ifndef MCU_FREQ_MANAGEMENT
  #define MCU_FREQ_MANAGEMENT

  // MCU Clock Management ******************************
  //function takes the following frequencies as valid values:
  //  240, 160, 80    <<< For all XTAL types
  //  40, 20, 10      <<< For 40MHz XTAL
  //  26, 13          <<< For 26MHz XTAL
  //  24, 12          <<< For 24MHz XTAL
  // For More Info Visit: https://deepbluembedded.com/esp32-change-cpu-speed-clock-frequency/

  // *************************** MCU frequency **********************************************************************
  void changeMcuFreq(INTERFACE_CLASS* interface, int Freq);

  void changeSerialBaudRate(INTERFACE_CLASS* interface, uint32_t Freq);

#endif