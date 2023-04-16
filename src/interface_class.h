#include "Arduino.h"
#include <math.h>
#include  "BLECharacteristic.h"
#include "maturity.h"
#include "time.h"
#include "ESP32Time.h"
#include "sha204_i2c.h"
#include "mserial.h"
#include "FFat.h"
#include "onboard_led.h"

#ifndef INTERFACE_CLASS_DEF
  #define INTERFACE_CLASS_DEF


  // ******************* Battery Level  *********************
  //* Pangodream_18650_CL BL(ADC_PIN, CONV_FACTOR, READS);
  #include "BAT_18650_CL.h"

  #define BATTERY_CONV_FACTOR 1.7
  #define BATTERY_READS 20
  #define BATTERY_ADC_IO_NUM 21
  

class INTERFACE_CLASS {
  private:
    uint8_t number_WIFI_networks;

  public:
    bool LIGHT_SLEEP_EN;

    typedef struct{
          // ********************** firmware update ****************
      String firmwareUpdate;
      String firmware_version; // ToDo: move outside settings - needs to be a static const value
      
      // ******************* Power management **************
      bool isBatteryPowered;
      bool POWER_PLUG_ALWAYS_ON;

      // ********************* WIFI *************************
      bool isWIFIenabled;
      String ssid[5];
      String  password[5];

    // RTC: NTP server ***********************
     String ntpServer;
     long  gmtOffset_sec; // 3600 for 1h difference
     int   daylightOffset_sec;
     long NTP_request_interval;// 64 sec.

      // ********************** onboard sensors *********************
      bool onboard_motion_sensor_en;
      bool onboard_temperature_en;
      bool onboard_humidity;

    // ************* DEBUG *****************
      bool DEBUG_EN; // ON / OFF
      uint8_t DEBUG_TO; // UART, BLE   
      uint8_t DEBUG_TYPE; // Verbose // Errors 
      bool DEBUG_SEND_REPOSITORY; // YES/ NO

      String SENSOR_DATA_FILENAME;

      String DEVICE_PASSWORD;
      String DEVICE_BLE_NAME;
      
    } config_strut;

    config_strut config;

    // IO Pin assignment *******************************
    int8_t EXT_PLUG_PWR_EN;
    bool POWER_PLUG_IS_ON;

   // ******************** Battery  **************************
    static constexpr int8_t BATTERY_ADC_IO = BATTERY_ADC_IO_NUM;
    Pangodream_18650_CL BL = Pangodream_18650_CL( BATTERY_ADC_IO_NUM, 1.7, 20);

    // ******************* MCU frequency  *********************
    int8_t SAMPLING_FREQUENCY ; 

    int WIFI_FREQUENCY ; // min WIFI MCU Freq is 80-240
    int MIN_MCU_FREQUENCY ;
    int SERIAL_DEFAULT_SPEED;    
    int MCU_FREQUENCY_SERIAL_SPEED;
    int CURRENT_CLOCK_FREQUENCY;
    uint32_t Freq = 0;
    
   // *********************** BLE **************************
     BLECharacteristic *pCharacteristicTX;  
     bool BLE_IS_DEVICE_CONNECTED; 
   
    // RTC SETUP *******************
    ESP32Time rtc;  // offset in seconds GMT

    unsigned long $espunixtime;
    unsigned long $espunixtimePrev;
    unsigned long $espunixtimeStartMeasure;
    unsigned long $espunixtimeDeviceDisconnected;
    String measurement_Start_Time;

    // RTC: NTP server ***********************
    struct tm timeinfo;
    long NTP_last_request;

    // Geo Location  ******************************
    String InternetIPaddress;
    StaticJsonDocument <512> geoLocationInfoJson;
    String requestGeoLocationDateTime;

    // Sensors ****************************************
    int8_t NUMBER_OF_SENSORS; 
    bool Measurments_EN; // start / end collecting data
    bool Measurments_NEW; // start / end collecting data

    // UNIQUE FingerPrint ID for Live data Authenticity and Authentication ******************************
    atsha204Class sha204 = atsha204Class();
    /* WARNING: is required to set BUFFER_LENGTH to at least 64 in Wire.h and twi.h  */
    const char *DATA_VALIDATION_KEY;

    // functions and methods  ****************************
    INTERFACE_CLASS();

    mSerial* mserial=nullptr;
    ONBOARD_LED_CLASS* onBoardLED=nullptr;
    
    void init(mSerial* mserial, bool DEBUG_ENABLE);
    void init_BLE(BLECharacteristic *pCharacteristicTX);

    bool loadSettings(fs::FS &fs=FFat );
    bool saveSettings(fs::FS &fs=FFat );

    void sendBLEstring(String message="", String commType="ble");

    void init_NTP_Time(char* ntpServer_="pool.ntp.org", long gmtOffset_sec_=0, int daylightOffset_sec=3600, long NTP_request_interval_=64000);

    void add_wifi_network( String ssid, String password);
    void clear_wifi_networks();
    int getNumberWIFIconfigured();
};

#endif