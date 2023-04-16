
#ifndef INTERFACE_CLASS_DEF
  #include "interface_class.h"
#endif

#ifndef ONBOARD_LED_CLASS_DEF
  #include "onboard_led.h"
#endif
#ifndef MSERIAL_DEF
  #include "mserial.h"
#endif

#ifndef GEO_LOCATION_CLASS_DEF
    #define GEO_LOCATION_CLASS_DEF

    class GEO_LOCATION_CLASS {
        private:
            mSerial*            mserial=nullptr;
            INTERFACE_CLASS*    interface=nullptr;
            ONBOARD_LED_CLASS*  onboardLED;

        public:
            GEO_LOCATION_CLASS();
            String get_ip_address();
            void get_ip_geo_location_data(String ipAddress="");
            void init(INTERFACE_CLASS* interface, mSerial* mserial, ONBOARD_LED_CLASS* onboardLED);
    }

#endif