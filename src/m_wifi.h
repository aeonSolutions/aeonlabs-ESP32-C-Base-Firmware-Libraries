#include <Arduino.h>
#include <WiFiMulti.h>
#include "onboard_led.h"
#include <semphr.h>
#include "WiFiClientSecure.h"

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
void resumeStandbyMode();

   // GBRL commands  *********************************************
bool gbrl_commands(String $BLE_CMD, uint8_t sendTo );

};



#endif