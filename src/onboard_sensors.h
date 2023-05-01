
#include "SparkFunLSM6DS3.h"
#include "mserial.h"
#include "interface_class.h"
#include <Wire.h>
#include "AHT20.h"

#ifndef ONBOARD_SENSORS_DEF
  #define ONBOARD_SENSORS_DEF

class ONBOARD_SENSORS {
  private:

  public:
    INTERFACE_CLASS* interface=nullptr;
    mSerial* mserial= nullptr;

    //AHT20 sensor  **********************************
    bool AHTsensorAvail;
    float aht_temp, aht_humidity;
    uint8_t AHT20_ADDRESS;
    AHT20* aht20;


    // LSM6DS3 motion sensor  ******************************
    uint8_t LSM6DS3_ADDRESS= 0x6B; // default address is 0x6B
    uint8_t IMU_CS_IO =38;
    bool MotionSensorAvail;
    LSM6DS3 LSM6DS3sensor= LSM6DS3( I2C_MODE, LSM6DS3_ADDRESS);
    float LSM6DS3_errors, LSM6DS3_Motion_X, LSM6DS3_Motion_Y, LSM6DS3_Motion_Z, LSM6DS3_GYRO_X,  LSM6DS3_GYRO_Y,  LSM6DS3_GYRO_Z,  LSM6DS3_TEMP;

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
    void startAHT();
    void startLSM6DS3();
    void request_onBoard_Sensor_Measurements();
    void getLSM6DS3sensorData();
    bool motionDetect();
    bool motionShakeDetected(uint8_t numShakes);

    bool commands(String $BLE_CMD, uint8_t sendTo);
    bool helpCommands(uint8_t sendTo);

};

#endif