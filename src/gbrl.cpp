#include "gbrl.h"
#include "Arduino.h"
#include"m_math.h"
#include "FFat.h"

GBRL::GBRL() {

}

void GBRL::init(INTERFACE_CLASS* interface, mSerial* mserial){
  this->interface=interface;
  this->mserial=mserial;
}



void GBRL::commands(String $BLE_CMD, String commType){
  String dataStr="";

  if($BLE_CMD=="$plug status"){
      this->plug_status(commType);
  }else if($BLE_CMD=="$?" || $BLE_CMD=="$help"){
      this->helpCommands(commType);
  }else if($BLE_CMD.indexOf("$set dn ")>-1){
      this->change_device_name($BLE_CMD,  commType);
  }else if($BLE_CMD.indexOf("$battery ")>-1){
      this->powerManagement($BLE_CMD, commType);
  }else if($BLE_CMD.indexOf("$usb ")>-1){
      this->powerManagement($BLE_CMD, commType);            
  }else if($BLE_CMD.indexOf("$firmware ")>-1){
      this->firmware($BLE_CMD, commType);
  }else if($BLE_CMD=="$dt"){
      dataStr=String(this->interface->rtc.getDateTime(true)) + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);  
  }else if($BLE_CMD.indexOf("$debug ")>-1){
      this->debug_commands($BLE_CMD, commType);
  } else if ( $BLE_CMD.indexOf("$set pwd ")>-1 ){
    if (this->interface->Measurments_EN){
      dataStr="Passowrd Cannot be set while running measurments";
    }else{
      String value = $BLE_CMD.substring(9, $BLE_CMD.length());
      this->interface->config.DEVICE_PASSWORD=value;
      dataStr = "password accepted";
    }
    this->interface->sendBLEstring( dataStr, commType); 

  }

  this->wifi_commands( $BLE_CMD, commType);
}


// ******************************************************
void GBRL::plug_status(String commType){
  String dataStr="";
  dataStr="ADC Plug Status:" + String(char(10));
  dataStr+= "Always ON 3.3V: " + String( this->interface->config.POWER_PLUG_ALWAYS_ON ?  "Yes": "No") + String(char(10));
  dataStr+= "Power is currently " + String( this->interface->POWER_PLUG_IS_ON ? "ON": "OFF") + String( char(10));

  this->interface->sendBLEstring( dataStr, commType);
}


// ********************************************************
void GBRL::runtime(String commType){
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
    this->interface->sendBLEstring( dataStr, commType);
}

// *********************************************************
 void GBRL::helpCommands(String commType){
    String dataStr="GBRL commands:\n" \
                    "$help $?                           - View available GBRL commands\n" \
                    "$dt                                - Device Time\n" \
                    "$set dn [name]                      - Set device name (max 20 chars)\n" \
                    "$set pwd [password]                - Set device access password\n" \
                    "\n" \
                    "$wifi status                       - View WIFI status\n" \
                    "$wifi ssid                         - Add WIFI network\n" \
                    "$wifi clear                        - Clear all WIFI credentials\n" \
                    "\n" \
                    "$debug [on/off] [ble/ uart / all]  - Output Debug\n" \
                    "$debug status                      - View Debug cfg\n" \
                    "debug repository [on/off]          - Save Debug data in the data repository\n" \
                    "\n" \
                    "$debug verbose [on/off]            - Output all debug messages\n" \
                    "$debug errors [on/off]             - Output only Error messages\n\n";

    this->interface->sendBLEstring( dataStr, commType);  
 }

// *********************************************************
void GBRL::wifi_commands(String $BLE_CMD, String commType){
  String dataStr="";
  if($BLE_CMD=="$wifi status"){
      dataStr  = String(this->interface->number_WIFI_networks) + " wifi networks configured\n";
      dataStr += this->outputWIFInetworkList();
      dataStr += "\n\n";
      dataStr += "Current WIFI status:" + String( char(10));
      dataStr += "     IP     : " + WiFi.localIP().toString() + "\n";
      dataStr +=  "    Gateway: " + WiFi.gatewayIP().toString() + "\n\n";

  }else if($BLE_CMD=="$wifi clear"){
      dataStr= "All WIFI credentials were deleted." + String( char(10));
      this->interface->clear_wifi_networks();

  }else if($BLE_CMD=="$wifi ssid"){
    this->selected_menu = "$wifi ssid";
    dataStr= "Network name (SSID) ?" + String( char(10));

  } else if (this->selected_menu=="$wifi ssid"){  
      this->wifi_ssid = $BLE_CMD;
      this->selected_menu = "$wifi pwd";
      dataStr=  this->wifi_ssid + "\nNetwork password? " + String( char(10));

  }  else if (this->selected_menu=="$wifi pwd"){   
      this->selected_menu = "";
      String ssid = this->wifi_ssid;
      String pwd;
      String mask="";
      if ( $BLE_CMD == "none"){
        pwd = "";
        mask= "no passowrd";
      }else{
        pwd = $BLE_CMD;
        mask = "**********";
      }
      this->interface->add_wifi_network(  ssid, pwd );
      
      dataStr= mask + "\n" + "added to the WIFI Netwrok List: " + String( char(10));
      dataStr += this->outputWIFInetworkList();
      dataStr += "\n";
  }

  if ( this->interface->saveSettings() == false ){
    dataStr += "Error saving settings \n";
  }
  this->interface->sendBLEstring( dataStr, commType);  

}

// ______________________________________________________________
String GBRL::outputWIFInetworkList(){
    String dataStr="";

    for(int i=0; i<5 ; i++){
      if ( this->interface->config.ssid[i] != ""){
        dataStr += this->interface->config.ssid[i];
        if ( this->interface->config.password[i] != ""){
          dataStr += " ,  pass: [Y]/N\n";
        }else{
          dataStr += " ,  pass: Y/[N]\n";
        }
      }
    }
    return dataStr;
}
// *********************************************************

void GBRL::debug_commands(String $BLE_CMD, String commType){
   String dataStr="";
 if($BLE_CMD=="$debug repository on"){
      this->interface->mserial->DEBUG_SEND_REPOSITORY=1;
      dataStr= "Debug to a data repository enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType); 
  }else if($BLE_CMD=="$debug repository off"){
      this->interface->mserial->DEBUG_SEND_REPOSITORY=0;
      dataStr= "Debug to a data repository disabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType); 

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
      this->interface->sendBLEstring( dataStr + "\n", commType);     

  }else if($BLE_CMD=="$debug verbose"){
      this->interface->mserial->DEBUG_TYPE= this->interface->mserial->DEBUG_TYPE_VERBOSE;
      dataStr= "Debug Verbose enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);        
  }else if($BLE_CMD=="$debug errors"){
      this->interface->mserial->DEBUG_TYPE= this->interface->mserial->DEBUG_TYPE_ERRORS;
      dataStr= "Debug only Errors enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);      

  }else if($BLE_CMD.indexOf("$debug ")>-1){
    if($BLE_CMD.indexOf("$debug off")>-1){
      this->interface->mserial->DEBUG_EN=0;
      dataStr= "Debug disabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);   
    } else if($BLE_CMD=="$debug on ble"){
      this->interface->mserial->DEBUG_EN = 1;
      this->interface->mserial->DEBUG_TO = this->interface->mserial->DEBUG_TO_BLE;
      dataStr= "Debug to BLE enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);  
    }else if($BLE_CMD=="$debug on uart"){
      this->interface->mserial->DEBUG_EN = 1;
      this->interface->mserial->DEBUG_TO= this->interface->mserial->DEBUG_TO_UART;
      dataStr= "Debug to UART enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);  
    }else if($BLE_CMD=="$debug on all"){
      this->interface->mserial->DEBUG_EN = 1;
      this->interface->mserial->DEBUG_TO = this->interface->mserial->DEBUG_TO_BLE_UART;
      dataStr= "Debug to UART & BLE enabled" + String( char(10));
      this->interface->sendBLEstring( dataStr, commType);  
    }
  }

  if ( this->interface->saveSettings() == false ){
    dataStr = "Error saving settings \n";
    this->interface->sendBLEstring( dataStr, commType);  
  }


}

// *******************************************************

void GBRL::change_device_name(String $BLE_CMD, String commType){
  String dataStr="";
  String value = $BLE_CMD.substring(8, $BLE_CMD.length());
  this->interface->config.DEVICE_BLE_NAME= value;
  if (value.length()>20){
    dataStr="max 20 chars allowed\n";
  }else if (this->interface->saveSettings()){
    dataStr = "The Device name is now: "+ value + "\n";
  }else{
    dataStr = "Error saving settings \n";
  }

  if ( this->interface->saveSettings() == false ){
    dataStr += "Error saving settings \n";
  }
  this->interface->sendBLEstring( dataStr, commType);  
}
// *********************************************************

void GBRL::powerManagement(String $BLE_CMD, String commType){
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
      this->interface->sendBLEstring( dataStr, commType); 
      return;
    }
  }else{
      dataStr="UNK $ CMD \r\n";
      this->interface->sendBLEstring( dataStr, commType);
      return; 
  } 

  if (this->interface->saveSettings() == false){
    dataStr = "Error saving settings \n";
    this->interface->sendBLEstring( dataStr, commType);  
  }

}


// *************************************************************
void GBRL::firmware(String $BLE_CMD, String commType){
  String dataStr="";

  if($BLE_CMD.indexOf("$firmware update ")>-1){
    String value= $BLE_CMD.substring(17, $BLE_CMD.length());
    if (value.indexOf("A")>-1){
      // firmwate update auto
      this->interface->config.firmwareUpdate="auto";
    } else if (value.indexOf("OFF")>-1){
      // no firware updates
      this->interface->config.firmwareUpdate="no";
    } else if (value.indexOf("ON")>-1){
      // manual firmware updates
      this->interface->config.firmwareUpdate="manual";
    }else{
      dataStr="UNK $ CMD \r\n";
      this->interface->sendBLEstring( dataStr, commType);
      return; 
    }
  }

  if (this->interface->saveSettings()==false){
    dataStr += "Error saving settings \n";
  }
  this->interface->sendBLEstring( dataStr, commType);  

}

