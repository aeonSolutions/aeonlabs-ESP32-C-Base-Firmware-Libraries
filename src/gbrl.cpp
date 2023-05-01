#include "gbrl.h"
#include "Arduino.h"
#include"m_math.h"
#include "FFat.h"
#include "m_atsha204.h"

GBRL::GBRL() {

}

void GBRL::init(INTERFACE_CLASS* interface, MATURITY_CLASS* maturity, mSerial* mserial){
  this->mserial=mserial;
  this->mserial->printStr("init GBRL ...");
  this->interface=interface;
  this->maturity=maturity;
  this->mserial->printStrln("done.");
}


bool GBRL::commands(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";

  if($BLE_CMD == "$uid"){
    dataStr = "This Smart Device Serial Number is : ";
    dataStr += CryptoICserialNumber(this->interface) + "\n";
    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return true;
  }else if($BLE_CMD=="$plug status"){
      return this->plug_status( sendTo );
  }else if($BLE_CMD=="$?" || $BLE_CMD=="$help"){
      return  this->helpCommands( sendTo );
  }else if($BLE_CMD.indexOf("$battery ")>-1){
      return this->powerManagement($BLE_CMD,  sendTo );
  }else if($BLE_CMD=="$settings reset"){
      if (FFat.exists("/settings.cfg") ){
        Serial.println("file exists");
        if(FFat.remove("/settings.cfg") > 0 ){
          this->mserial->printStrln("old settings file deleted");          
        }else{
          this->interface->sendBLEstring( "unable to delete settings\n",  sendTo );  
        }
      }else{
        this->mserial->printStrln("settings file not found");
      }
    this->interface->settings_defaults();
    this->interface->sendBLEstring( "Settings were reseted to default values.\n",  sendTo );  
    return true;
  }else if($BLE_CMD=="$ver"){
      dataStr = "Version " + String(this->interface->firmware_version) + "\n";
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;

  }else if($BLE_CMD.indexOf("$usb ")>-1){
      return this->powerManagement($BLE_CMD,  sendTo );            
  }else if($BLE_CMD.indexOf("$firmware cfg ")>-1){
      return this->firmware($BLE_CMD,  sendTo );
  }else if($BLE_CMD=="$firmware update"){
    this->interface->forceFirmwareUpdate=true;
    dataStr = "Starting Firmware update... one moment.\n";
    this->interface->sendBLEstring( dataStr,  sendTo );  
    return true;
  }else if($BLE_CMD=="$dt"){
      dataStr=String(this->interface->rtc.getDateTime(true)) + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );  
      return true;
  }else if($BLE_CMD.indexOf("$debug ")>-1){
      return this->debug_commands($BLE_CMD,  sendTo );
  } else if ( $BLE_CMD.indexOf("$set pwd ")>-1 ){
    if (this->interface->Measurments_EN){
      dataStr="Passowrd Cannot be set while running measurments\n\n";
    }else{
      String value = $BLE_CMD.substring(9, $BLE_CMD.length());
      this->interface->config.DEVICE_PASSWORD=value;
      dataStr = "password accepted\n\n";
    }
    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return true;
  }

}


// ******************************************************
bool GBRL::plug_status(uint8_t sendTo ){
  String dataStr="";
  dataStr="ADC Plug Status:" + String(char(10));
  dataStr+= "Always ON 3.3V: " + String( this->interface->config.POWER_PLUG_ALWAYS_ON ?  "Yes": "No") + String(char(10));
  dataStr+= "Power is currently " + String( this->interface->POWER_PLUG_IS_ON ? "ON": "OFF") + String( char(10));

  this->interface->sendBLEstring( dataStr,  sendTo );
  return true;
}


// ********************************************************
bool GBRL::runtime(uint8_t sendTo ){
    long int hourT; 
    long int minT; 
    long int secT; 
    long int daysT;
    String dataStr="";
    long int $timedif;
    time_t now = time(nullptr);

    $timedif = time(&now)- this->interface->$espunixtimeStartMeasure;
    hourT = (long int) ($timedif/3600);
    minT = (long int) ($timedif/60 - (hourT*60));
    secT =  (long int) ($timedif - (hourT*3600) - (minT*60));
    daysT = (long int) (hourT/24);
    hourT = (long int) (($timedif/3600) - (daysT*24));
    
    dataStr="RunTime"+ String(char(10)) + String(daysT)+"d "+ String(hourT)+"h "+ String(minT)+"m "+ String(secT)+"s "+ String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo );
    return true;
}

// *********************************************************
 bool GBRL::helpCommands(uint8_t sendTo ){
    String dataStr="GBRL commands:\n" \
                    "$help $?                           - View available GBRL commands\n" \
                    "$dt                                - Device Time\n" \
                    "$ver                               - Device Firmware Version\n" \
                    "$uid                               - This Smart Device Unique Serial Number\n" 
                    "\n" \
                    "$firmware update                   - Update the Device with a newer Firmware\n" \
                    "$firmware cfg [on/auto/manual]     - Configure Firmware updates\n" \
                    "\n" \
                    "$settings reset                    - reset Settings to Default values\n" \
                    "$set pwd [password]                - Set device access password\n" \
                    "\n" \
                    "$debug [on/off] [ble/ uart / all]  - Output Debug\n" \
                    "$debug status                      - View Debug cfg\n" \
                    "debug repository [on/off]          - Save Debug data in the data repository\n" \
                    "\n" \
                    "$debug verbose [on/off]            - Output all debug messages\n" \
                    "$debug errors [on/off]             - Output only Error messages\n\n";

    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return false; 
 }



// *********************************************************

bool GBRL::debug_commands(String $BLE_CMD, uint8_t sendTo ){
   String dataStr="";
 if($BLE_CMD=="$debug repository on"){
      this->interface->mserial->DEBUG_SEND_REPOSITORY=1;
      dataStr= "Debug to a data repository enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
  }else if($BLE_CMD=="$debug repository off"){
      this->interface->mserial->DEBUG_SEND_REPOSITORY=0;
      dataStr= "Debug to a data repository disabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
  }else if($BLE_CMD=="$debug status"){
      if (this->interface->mserial->DEBUG_EN){
        dataStr= "Debug is ON ";

        if ( this->interface->mserial->DEBUG_TO == this->interface->mserial->DEBUG_TO_BLE ){
          dataStr += "BLE ";
        }else if  ( this->interface->mserial->DEBUG_TO == this->interface->mserial->DEBUG_TO_UART ) {
          dataStr += "UART " ;
        }else{
          dataStr += "BLE & UART " ;   
        }

        if (this->interface->mserial->DEBUG_TYPE == this->interface->mserial->DEBUG_TYPE_VERBOSE){
          dataStr += ", Mode: verbose" + String( char(10));
        }else{
          dataStr += ", Mode: errors only" + String( char(10));
        }

        if (this->interface->mserial->DEBUG_SEND_REPOSITORY == 0){
          dataStr += "upload Logs to repository is: OFF " + String( char(10));
        }else{
          dataStr += "upload Logs to repository is: ON " + String( char(10));
        }

      }else{
        dataStr= "Debug is OFF" + String( char(10));
      }
      this->interface->sendBLEstring( dataStr + "\n",  sendTo );     
      return true;

  }else if($BLE_CMD=="$debug verbose"){
      this->interface->mserial->DEBUG_TYPE= this->interface->mserial->DEBUG_TYPE_VERBOSE;
      dataStr= "Debug Verbose enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );   
      return true; 

  }else if($BLE_CMD=="$debug errors"){
      this->interface->mserial->DEBUG_TYPE= this->interface->mserial->DEBUG_TYPE_ERRORS;
      dataStr= "Debug only Errors enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );      
      return true;

  }else if($BLE_CMD.indexOf("$debug ")>-1){
    if($BLE_CMD.indexOf("$debug off")>-1){
      this->interface->mserial->DEBUG_EN=0;
      dataStr= "Debug disabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;

    } else if($BLE_CMD=="$debug on ble"){
      this->interface->mserial->DEBUG_EN = 1;
      this->interface->mserial->DEBUG_TO = this->interface->mserial->DEBUG_TO_BLE;
      dataStr= "Debug to BLE enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );  
      return true;

    }else if($BLE_CMD=="$debug on uart"){
      this->interface->mserial->DEBUG_EN = 1;
      this->interface->mserial->DEBUG_TO= this->interface->mserial->DEBUG_TO_UART;
      dataStr= "Debug to UART enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );  
      return true;

    }else if($BLE_CMD=="$debug on all"){
      this->interface->mserial->DEBUG_EN = 1;
      this->interface->mserial->DEBUG_TO = this->interface->mserial->DEBUG_TO_BLE_UART;
      dataStr= "Debug to UART & BLE enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );  
      return true;
    }
  }
  return false;
}


// *********************************************************

bool GBRL::powerManagement(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";

  if($BLE_CMD.indexOf("$battery ")>-1){
    String value= $BLE_CMD.substring(9, $BLE_CMD.length());
    if (value.indexOf("ON")>-1){
      // usb OFF  BAT ON 
      this->interface->config.isBatteryPowered=true;
      
    } else if (value.indexOf("OFF")>-1){
        // USB ON BAT OFF
          this->interface->config.isBatteryPowered=false;
    }
  } else if($BLE_CMD=="$BAT" || $BLE_CMD=="$bat"){
      this->mserial->printStr("Value from pin: ");
      this->mserial->printStrln(String(analogRead(INTERFACE_CLASS::BATTERY_ADC_IO)));
      this->mserial->printStr("Average value from pin: ");
      this->mserial->printStrln(String(this->interface->BL.pinRead()));
      this->mserial->printStr("Volts: ");
      this->mserial->printStrln(String(this->interface->BL.getBatteryVolts()));
      this->mserial->printStr("Charge level: ");
      this->mserial->printStrln(String(this->interface->BL.getBatteryChargeLevel()));
      this->mserial->printStrln("");
      
      dataStr="Battery at "+ String(this->interface->BL.getBatteryChargeLevel()) + "%" + String(char(10));
      this->interface->sendBLEstring( dataStr,  sendTo );
      return true;
  } else if($BLE_CMD.indexOf("$usb ")>-1){
    String value= $BLE_CMD.substring(5, $BLE_CMD.length());
    if (value.indexOf("ON")>-1){
      // usb ON  BAT OFF 
          this->interface->config.isBatteryPowered=false;
    } else if (value.indexOf("OFF")>-1){
        // USB OFF BAT ON
          this->interface->config.isBatteryPowered=true;
    }else{
      dataStr="UNK $ CMD \r\n";
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
    }
  }else{
      dataStr="UNK $ CMD \r\n";
      this->interface->sendBLEstring( dataStr,  sendTo );
      return true; 
  } 

  if (this->interface->saveSettings()){
    dataStr="$ CMD OK \n";
    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return true; 
  }else{
    dataStr="Error saving settings \n";
    this->interface->sendBLEstring( dataStr,  sendTo );  
    return true;
  }

  return false;
}


// *************************************************************
bool GBRL::firmware(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";

  if($BLE_CMD.indexOf("$firmware cfg ")>-1){
    String value= $BLE_CMD.substring(17, $BLE_CMD.length());
    if (value.indexOf("auto")>-1){
      // firmwate update auto
      this->interface->config.firmwareUpdate="auto";
    } else if (value.indexOf("off")>-1){
      // no firware updates
      this->interface->config.firmwareUpdate="no";
    } else if (value.indexOf("on")>-1){
      // manual firmware updates
      this->interface->config.firmwareUpdate="manual";
    }else{
      dataStr="UNK $ CMD \r\n";
      this->interface->sendBLEstring( dataStr,  sendTo );
      return true; 
    }
  }

  if (this->interface->saveSettings()){
    dataStr="$ CMD OK \n";
    this->interface->sendBLEstring( dataStr,  sendTo );  
  }else{
    dataStr="Error saving settings \n";
    this->interface->sendBLEstring( dataStr,  sendTo );  
  }
return true;
}

