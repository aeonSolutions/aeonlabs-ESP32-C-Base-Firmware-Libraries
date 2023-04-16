#include "m_wifi.h"
#include "time.h"
#include "ESP32Time.h"

#ifndef ESP32PING_H
  #include <ESP32Ping.h>
#endif

M_WIFI_CLASS::M_WIFI_CLASS(){
  this->MemLockSemaphoreWIFI = xSemaphoreCreateMutex();
  this->WIFIconnected=false;
}


// **********************************************
void M_WIFI_CLASS::init(INTERFACE_CLASS* interface, mSerial* mserial, ONBOARD_LED_CLASS* onboardLED){
    this->interface=interface;
    this->mserial=mserial;
    this->onboardLED=onboardLED;
    this->wifiMulti= new WiFiMulti();
    this->HTTP_TTL= 20000; // 20 sec TTL
}

// ************************************************

bool M_WIFI_CLASS::start(uint32_t  connectionTimeout, uint8_t numberAttempts){
    this->connectionTimeout=connectionTimeout;
    
    if (this->interface->config.ssid[0] == "" ){
        this->mserial->printStrln("You need to add a wifi network first", this->mserial->DEBUG_TYPE_ERRORS);
        return false;
    }
    
    this->wifiMulti= new WiFiMulti();
    WiFi.mode(WIFI_AP_STA);

    for(uint8_t i=0; i < 5; i++){
        if (this->interface->config.ssid[i] !="")
          this->wifiMulti->addAP(this->interface->config.ssid[i].c_str(), this->interface->config.password[i].c_str());        
    }
    this->connect2WIFInetowrk(numberAttempts);
    return true;
}



// **************************************************

bool M_WIFI_CLASS::connect2WIFInetowrk(uint8_t numberAttempts){
  if (WiFi.status() == WL_CONNECTED)
    return true;
  
  if (this-.interface->getNumberWIFIconfigured() == 0){
    this->mserial->printStrln("You need to add a wifi network first", this->mserial->DEBUG_TYPE_ERRORS);
    return false;
  }

  //WiFi.disconnect(true);
  //WiFi.onEvent(M_WIFI_CLASS::WiFiEvent);
  
  int WiFi_prev_state=-10;
  int cnt = 0;        
  uint8_t statusWIFI=WL_DISCONNECTED;
  

  while (statusWIFI != WL_CONNECTED) {
    // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
    this->mserial->printStrln("Connection attempt "+ String(cnt+1) + " ...");
    if(this->wifiMulti->run(this->connectionTimeout) == WL_CONNECTED) {
        this->mserial->printStrln("");
        this->mserial->printStrln("WiFi connected");
        this->mserial->printStrln("IP address: ");
        this->mserial->printStrln(String(WiFi.localIP()));
        this->mserial->printStr(String(WiFi.SSID()));
        this->mserial->printStr(" ");
        this->mserial->printStrln(String(WiFi.RSSI()));
        
        statusWIFI = WiFi.waitForConnectResult();
        this->mserial->printStrln("("+String(statusWIFI)+"): "+get_wifi_status(statusWIFI));
        this->WIFIconnected=true;
        return true;
    }
    
    cnt++;
    if (cnt == numberAttempts){
        this->mserial->printStrln("");
        this->mserial->printStrln("Could not connect to a WIFI network after " + String(numberAttempts) + " attempts");
        this->WIFIconnected=false;
        return false;       
    }
  }
  
  return true;
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
    
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  if (n == 0) {
    this->mserial->printStrln("no networks found");
  } else {
   this-> mserial->printStr(String(n));
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
    Serial.println(&this->interface->timeinfo, "%A, %B %d %Y %H:%M:%S");
    
    this->interface->rtc.setTime(this->interface->timeinfo.tm_hour, this->interface->timeinfo.tm_min, this->interface->timeinfo.tm_sec,
                                  this->interface->timeinfo.tm_mday, this->interface->timeinfo.tm_mon,this->interface->timeinfo.tm_year); 

    this->mserial->printStrln("Internet Time is: " + String(this->interface->rtc.getDateTime(true)));
  }
}

// ***********************************************************