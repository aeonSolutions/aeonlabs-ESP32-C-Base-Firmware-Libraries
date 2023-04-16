#include "interface_class.h"
#include "Arduino.h"
#include "m_file_functions.h"
#include "mserial.h"

INTERFACE_CLASS::INTERFACE_CLASS(){}

void INTERFACE_CLASS::init( mSerial* mserial, bool DEBUG_ENABLE) {
  this->$espunixtime = time(0);
  this->$espunixtimePrev = time(0);
  this->$espunixtimeStartMeasure= time(0);

  this->WIFI_FREQUENCY=80 ; // min WIFI MCU Freq is 80-240
  this->MIN_MCU_FREQUENCY=10;
  
  this->SERIAL_DEFAULT_SPEED=115200;    

  this->DATA_VALIDATION_KEY = "A9CD7F1B6688159B54BBE862F638FF9D29E0FA5F87C69D27BFCD007814BA69C9";

  this->mserial=mserial;
  this->mserial->DEBUG_EN=DEBUG_ENABLE;

  this->config.firmwareUpdate="auto";
  this->config.isBatteryPowered=false;
  this->config.isWIFIenabled=false;
 
  this->number_WIFI_networks=0;
  for(int i=0; i<5 ; i++){
    this->config.ssid[i]="";
    this->config.password[i]="";
  }

  this->Measurments_EN=false;
  this->Measurments_NEW=false;
  this->config.SENSOR_DATA_FILENAME="concrete_curing.csv";
  this->config.DEVICE_PASSWORD="";
  
  this->onBoardLED = new ONBOARD_LED_CLASS();
  this->LIGHT_SLEEP_EN=false;
  this->BLE_IS_DEVICE_CONNECTED=false;
  this->config.DEVICE_BLE_NAME="Slab 12A";
}


void INTERFACE_CLASS::init_BLE(BLECharacteristic *pCharacteristicTX){
  this->pCharacteristicTX=pCharacteristicTX;
  this->mserial->pCharacteristicTX=pCharacteristicTX;

}


// ****************************************************
void INTERFACE_CLASS::clear_wifi_networks(){
  for(int i=0; i<5 ; i++){
    this->config.ssid[i]="";
    this->config.password[i]="";
  }
  this->number_WIFI_networks=0;
}

// ****************************************************
void INTERFACE_CLASS::add_wifi_network(String  ssid, String password){

    this->config.ssid[this->number_WIFI_networks]=ssid;
    this->config.password[ this->number_WIFI_networks]= password;
    this->number_WIFI_networks++;
    if( this->number_WIFI_networks>4)
      this->number_WIFI_networks=0;
}

// ***************************************************
int INTERFACE_CLASS::getNumberWIFIconfigured(){

  int counter=0;
  for (int i=0; i<5; i++){
    if (interface->config->ssid[i] != "")
      counter++;
  }
  
  return counter;
}

// ****************************************************

void INTERFACE_CLASS::init_NTP_Time(char* ntpServer_, long gmtOffset_sec_, int daylightOffset_sec_, long NTP_request_interval_){
  this->config.ntpServer = String(ntpServer_);
  this->config.gmtOffset_sec = gmtOffset_sec_; // 3600 for 1h difference
  this->config.daylightOffset_sec = daylightOffset_sec_;
  this->config.NTP_request_interval=NTP_request_interval_;// 64 sec.
  this->NTP_last_request=0;
  this->rtc =  ESP32Time(this->config.gmtOffset_sec);
  configTime(this->config.gmtOffset_sec, this->config.daylightOffset_sec, this->config.ntpServer.c_str());

  this->mserial->printStrln("set RTC clock to firmware Date & Time ...");  
  rtc.setTimeStruct(CompileDateTime(__DATE__, __TIME__)); 

  this->mserial->printStrln(rtc.getDateTime(true));

}

void INTERFACE_CLASS::sendBLEstring(String message, String commType){
  if (commType == "ble"){
    this->mserial->sendBLEstring(message);
  }else{
    Serial.println(message);
  }
}

// --------------------------------------------------------------------------

bool INTERFACE_CLASS::saveSettings(fs::FS &fs){
  this->config.DEBUG_EN = this->mserial->DEBUG_EN; // ON / OFF
  this->config.DEBUG_TO = this->mserial->DEBUG_TO; // UART, BLE   
  this->config.DEBUG_TYPE = this->mserial->DEBUG_TYPE; // Verbose // Errors 
  this->config.DEBUG_SEND_REPOSITORY = this->mserial->DEBUG_SEND_REPOSITORY ;
  auto settingsFile = fs.open(F("/settings.cfg"), "w"); 
  if (!settingsFile)
    return false;

  settingsFile.write((byte *)&this->config, sizeof(this->config));
  settingsFile.close();
  return true;
}

bool INTERFACE_CLASS::loadSettings(fs::FS &fs){
  File settingsFile = fs.open("/settings.cfg", FILE_WRITE);
  if (!settingsFile){
    this->mserial->printStrln("Settings file not found.");
    return false;
  }


  settingsFile.read((byte *)&this->config, sizeof(this->config));
  settingsFile.close();

  this->mserial->DEBUG_EN = this->config.DEBUG_EN ; 
  this->mserial->DEBUG_TO = this->config.DEBUG_TO; 
  this->mserial->DEBUG_TYPE = this->config.DEBUG_TYPE; 
  this->mserial->DEBUG_SEND_REPOSITORY = this->config.DEBUG_SEND_REPOSITORY ;

  return true;
}