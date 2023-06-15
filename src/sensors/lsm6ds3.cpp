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

#include "lsm6ds3.h"
#include "Arduino.h"

LSM3DS6_SENSOR::LSM3DS6_SENSOR() {
  this->LSM6DS3_ADDRESS = 0x6B;
  this->numSensors=7;
  this->sensor_n_errors=0;
  this->measurement = new float[this->numSensors] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  this->measurement_label = new String[this->numSensors] {"motion x", "motion y", "motion z", "gyro x", "gyro y", "gyro z", "temperature"};
  this->errorMessage = "";
  this->sensorAvailable=false;
}

void LSM3DS6_SENSOR::init(INTERFACE_CLASS* interface){
  this->interface=interface;
  this->lsm6ds3 = new LSM6DS3( I2C_MODE, this->LSM6DS3_ADDRESS);
}

// ********************************************************
bool LSM3DS6_SENSOR::startLSM6DS3() {
  if ( this->lsm6ds3->begin() != 0 ) {
    this->interface->mserial->printStrln("\nError starting the motion sensor at specified address (0x"+String(this->LSM6DS3_ADDRESS, HEX)+")");
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->statusLED(100,2); 
    this->sensorAvailable=false;
    return false;
  } else {
    this->interface->mserial->printStrln("\ninit motion sensor...done");
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100,0); 
    this->sensorAvailable = true; 
  }
  return true;
}

// *************************************************
 bool LSM3DS6_SENSOR::requestMeasurements(){
    if (this->sensorAvailable == false) {
      this->startLSM6DS3(); 
      if (this->sensorAvailable == false) 
        return false;
    }  
    
  this->measurement[0] = this->lsm6ds3->readFloatAccelX();
  this->measurement[1] = this->lsm6ds3->readFloatAccelY();
  this->measurement[2] = this->lsm6ds3->readFloatAccelZ();
  this->measurement[3] = this->lsm6ds3->readFloatGyroX();
  this->measurement[4] = this->lsm6ds3->readFloatGyroY();
  this->measurement[5] = this->lsm6ds3->readFloatGyroZ();
  this->measurement[6] = this->lsm6ds3->readTempC();
  //mserial->printStrln(String(LSM6DS3sensor.readTempF()));

  this->sensor_n_errors = this->lsm6ds3->nonSuccessCounter;

    return false;
 }


// *********************************************************
 bool LSM3DS6_SENSOR::helpCommands(uint8_t sendTo ){
    String dataStr="LSM6DS3 commands:\n" \
                    "$help $?                           - View available GBRL commands\n" \
                    "$lang set [country code]           - Change the smart device language\n\n";

    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return false; 
 }
// ******************************************************************************************

bool LSM3DS6_SENSOR::commands(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";
  if($BLE_CMD.indexOf("$lang dw ")>-1){

  }

  return false;
}




