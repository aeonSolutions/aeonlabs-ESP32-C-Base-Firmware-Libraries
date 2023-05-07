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

#include <Arduino.h>
#include <WiFiMulti.h>
#include "onboard_led.h"
#include <semphr.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>


#ifndef INTERFACE_CLASS_DEF
  #include "interface_class.h"
#endif

#ifndef ONBOARD_LED_CLASS_DEF
  #include "onboard_led.h"
#endif
#ifndef MSERIAL_DEF
  #include "mserial.h"
#endif

#ifndef M_WIFI_CLASS_DEF
  #define M_WIFI_CLASS_DEF

class M_WIFI_CLASS {
  private:
    WiFiMulti* wifiMulti;
    long int lastTimeWifiConnectAttempt;
    static void WiFiEvent(WiFiEvent_t event);
    bool errMsgShown;

   // GBRL commands  *********************************************
    String selected_menu;
    String selected_sub_menu;
    String wifi_ssid;
    String wifi_pwd;

    bool helpCommands(uint8_t sendTo );
    bool wifi_commands(String $BLE_CMD, uint8_t sendTo );
    bool change_device_name(String $BLE_CMD, uint8_t sendTo );


  public:
    int HTTP_TTL; // 20 sec TTL
    WiFiClientSecure client;

    mSerial*            mserial=nullptr;
    INTERFACE_CLASS*    interface=nullptr;
    ONBOARD_LED_CLASS*  onboardLED;
    
    static WiFiEvent_t       event;
    uint32_t                 connectionTimeout;
    bool              WIFIconnected;
    uint8_t number_WIFI_networks;
    SemaphoreHandle_t MemLockSemaphoreWIFI;

    // ________________ Onborad LED  _____________


M_WIFI_CLASS();

void init(INTERFACE_CLASS* interface, mSerial* mserial,  ONBOARD_LED_CLASS* onboardLED);

bool start(uint32_t  connectionTimeout, uint8_t numberAttempts);

bool connect2WIFInetowrk(uint8_t numberAttempts);

String get_wifi_status(int status);
void WIFIscanNetworks();

void WIFIevents();
void updateInternetTime();
void resumePowerSavingMode();
void resumeWifiMode();

bool downloadFileHttpGet(String filename, String httpAddr, uint8_t sendTo);

   // GBRL commands  *********************************************
bool gbrl_commands(String $BLE_CMD, uint8_t sendTo );

};



#endif