#include "manage_mcu_freq.h"
#include <Arduino.h>


//--------------------------------------------------------------------------
// MCU Clock Management ******************************

//function takes the following frequencies as valid values:
//  240, 160, 80    <<< For all XTAL types
//  40, 20, 10      <<< For 40MHz XTAL
//  26, 13          <<< For 26MHz XTAL
//  24, 12          <<< For 24MHz XTAL
// For More Info Visit: https://deepbluembedded.com/esp32-change-cpu-speed-clock-frequency/



// *************************** MCU frequency **********************************************************************
void changeMcuFreq(INTERFACE_CLASS* interface, int Freq){
  delay(1000);
  setCpuFrequencyMhz(Freq);
  interface->CURRENT_CLOCK_FREQUENCY=Freq;
  changeSerialBaudRate(interface, Freq);
} 

void changeSerialBaudRate(INTERFACE_CLASS* interface, uint32_t Freq){
  if (Freq < 80) {
    interface->MCU_FREQUENCY_SERIAL_SPEED = 80 / Freq * interface->SERIAL_DEFAULT_SPEED;
  }
  else {
    interface->MCU_FREQUENCY_SERIAL_SPEED = interface->SERIAL_DEFAULT_SPEED;
  }
  Serial.end();
  Serial.begin(interface->MCU_FREQUENCY_SERIAL_SPEED);
  Serial.println(interface->MCU_FREQUENCY_SERIAL_SPEED);
}