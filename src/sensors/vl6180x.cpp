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
#include "vl6180x.h"
#include "Arduino.h"

VL6180X_SENSOR::VL6180X_SENSOR() {
  this->VL6180X_ADDRESS = 0x38;
  this->numSensors = 2;
  this->measurement = new float[this->numSensors] {0.0, 0.0};
  this->measurement_label = new String[this->numSensors] {"lux", "distance"};
  this->errorMessage = "";
}

void VL6180X_SENSOR::init(INTERFACE_CLASS* interface){
  this->interface=interface;
  this->vl6180x = new Adafruit_VL6180X();  
}

// ********************************************************
void VL6180X_SENSOR::startVL6180X() {
  this->interface->mserial->printStr("\ninit VL6180X sensor ...");
  bool result = this->vl6180x->begin();  
  if (result){
    this->sensorAvailable = true;
    this->interface->mserial->printStrln("done.");
  }else{
    this->sensorAvailable = false;
    this->interface->mserial->printStrln("VL6180X sensor not found");
    this->interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    this->interface->onBoardLED->statusLED(100,2); 
  }
}

// *************************************************
 bool VL6180X_SENSOR::requestMeasurements(){
    if (this->sensorAvailable == false) {
      startVL6180X(); 
      if (this->sensorAvailable == false) 
        return false;
    }  
    
    this->measurement[0] = this->vl6180x->readLux(VL6180X_ALS_GAIN_5);    
    this->measurement[1] = this->vl6180x->readRange(); // milimeters
    
    uint8_t status = this->vl6180x->readRangeStatus();
    if (status == VL6180X_ERROR_NONE)
      return true;

    // Some error occurred, print it out!
    if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
      this->errorMessage = "system error";
    }
    else if (status == VL6180X_ERROR_ECEFAIL) {
      this->errorMessage = "ECE failure";
    }
    else if (status == VL6180X_ERROR_NOCONVERGE) {
      this->errorMessage = "No convergence";
    }
    else if (status == VL6180X_ERROR_RANGEIGNORE) {
      this->errorMessage = "Ignoring range";
    }
    else if (status == VL6180X_ERROR_SNR) {
      this->errorMessage = "Signal/noise error";
    }
    else if (status == VL6180X_ERROR_RAWUFLOW) {
      this->errorMessage = "raw reading underflow";
    }
    else if (status == VL6180X_ERROR_RAWOFLOW) {
      this->errorMessage = "raw reading overlfow";
    }
    else if (status == VL6180X_ERROR_RANGEUFLOW) {
      this->errorMessage = "rnge reding underflow";
    }
    else if (status == VL6180X_ERROR_RANGEOFLOW) {
      this->errorMessage = "range reading overflow";
    }
    return false;
 }


// *********************************************************
 bool VL6180X_SENSOR::helpCommands(uint8_t sendTo ){
    String dataStr="VL6180x commands:\n" \
                    "$help $?                           - View available GBRL commands\n" \
                    "$lang set [country code]           - Change the smart device language\n\n";

    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return false; 
 }
// ******************************************************************************************

bool VL6180X_SENSOR::commands(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";
  if($BLE_CMD.indexOf("$lang dw ")>-1){

  }

  return false;
}




