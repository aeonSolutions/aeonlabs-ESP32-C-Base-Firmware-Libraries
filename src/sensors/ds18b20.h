/*
 Copyright (c) 2023 Miguel Tomas, http://www.aeonlabs.science

License Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
You are free to:
   Share — copy and redistribute the material in any medium or format
   Adapt — remix, transform, and build upon the material

The licensor cannot revoke these freedoms as long as you follow the license terms. Under the following terms:
Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. 
You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

NonCommercial — You may not use the material for commercial purposes.

ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under
 the same license as the original.

No additional restrictions — You may not apply legal terms or technological measures that legally restrict others
 from doing anything the license permits.

Notices:
You do not have to comply with the license for elements of the material in the public domain or where your use
 is permitted by an applicable exception or limitation.
No warranties are given. The license may not give you all of the permissions necessary for your intended use. 
For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.


Before proceeding to download any of AeonLabs software solutions for open-source development
 and/or PCB hardware electronics development make sure you are choosing the right license for your project. See 
https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/AeonLabs-Solutions-for-Open-Hardware-&-Source-Development
 for Open Hardware & Source Development for more information.

*/

#include "../interface_class.h"
#include "../mserial.h"

#ifndef OneWire_h
  #include "OneWire.h" 
#endif

#include "DallasTemperature.h"

#ifndef DS18B20_SENSOR_CLASS  
  #define DS18B20_SENSOR_CLASS
  
  class DS18B20_SENSOR {
    private:
      INTERFACE_CLASS* interface=nullptr;
      OneWire onewire; 
      uint8_t EXT_IO_ANALOG_PIN;

   // GBRL commands  *********************************************
      bool helpCommands( uint8_t sendTo );

    public:
      DallasTemperature sensors;
      DeviceAddress insideThermometer;

      uint8_t numSensors;
      bool sensorAvailable;
      float* measurement;
      String* measurement_label;

      DS18B20_SENSOR();
      void init(INTERFACE_CLASS* interface,  uint8_t EXT_IO_ANALOG_PIN);
      bool requestMeasurements();
      bool startDS18B20(uint8_t sendTo = mSerial::DEBUG_ALL_USB_UART_BLE);

      bool commands(String $BLE_CMD, uint8_t sendTo );
 
};

#endif
