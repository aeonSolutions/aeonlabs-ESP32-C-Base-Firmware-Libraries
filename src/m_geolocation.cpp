#include <ArduinoJson.h>
#include "m_geolocation.h"
#include <WiFiMulti.h>
#include "HTTPClient.h"
#include "manage_mcu_freq.h"


GEO_LOCATION_CLASS::GEO_LOCATION_CLASS(){
}

// **********************************************
void GEO_LOCATION_CLASS::init(INTERFACE_CLASS* interface,  M_WIFI_CLASS* mWifi, mSerial* mserial ){
  this->mserial=mserial;
  this->mserial->printStr("init GeoLocation ...");
  this->interface=interface;
  this-> mWifi = mWifi;
  
  this->REQUEST_DELTA_TIME  = 10*60*1000; // 10 min 
  this->$espunixtimePrev= millis(); 
  this->mserial->printStrln("done.");
}

// *************************************************************************
void GEO_LOCATION_CLASS::get_ip_address(){
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = "http://api.ipify.org";
      this->mserial->printStrln("request external IP address");
      http.begin(serverPath.c_str());      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        this->interface->InternetIPaddress = http.getString();
      }else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      //Serial.println("WiFi Disconnected");
    }
}

// *************************************************************************
void GEO_LOCATION_CLASS::get_ip_geo_location_data(String ipAddress){
  
  if ( ( millis() - this->$espunixtimePrev) < this->REQUEST_DELTA_TIME )
    return;
    

  if (this->interface->CURRENT_CLOCK_FREQUENCY <= this->interface->WIFI_FREQUENCY )
      changeMcuFreq(this->interface, this->interface->WIFI_FREQUENCY);
    
  if (WiFi.status() != WL_CONNECTED){
      this->mWifi->start(10000, 5); // TTL , n attempts 
  }
  
  if (WiFi.status() != WL_CONNECTED){
      this->mserial->printStrln("unable to connect to WIFI.");
      return;
  }
  this->$espunixtimePrev= millis();
  
  if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        String serverPath = "http://ip-api.com/json/";
        if (ipAddress==""){
          if (this->interface->InternetIPaddress==""){
            this->get_ip_address();
          }
          serverPath += this->interface->InternetIPaddress;
        }else{
            serverPath += ipAddress;
        }
        this->mserial->printStrln("request Geo Location Data");
        http.begin(serverPath.c_str());      
        // Send HTTP GET request
        int httpResponseCode = http.GET();
      
        if (httpResponseCode>0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String JSONpayload = http.getString();
            Serial.println(JSONpayload);
          
            StaticJsonDocument <512> geoLocationInfoJsonStatic;
            // Parse JSON object
            DeserializationError error = deserializeJson(geoLocationInfoJsonStatic, JSONpayload);
            if (error) {
                Serial.print("Error deserializing JSON");
                //this->interface->geoLocationInfoJson=nullptr;
            }else{
                this->interface->geoLocationInfoJson = geoLocationInfoJsonStatic[0];
                this->interface->requestGeoLocationDateTime= String( this->interface->rtc.getDateTime(true) );
                /*
                {
                    "status":"success",
                    "country":"Belgium",
                    "countryCode":"BE",
                    "region":"BRU",
                    "regionName":"Brussels Capital",
                    "city":"Brussels",
                    "zip":"1000",
                    "lat":50.8534,
                    "lon":4.347,
                    "timezone":"Europe/Brussels",
                    "isp":"PROXIMUS",
                    "org":"",
                    "as":"AS5432 Proximus NV",
                    "query":"37.62.11.2"
                }
                String stat = this->datasetInfoJson["status"];
                */
            }
        }else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();
    }else {
      //Serial.println("WiFi Disconnected");
    }
  this->mWifi->resumeStandbyMode();
}

// *************************************************************************


