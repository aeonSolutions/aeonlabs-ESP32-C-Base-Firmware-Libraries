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


#include "mserial.h"
#include "interface_class.h"
#include <Wire.h>
#include <driver/i2c.h>

//#include "src/sensors/aht20.h"
#include "src/sensors/sht3x.h"

#include "src/sensors/lsm6ds3.h"

#ifndef ONBOARD_SENSORS_DEF
  #define ONBOARD_SENSORS_DEF

class ONBOARD_SENSORS {
  private:

  public:
    INTERFACE_CLASS* interface=nullptr;
    mSerial* mserial= nullptr;

    //AHT20 sensor  **********************************
    bool AHTsensorAvail;
    float ONBOARD_TEMP, ONBOARD_HUMIDITY;
    uint8_t HT_SENSOR_ADDRESS;
    
    //AHT20_SENSOR* onboardTHsensor;
    SHT3X_SENSOR* onboardTHsensor;
    LSM3DS6_SENSOR* onboardMotionSensor;

    // LSM6DS3 motion sensor  ******************************

    bool MOTION_DETECT_EN;
    float ROLL_THRESHOLD ; 
    time_t $espunixtimePrev;
    float prevReadings[6];
    uint8_t numtimesBeforeDetectMotion;
    uint8_t numtimesMotionDetected;

    uint8_t NUMBER_OF_ONBOARD_SENSORS;

    // I2C scanner  ************************************
    // see https://stackoverflow.com/questions/52221727/arduino-wire-library-returning-error-code-7-which-is-not-defined-in-the-library
    char*i2c_err_t[9];

    ONBOARD_SENSORS();
    void I2Cscanner();
    void init(INTERFACE_CLASS* interface, mSerial* mserial);

    void startLSM6DS3();
    void request_onBoard_Sensor_Measurements();
    void getLSM6DS3sensorData();
    bool motionDetect();
    bool motionShakeDetected(uint8_t numShakes);

    bool gbrl_commands(String $BLE_CMD, uint8_t sendTo);
    bool helpCommands(uint8_t sendTo);
    void initRollTheshold();
};

#endif