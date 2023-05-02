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

#include "m_wifi.h"
#include "time.h"
#include "ESP32Time.h"
#include "manage_mcu_freq.h"


#ifndef ESP32PING_H
  #include <ESP32Ping.h>
#endif

M_WIFI_CLASS::M_WIFI_CLASS(){
  this->MemLockSemaphoreWIFI = xSemaphoreCreateMutex();
  this->WIFIconnected=false;
}


// **********************************************
void M_WIFI_CLASS::init(INTERFACE_CLASS* interface, mSerial* mserial, ONBOARD_LED_CLASS* onboardLED){
    this->mserial=mserial;
    this->mserial->printStr("init wifi ...");
    this->interface=interface;
    this->onboardLED=onboardLED;
    this->wifiMulti= new WiFiMulti();
    this->HTTP_TTL= 20000; // 20 sec TTL
    this->lastTimeWifiConnectAttempt=millis();
    this->mserial->printStrln("done");
}

// ************************************************

bool M_WIFI_CLASS::start(uint32_t  connectionTimeout, uint8_t numberAttempts){
    this->connectionTimeout=connectionTimeout;
    
    if (interface->getNumberWIFIconfigured() == 0 && ( millis() - this->lastTimeWifiConnectAttempt > 60000) ){
      this->lastTimeWifiConnectAttempt=millis();
      this->mserial->printStrln("WIFI: You need to add a wifi network first", this->mserial->DEBUG_TYPE_ERRORS);
      return false;
    }
    
    this->wifiMulti= new WiFiMulti();
    WiFi.mode(WIFI_AP_STA);

    for(uint8_t i=0; i < 5; i++){
        if (this->interface->config.ssid[i] !="")
          this->wifiMulti->addAP(this->interface->config.ssid[i].c_str(), this->interface->config.password[i].c_str());        
    }
    this->connect2WIFInetowrk(numberAttempts);
    this->lastTimeWifiConnectAttempt=millis();
    return true;
}


// **************************************************

bool M_WIFI_CLASS::connect2WIFInetowrk(uint8_t numberAttempts){
  if (WiFi.status() == WL_CONNECTED)
    return true;
  
  if (this->interface->getNumberWIFIconfigured() == 0){
    this->mserial->printStrln("You need to add a wifi network first", this->mserial->DEBUG_TYPE_ERRORS);
    return false;
  }

  //WiFi.disconnect(true);
  //WiFi.onEvent(M_WIFI_CLASS::WiFiEvent);
  
  int WiFi_prev_state=-10;
  int cnt = 0;        
  uint8_t statusWIFI=WL_DISCONNECTED;
  
  this->mserial->printStr("Connecting ( "+ String(cnt+1) + ") : ");
  while (statusWIFI != WL_CONNECTED) {
    // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
    this->mserial->printStr( "# " );
    if(this->wifiMulti->run(this->connectionTimeout) == WL_CONNECTED) {
        this->mserial->printStrln("\nWiFi connected");
        this->mserial->printStrln("IP address: ");
        this->mserial->printStrln(String(WiFi.localIP()));
        this->mserial->printStr(String(WiFi.SSID()));
        this->mserial->printStr(" ");
        this->mserial->printStrln(String(WiFi.RSSI()));
        
        statusWIFI = WiFi.waitForConnectResult();
        this->mserial->printStrln("("+String(statusWIFI)+"): "+get_wifi_status(statusWIFI));
        this->WIFIconnected=true;
        this->mserial->printStr( "." );
        return true;
    }
    
    cnt++;
    if (cnt == numberAttempts){
        this->mserial->printStr( "." );
        this->mserial->printStrln("\nCould not connect to a WIFI network after " + String(numberAttempts) + " attempts\n");
        this->WIFIconnected=false;
        return false;       
    }
  }
  
  return true;
}

// ********************************************************
 void M_WIFI_CLASS::resumeStandbyMode(){
    WiFi.disconnect(true);
    if (this->interface->BLE_IS_DEVICE_CONNECTED == false){
      this->interface->$espunixtimeDeviceDisconnected=millis();
      //WiFi.mode(WIFI_OFF);
      changeMcuFreq(interface, this->interface->MIN_MCU_FREQUENCY);
    }   
 }

// ********************************************************
String M_WIFI_CLASS::get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS(0): WiFi is in process of changing between statuses";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED(2): Successful connection is established";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL(1): SSID cannot be reached";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED (4): Password is incorrect";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST (5)";
        case WL_CONNECTED:
        return "WL_CONNECTED (3)";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED (6): Module is not configured in station mode";
    }
}

// *********************************************************
void M_WIFI_CLASS::WIFIscanNetworks(){
  if (WiFi.status() == WL_CONNECTED)
    return;
    

  int n = WiFi.scanNetworks();
  if (n == 0) {
    this->mserial->printStrln("no WIFI networks found\n");
  } else {
   this-> mserial->printStr("\n" + String(n));
    this->mserial->printStrln(" WiFi Networks found:");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      this->mserial->printStr(String(i + 1));
      this->mserial->printStr(": ");
      this->mserial->printStr(String(WiFi.SSID(i)));
      this->mserial->printStr(" (");
      this->mserial->printStr(String(WiFi.RSSI(i)));
      this->mserial->printStr(")");
      this->mserial->printStrln((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }  
}

// *************************************************************
void M_WIFI_CLASS::WiFiEvent(WiFiEvent_t event) {  
  M_WIFI_CLASS::event=event;
}

void M_WIFI_CLASS::WIFIevents(){
  switch (this->event) {
    case SYSTEM_EVENT_WIFI_READY: 
      Serial.println("WiFi interface ready");
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      Serial.println("Completed scan for access points");
      break;
    case SYSTEM_EVENT_STA_START:
      Serial.println("WiFi client started");
      break;
    case SYSTEM_EVENT_STA_STOP:
      Serial.println("WiFi clients stopped");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("Connected to access point");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      xSemaphoreTake(this->MemLockSemaphoreWIFI, portMAX_DELAY); // enter critical section
          this->WIFIconnected=false;
      xSemaphoreGive(this->MemLockSemaphoreWIFI); // exit critical section
      this->mserial->printStrln("Disconnected from WiFi access point");
      //WiFi.begin(ssid, password);
      break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
      Serial.println("Authentication mode of access point has changed");
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("Connection Details");
      Serial.println("     IP     : "+WiFi.localIP().toString());
      Serial.println("     Gateway: "+WiFi.gatewayIP().toString());

      if(!Ping.ping("www.google.com", 3)){
       Serial.println("no Internet connectivity found.");
      }else{
        Serial.println("Connection has Internet connectivity.");
        //init time
      configTime(this->interface->config.gmtOffset_sec, this->interface->config.daylightOffset_sec, this->interface->config.ntpServer.c_str());
        this->updateInternetTime();
      }

      xSemaphoreTake(this->MemLockSemaphoreWIFI, portMAX_DELAY); // enter critical section
        this->WIFIconnected=true;
      xSemaphoreGive(this->MemLockSemaphoreWIFI); // exit critical section  
      delay(200);
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("Lost IP address and IP address is reset to 0");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
     Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("WiFi access point started");
      break;
    case SYSTEM_EVENT_AP_STOP:
     Serial.println("WiFi access point  stopped");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Client connected");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Client disconnected");
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      Serial.println("Assigned IP address to client");
      break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
     Serial.println("Received probe request");
      break;
    case SYSTEM_EVENT_GOT_IP6:
      Serial.println("IPv6 is preferred");
      break;
    case SYSTEM_EVENT_ETH_START:
      Serial.println("Ethernet started");
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("Ethernet stopped");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
     Serial.println("Ethernet connected");
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("Ethernet disconnected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.println("Obtained IP address");
      break;
    default: break;
  }
}


// *************************************************************
void M_WIFI_CLASS::updateInternetTime(){  
  if (WiFi.status() != WL_CONNECTED)
    return;

  long diff= (millis()- this->interface->NTP_last_request);
  if(abs(diff)< this->interface->config.NTP_request_interval && this->interface->NTP_last_request!=0){
    this->mserial->printStrln("Internet Time (NTP) is up to date. ");
    return;
  }
  this->interface->NTP_last_request=millis();

  this->mserial->printStrln("Requesting Internet Time (NTP) to "+ String(this->interface->config.ntpServer));
  if(!getLocalTime(&this->interface->timeinfo)){
    this->mserial->printStrln("Failed to obtain Internet Time. Current System Time is " + String(this->interface->rtc.getDateTime(true)) , this->mserial->DEBUG_TYPE_ERRORS);
    return;
  }else{
    this->mserial->printStr("Internet Time is ");
    Serial.println(&this->interface->timeinfo, "%A, %B %d %Y %H:%M:%S");

    this->interface->rtc.setTimeStruct(this->interface->timeinfo); 

    //this->interface->rtc.setTime(this->interface->timeinfo.tm_hour, this->interface->timeinfo.tm_min, this->interface->timeinfo.tm_sec,
    //                              this->interface->timeinfo.tm_mday, this->interface->timeinfo.tm_mon,this->interface->timeinfo.tm_year); 

    this->mserial->printStrln("Local Time is: " + String(this->interface->rtc.getDateTime(true)));
  }
}

   // GBRL commands  *********************************************
   // ***********************************************************
bool M_WIFI_CLASS::gbrl_commands(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";

  if($BLE_CMD=="$?" || $BLE_CMD=="$help"){
      return  this->helpCommands( sendTo );
  }else if($BLE_CMD=="$view dn" ){
    dataStr =  "The devie BLE name is " + this->interface->config.DEVICE_BLE_NAME + "\n";
    this->interface->sendBLEstring(dataStr,  sendTo ); 
    return true; 
  }else if($BLE_CMD.indexOf("$set dn ")>-1){
      return this->change_device_name($BLE_CMD,   sendTo );
  }

  return this->wifi_commands( $BLE_CMD,   sendTo );
}

// *********************************************************
 bool M_WIFI_CLASS::helpCommands(uint8_t sendTo ){
    String dataStr="Device Connectivity Commands:\n" \

                    "$set dn [name]                     - Set device BLE name (max 20 chars)\n" \
                    "$view dn                           - View device BLE name\n" \
                    "\n" \
                    "$wifi status                       - View WIFI status\n" \
                    "$wifi networks                     - View configured WIFI networks\n" \
                    "$wifi ssid                         - Add WIFI network\n" \
                    "$wifi clear                        - Clear all WIFI credentials\n\n";

    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return false; 
 }

 // *********************************************************
bool M_WIFI_CLASS::wifi_commands(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";
  if($BLE_CMD=="$wifi status"){
      dataStr= "Current WIFI status:" + String( char(10));
      dataStr += "     IP     : " + WiFi.localIP().toString() + "\n";
      dataStr +=  "    Gateway: " + WiFi.gatewayIP().toString() + "\n\n";
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
  }
  
  if($BLE_CMD=="$wifi clear"){
      dataStr= "All WIFI credentials were deleted." + String( char(10));
      this->interface->clear_wifi_networks();
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
  }
// .....................................................................................
  if (this->selected_menu=="$wifi pwd"){   
    this->selected_menu = "wifi pwd completed";
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
    this->interface->saveSettings();
    
    dataStr= mask + "\n";
      Serial.println("save 21");
  }

  if (this->selected_menu=="wifi pwd completed" || $BLE_CMD=="$wifi networks"){
    this->selected_menu = ""; 
    dataStr += "WIFI Netwrok List: " + String( char(10));
      Serial.println("save 22");
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
    dataStr += "\n";
  Serial.println("save 23");
    this->interface->sendBLEstring( dataStr,  sendTo ); 
      Serial.println("save 24");
    return true;
  }
  // ................................................................
  if (this->selected_menu=="$wifi ssid"){  
      this->wifi_ssid = $BLE_CMD;
      this->selected_menu = "$wifi pwd";
      dataStr=  this->wifi_ssid + "\nNetwork password? " + String( char(10));
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
  }

  if($BLE_CMD=="$wifi ssid"){
    this->selected_menu = "$wifi ssid";
    dataStr= "Network name (SSID) ?" + String( char(10));
    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return true;
  }

  if($BLE_CMD.indexOf("$wifi ")>-1){
    dataStr= "Invalid WIFI command" + String( char(10));
    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return true;
  }

  return false;
}

// *******************************************************

bool M_WIFI_CLASS::change_device_name(String $BLE_CMD, uint8_t sendTo ){
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
  this->interface->sendBLEstring( dataStr,  sendTo );  
  return true;
}