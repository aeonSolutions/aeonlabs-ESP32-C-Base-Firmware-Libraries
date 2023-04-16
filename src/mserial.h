#include <Arduino.h>
#include <semphr.h>
#include "FFat.h"
#include  "BLECharacteristic.h"

#ifndef  MSERIAL_DEF
  #define  MSERIAL_DEF


  class mSerial {
    private:
      SemaphoreHandle_t MemLockSemaphoreSerial = xSemaphoreCreateMutex();
      
      void log(String str , uint8_t debugType);

    public:
       // *********************** BLE **************************
     BLECharacteristic *pCharacteristicTX;   
      bool ble;
      
      // ************* DEBUG *****************
      uint8_t DEBUG_TYPE_VERBOSE;
      uint8_t DEBUG_TYPE_ERRORS;

      uint8_t DEBUG_TO_BLE;
      uint8_t DEBUG_TO_UART;
      uint8_t DEBUG_TO_BLE_UART;

      bool DEBUG_EN; // ON / OFF
      uint8_t DEBUG_TO; // UART, BLE   
      uint8_t DEBUG_TYPE; // Verbose // Errors 
      bool DEBUG_SEND_REPOSITORY; // YES/ NO
      String LogFilename;
      String serialDataReceived;

      bool reinitialize_log_file(fs::FS &fs);
      void setDebugMode(boolean stat);

      bool saveLog(fs::FS &fs, String str);
      bool readLog(fs::FS &fs);


// ******************************
      mSerial(bool DebugMode);
      void start(int baud);

      void printStrln(String str, uint8_t debugType=100); // default DEBUG_TYPE_VERBOSE
      void printStr(String str,  uint8_t debugType=100);
      
      void sendBLEstring(String message);
      bool readSerialData();

  };
#endif