#include <Arduino.h>
#include <semphr.h>
#include "FFat.h"
#include  "BLECharacteristic.h"
#include <HardwareSerial.h>
#include "USB.h"



#ifndef  MSERIAL_DEF
  #define  MSERIAL_DEF


  class mSerial {
    private:
      SemaphoreHandle_t MemLockSemaphoreSerial = xSemaphoreCreateMutex();
      SemaphoreHandle_t MemLockSemaphoreUSBSerial = xSemaphoreCreateMutex();
      
      void log(String str, uint8_t debugType, uint8_t sendTo );
      HardwareSerial* UARTserial;

    public:
       // *********************** BLE **************************
     BLECharacteristic *pCharacteristicTX;   
      bool ble;
      
      // ************* DEBUG *****************
      static constexpr uint8_t DEBUG_TYPE_VERBOSE = 100 ;
      static constexpr uint8_t DEBUG_TYPE_ERRORS = 101;

      static constexpr uint8_t DEBUG_TO_BLE = 10;
      static constexpr uint8_t DEBUG_TO_UART = 11;
      static constexpr uint8_t DEBUG_TO_USB = 13;
      static constexpr uint8_t DEBUG_BOTH_USB_UART = 14;
      static constexpr uint8_t DEBUG_TO_BLE_UART = 12;

      bool DEBUG_EN; // ON / OFF
      uint8_t DEBUG_TO; // UART, BLE, USB   
      uint8_t DEBUG_TYPE; // Verbose // Errors 
      bool DEBUG_SEND_REPOSITORY; // YES/ NO
      String LogFilename;
      String serialDataReceived;

      bool reinitialize_log_file(fs::FS &fs);
      void setDebugMode(boolean stat);

      bool saveLog(fs::FS &fs, String str);
      bool readLog(fs::FS &fs);


// ******************************
      mSerial(bool DebugMode, HardwareSerial* UARTserial);
      void start(int baud);

      void printStrln( String str,   uint8_t debugType = mSerial::DEBUG_TYPE_VERBOSE , uint8_t DEBUG_TO = mSerial::DEBUG_TO_UART ); // default DEBUG_TYPE_VERBOSE
      void printStr( String str,  uint8_t debugType = mSerial::DEBUG_TYPE_VERBOSE, uint8_t DEBUG_TO = mSerial::DEBUG_TO_UART  );
      
      void sendBLEstring(String message, uint8_t sendTo = mSerial::DEBUG_TO_BLE);
      bool readSerialData();
      bool readUARTserialData();

  };
#endif