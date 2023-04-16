#include "mserial.h"
#include <Arduino.h>
#include <semphr.h>

// **************************** == Serial Class == ************************
mSerial::mSerial(bool DebugMode) {
      this->DEBUG_EN = DebugMode;

      this->MemLockSemaphoreSerial = xSemaphoreCreateMutex();

      this->DEBUG_TYPE_VERBOSE = 100;
      this->DEBUG_TYPE_ERRORS = 101;

      this->DEBUG_TO_BLE = 10;
      this->DEBUG_TO_UART = 11;
      this->DEBUG_TO_BLE_UART = 12;

      this->LogFilename="sccd_log.txt";

      this->DEBUG_EN = 1; // ON / OFF
      this->DEBUG_TO = this->DEBUG_TO_UART ; // UART, BLE   
      this->DEBUG_TYPE = this->DEBUG_TYPE_VERBOSE; // Verbose // Errors 
      this->DEBUG_SEND_REPOSITORY = 0; // YES/ NO

      this->ble = true;
}

// ------------------------------------------------------
void mSerial::setDebugMode(boolean stat){
  this->DEBUG_EN=stat;
}

// ----------------------------------------------------
void mSerial::start(int baud) {
  if (this->DEBUG_EN) {
    Serial.begin(baud);
    Serial.println("mSerial started...");
  }
}

// ---------------------------------------------------------
void mSerial::printStrln(String str, uint8_t debugType) {

  this->log(str + String( char(10) ), debugType );

}

// ----------------------------------------------------
void mSerial::printStr(String str, uint8_t debugType) {
  this->log(str, debugType);
}

// ----------------------------------------------------------
  void mSerial::log( String str, uint8_t debugType){
    if (this->DEBUG_EN && ( this->DEBUG_TYPE == debugType || this->DEBUG_TYPE == this->DEBUG_TYPE_VERBOSE ) ) {
      if ( ( this->DEBUG_TO == this->DEBUG_TO_BLE || this->DEBUG_TO == this->DEBUG_TO_BLE_UART ) && this->ble ){
          this->sendBLEstring(str + String( char(10) ) );
      }

      if ( this->DEBUG_TO == this->DEBUG_TO_UART || this->DEBUG_TO == this->DEBUG_TO_BLE_UART){ // debug to UART
        xSemaphoreTake(MemLockSemaphoreSerial, portMAX_DELAY); // enter critical section
          Serial.print(str);
        xSemaphoreGive(MemLockSemaphoreSerial); // exit critical section    
      }

      if (this->DEBUG_SEND_REPOSITORY){
          this->saveLog(FFat, str);
      }
    }
  }
  
  // -----------------------------------------------------------------
bool mSerial::readSerialData(){
  this->serialDataReceived = "";
  if( Serial.available() ){ // if new data is coming from the HW Serial
    while(Serial.available()){
      char inChar = Serial.read();
      this->serialDataReceived += inChar;
    }
    return true;
  }else{
    return false;
  }
}
  // --------------------------------------------------------------------------
bool mSerial::reinitialize_log_file(fs::FS &fs){
    if (fs.exists( "/" +  this->LogFilename ) ){
        if( fs.remove( "/" +  this->LogFilename ) !=true ){
            this->printStrln("Error removing old log file");
            return false;
        }  
    }
}

// --------------------------------------------------------
bool mSerial::saveLog(fs::FS &fs, String str){
  auto logFile = fs.open("/" + this->LogFilename , FILE_WRITE); 
  if (!logFile)
    return false;

  logFile.print(str + String( char(10) ) );
  logFile.close();
  return true;
}

// -----------------------------------------------------
bool mSerial::readLog(fs::FS &fs){
  File logFile = fs.open("/" + this->LogFilename , FILE_READ);
  if (!logFile)
    return false;

  logFile.read();
  logFile.close();
  return true;
}

// -------------------------------------------------------------------------------
void mSerial::sendBLEstring(String message){
// no mSerial output allowed here :: reason loop 
  
  Serial.println("Start send BLE message...");

  float remain= message.length() % 20;

  for(int i=0; i<floor(message.length()/20);i++){
    this->pCharacteristicTX->setValue((message.substring(i*20, (i*20+20) )).c_str());
    this->pCharacteristicTX->notify();
    delay(10);
  }
  Serial.println("end loop send BLE message...");
  if (remain > 0.0){
    this->pCharacteristicTX->setValue((message.substring( floor(message.length()/20)*20, message.length())).c_str());
    this->pCharacteristicTX->notify();  
  }
  Serial.println("end send BLE message...");
}