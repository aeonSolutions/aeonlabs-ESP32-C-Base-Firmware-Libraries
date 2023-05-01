//---------------------------------------------------------------------------------
// EVALUATION OF CONCRETE STRENGTH BY MONITORING CONCRETE TEMPERATURE USING SENSOR
// More Info here: https://www.irjet.net/archives/V6/i3/IRJET-V6I3592.pdf
// Maturity Temperature Time Factor
// TTF = ∑ (Ta – T0) ∆t

/* Where,
     TTF - temperature-time factor, degree hours
     Ta - average concrete temperature during each time interval, °C
     T0 - datum temperature,-10°C
     ∆t - time interval, hours
*/

/* EXPRESSION FOR STRENGTH
     Expression for calculating strength
        MR =Su.e- ( Ʈ / TTF)^α   (Reference ASTM C1074
Where,
   TTF - Average TTF to date of calc. 
   MR - flexural strength or compressive strength
   Su - ultimate expected flexural strength,
   Ʈ - time coefficient
   α - shape parameter
*/

#ifndef MATURITY_CLASS_DEF
  #define MATURITY_CLASS_DEF

  //-------------------------------------------------------------------
  //External DS18b20 sensor
  #include <OneWire.h> 
  #include <DallasTemperature.h>
  #include "mserial.h"
  #include "interface_class.h"
  #include "onboard_sensors.h"
  #include "FFat.h"
  #include "onboard_led.h"
  #include "tinyexpr.h"

  class MATURITY_CLASS {
  private:
    OneWire onewire; 
    String validateExpression(String expression);
    String previous_UFPID;
    uint8_t  get_number_model_vars(String equation);
    double measureValues[8]; // {"a","h","pt","dt","p","m","t","u"};
    int currentDatasetPos;    
    String var[8];
    String signs[9];
    
   // GBRL commands  *********************************************
    bool gbrl_menu_selection(String $BLE_CMD, uint8_t sendTo);
    bool gbrl_model_help(uint8_t sendTo);
    bool gbrl_menu_custom_model(String $BLE_CMD, uint8_t sendTo);
    bool cfg_commands(String $BLE_CMD, uint8_t sendTo);
    bool measurementInterval(uint8_t sendTo);
    bool history(String $BLE_CMD, uint8_t sendTo);
    bool helpCommands(String $BLE_CMD, uint8_t sendTo);
    bool measurementInterval(String $BLE_CMD, uint8_t sendTo);
    bool gbrl_summary_measurement_config(uint8_t sendTo);

  public:
      typedef struct{
          bool custom_maturity_equation_is_summation;
          String custom_maturity_equation;
          String custom_strenght_equation;
          unsigned long MEASUREMENT_INTERVAL; // IN SEC

      } config_strut;
      config_strut config;

      String selected_menu;
      String selected_sub_menu;
      
    DallasTemperature sensors;
    uint8_t EXT_PLUG_DI_IO;

    bool hasNewMeasurementValues;
    float last_measured_probe_temp;
    float last_measured_time_delta;     
    int DATASET_NUM_SAMPLES;

    mSerial* mserial=nullptr;
    INTERFACE_CLASS* interface=nullptr;
    ONBOARD_SENSORS* onBoardSensors=nullptr;
    ONBOARD_LED_CLASS* onboardLED=nullptr;

    MATURITY_CLASS();

    void init(INTERFACE_CLASS* interface, ONBOARD_SENSORS* onBoardSensors, mSerial* mserial, ONBOARD_LED_CLASS* onboardLED);
    
    int get_dataset_size();
    int get_dataset_memory_availability();   
    bool reinitialize_dataset_file(fs::FS &fs);
    void get_dataset_values(int pos);

    void request_measurment_data();
    bool save_measurment_record(fs::FS &fs);

    double custom_strenght(int pos);
    double custom_maturity(int pos);

    // Setup configuration and settings *******************************************
    bool readSettings(fs::FS &fs=FFat);
    bool saveSettings(fs::FS &fs=FFat);


   // GBRL commands  *********************************************
    bool gbrl_commands(String $BLE_CMD, uint8_t sendTo);

};
#endif