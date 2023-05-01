#include "interface_class.h"
#include "mserial.h"
#include "m_wifi.h"

#ifndef GEO_LOCATION_CLASS_DEF
  #define GEO_LOCATION_CLASS_DEF

  class GEO_LOCATION_CLASS {
      private:
          mSerial*            mserial=nullptr;
          INTERFACE_CLASS*    interface=nullptr;
          M_WIFI_CLASS*       mWifi;

          long int            REQUEST_DELTA_TIME;
          long int            $espunixtimePrev;
          
          
      public:
          GEO_LOCATION_CLASS();
          void get_ip_address();
          void get_ip_geo_location_data(String ipAddress="");
          void init(INTERFACE_CLASS* interface, M_WIFI_CLASS* mWifi,  mSerial* mserial);
  };

#endif