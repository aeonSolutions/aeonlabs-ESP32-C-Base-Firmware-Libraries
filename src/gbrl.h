#include "Arduino.h"
#include "interface_class.h"
#include "m_wifi.h"

#ifndef GBRL_COMMANDS  
  #define GBRL_COMMANDS
  

  class GBRL {
    private:
      mSerial* mserial=nullptr;
      INTERFACE_CLASS* interface=nullptr;
      M_WIFI_CLASS* mWifi=nullptr;

      String selected_menu;
      String selected_sub_menu;
      String wifi_ssid;
      String wifi_pwd;

      void firmware(String $BLE_CMD, String commType);
      void helpCommands(String commType);
      void history(String commType);
      void runtime(String commType);
      void powerManagement(String $BLE_CMD, String commType);
      void plug_status(String commType);
      void debug_commands(String $BLE_CMD, String commType);
      void wifi_commands(String $BLE_CMD, String commType);
      void change_device_name(String $BLE_CMD, String commType);
      String outputWIFInetworkList();
    public:
      GBRL();
      void init(INTERFACE_CLASS* interface, mSerial* mserial);
      void commands(String $BLE_CMD, String commType);
 
};

#endif
