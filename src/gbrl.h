#include "Arduino.h"
#include "interface_class.h"
#include "m_wifi.h"

#ifndef GBRL_COMMANDS  
  #define GBRL_COMMANDS
  

  class GBRL {
    private:
      mSerial* mserial=nullptr;
      INTERFACE_CLASS* interface=nullptr;
      MATURITY_CLASS* maturity=nullptr;
      M_WIFI_CLASS* mWifi=nullptr;
      
   // GBRL commands  *********************************************
      bool firmware(String $BLE_CMD, uint8_t sendTo );
      bool helpCommands( uint8_t sendTo );
      bool runtime( uint8_t sendTo  );
      bool powerManagement(String $BLE_CMD, uint8_t sendTo );
      bool plug_status( uint8_t sendTo  );
      bool debug_commands(String $BLE_CMD, uint8_t sendTo  );
      
    public:
      GBRL();
      void init(INTERFACE_CLASS* interface, MATURITY_CLASS* maturity, mSerial* mserial);
      bool commands(String $BLE_CMD, uint8_t sendTo );
 
};

#endif
