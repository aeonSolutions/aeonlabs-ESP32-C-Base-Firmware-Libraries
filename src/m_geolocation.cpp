

GEO_LOCATION_CLASS::GEO_LOCATION_CLASS(){
}

// **********************************************
void GEO_LOCATION_CLASS::init(INTERFACE_CLASS* interface, mSerial* mserial, ONBOARD_LED_CLASS* onboardLED){
    this->interface=interface;
    this->mserial=mserial;
    this->onboardLED=onboardLED;
}

// *************************************************************************
String GEO_LOCATION_CLASS::get_ip_address(){
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = "https://api.ipify.org";
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
      Serial.println("WiFi Disconnected");
    }
}

// *************************************************************************
void GEO_LOCATION_CLASS::get_ip_geo_location_data(String ipAddress){
    if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        String serverPath = "http://ip-api.com/json/";
        if (ipAddress==""){
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

            // Parse JSON object
            DeserializationError error = deserializeJson(this->interface->geoLocationInfoJson, JSONpayload);
            if (error) {
                Serial.print("Error deserializing JSON");
                this->geoLocationInfoJson=NULL;
            }else{
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
      Serial.println("WiFi Disconnected");
    }
}

// *************************************************************************