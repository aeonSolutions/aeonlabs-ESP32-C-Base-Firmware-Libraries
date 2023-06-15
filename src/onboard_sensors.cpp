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

#include "onboard_sensors.h"
#include "interface_class.h"
#include "onboard_led.h"
#include "m_math.h"

ONBOARD_SENSORS::ONBOARD_SENSORS(){
  this->NUMBER_OF_ONBOARD_SENSORS=0;
  this->HT_SENSOR_ADDRESS = 0x38;
}

void ONBOARD_SENSORS::init(INTERFACE_CLASS* interface, mSerial* mserial){
    this->mserial=mserial;
    this->mserial->printStrln("\n==============   Init onboard sensors   =======================");
    
    this->interface=interface;
    this->i2c_err_t[0]="I2C_ERROR_OK";
    this->i2c_err_t[1]="I2C_ERROR_DEV";     // 1
    this->i2c_err_t[2]="I2C_ERROR_ACK";     // 2
    this->i2c_err_t[3]="I2C_ERROR_TIMEOUT"; // 3
    this->i2c_err_t[4]="I2C_ERROR_BUS";     // 4
    this->i2c_err_t[5]="I2C_ERROR_BUSY";    // 5
    this->i2c_err_t[6]="I2C_ERROR_MEMORY";  // 6
    this->i2c_err_t[7]="I2C_ERROR_CONTINUE";// 7
    this->i2c_err_t[8]="I2C_ERROR_NO_BEGIN"; // 8

    this->onboardTHsensor = new SHT3X_SENSOR();
    this->onboardTHsensor->init(this->interface, this->HT_SENSOR_ADDRESS);
    
    if ( this->onboardTHsensor->startSHT3X() )
      this->NUMBER_OF_ONBOARD_SENSORS=this->NUMBER_OF_ONBOARD_SENSORS + this->onboardTHsensor->numSensors;

    this->onboardMotionSensor = new LSM3DS6_SENSOR();
    this->onboardMotionSensor->init(this->interface );
    if ( this->onboardMotionSensor->startLSM6DS3() )
      this->NUMBER_OF_ONBOARD_SENSORS=this->NUMBER_OF_ONBOARD_SENSORS + this->onboardMotionSensor->numSensors;

    this->prevReadings[0] = 0.0;
    this->prevReadings[1] = 0.0;
    this->prevReadings[2] = 0.0;
    this->prevReadings[3] = 0.0;
    this->prevReadings[4] = 0.0;
    this->prevReadings[5] = 0.0;
    this->ROLL_THRESHOLD  = 8.5;   
    this->numtimesBeforeDetectMotion=5;
    this->numtimesMotionDetected=0;

    time(&this->$espunixtimePrev);
    this->mserial->printStrln("==================  done   ==================================");
}

// ***************************************************************
void ONBOARD_SENSORS::request_onBoard_Sensor_Measurements(){
    this->onboardTHsensor->requestMeasurements();

    this->ONBOARD_TEMP = onboardTHsensor->measurement[0];
    this->ONBOARD_HUMIDITY = onboardTHsensor->measurement[1];

    getLSM6DS3sensorData();
}


// ********************************************************
void ONBOARD_SENSORS::getLSM6DS3sensorData() {
  if ( this->onboardMotionSensor->sensorAvailable==false ){
    if ( this->onboardMotionSensor->startLSM6DS3() )
      this->NUMBER_OF_ONBOARD_SENSORS=this->NUMBER_OF_ONBOARD_SENSORS + this->onboardMotionSensor->numSensors;
  }
      
  this->prevReadings[0] = this->onboardMotionSensor->measurement[0];
  this->prevReadings[1] = this->onboardMotionSensor->measurement[1];
  this->prevReadings[2] = this->onboardMotionSensor->measurement[2];
  this->prevReadings[3] = this->onboardMotionSensor->measurement[3];
  this->prevReadings[4] = this->onboardMotionSensor->measurement[4];
  this->prevReadings[5] = this->onboardMotionSensor->measurement[5];   
  this->prevReadings[6] = this->onboardMotionSensor->measurement[6];   
    //Get all parameters
   this->onboardMotionSensor->requestMeasurements();
}


// ***********************************************************
bool ONBOARD_SENSORS::motionShakeDetected(uint8_t numShakes){
  time_t timeNow;

  time(&timeNow);

  if (timeNow - this->$espunixtimePrev < 60){
    if ( this->motionDetect() ) {
      if (this->numtimesMotionDetected >= numShakes){
        this->numtimesMotionDetected =0;
        this->$espunixtimePrev=timeNow;
        return true;
      }else{
        this->numtimesMotionDetected ++;
      }
    }
  }else{
    this->numtimesMotionDetected =0;
    this->$espunixtimePrev=timeNow;
  }
  return false;
};

// **********************************************
void ONBOARD_SENSORS::initRollTheshold(){
    this->mserial->printStr("Calibration of motion detection.Dont move the device..");
    float X, Y, Z, totalAccel;
    for (int i=0; i<100; i++) {
      this->onboardMotionSensor->requestMeasurements();
      X += this->onboardMotionSensor->measurement[0]; 
      Y += this->onboardMotionSensor->measurement[1];
      Z += this->onboardMotionSensor->measurement[2];
      delay(200);
    }
    X /= 10;
    Y /= 10;
    Z /= 10;

    totalAccel = sqrt(X*X + Y*Y + Z*Z);
    this->ROLL_THRESHOLD = totalAccel * ( 1.0 + this->interface->config.MOTION_SENSITIVITY); 
    this->mserial->printStrln("Done.\n");
}

// ************************************************************
bool ONBOARD_SENSORS::motionDetect(){
  time_t timeNow;
  long int $timedif;

  time(&timeNow);
  if ( ( timeNow - this->$espunixtimePrev) > 1 && (timeNow != this->interface->$espunixtimePrev) ){ 
    this->getLSM6DS3sensorData();

    // ToDo: prev Time 

    float X, Y, Z, totalAccel;
    for (int i=0; i<10; i++) {
      this->onboardMotionSensor->requestMeasurements();
      X += this->onboardMotionSensor->measurement[0]; 
      Y += this->onboardMotionSensor->measurement[1];
      Z += this->onboardMotionSensor->measurement[2];
      delay(200);
    }
    X /= 10;
    Y /= 10;
    Z /= 10;

    totalAccel = sqrt(X*X + Y*Y + Z*Z);

   // Serial.println("Accel : "+String(totalAccel));

    if (totalAccel > this->ROLL_THRESHOLD) {
      return true;
    } else{
      return false;
    }
  }
}
// *************************************************************

void ONBOARD_SENSORS::I2Cscanner() {
    this->interface->mserial->printStrln("\nScanning I2C Bus (SDA IO:"+ String( interface->I2C_SDA_IO_PIN ) +" SCL IO:" + String( interface->I2C_SCL_IO_PIN ) + ")   ============");
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = interface->I2C_SDA_IO_PIN;
    conf.scl_io_num = interface->I2C_SCL_IO_PIN;
    conf.sda_pullup_en = false;
    conf.scl_pullup_en = false;
    conf.master.clk_speed = 400000;
    i2c_param_config(I2C_NUM_0, &conf);

    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    uint8_t count = 0;
    esp_err_t res;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
    printf("00:         ");
    for (uint8_t i = 3; i < 0x78; i++)
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
        i2c_master_stop(cmd);

        res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
        if (i % 16 == 0){
            printf("\n%.2x:", i);
        }
        if (res == 0){
            printf(" %.2x", i);
            count++;
        }else{
            printf(" --");
        }
        i2c_cmd_link_delete(cmd);
    }
    printf("\n");

  this->interface->mserial->printStr ("Found ");
  this->interface->mserial->printStr (String(count));        // numbers of devices
  this->interface->mserial->printStrln (" device(s).");
  if (count == 0) {
      interface->onBoardLED->led[1] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100,2); 
    }else if (count==2){
      interface->onBoardLED->led[1] = interface->onBoardLED->LED_GREEN;
      interface->onBoardLED->led[1] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100,2); 
    }else{
    interface->onBoardLED->led[1] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100,2); 
  }
  this->interface->mserial->printStrln("  ============ scan completed ================================");

}

// *********************************************************
 bool ONBOARD_SENSORS::helpCommands(uint8_t sendTo){
    String dataStr="Onboard sensors commands:\n" \
                    "$sensor port [on/off]           - Enable/ Disable Power on External Ports\n" \
                    "\n" \
                    "$ms view                        - View motion detection sensitivity\n" \
                    "$ms set [0;1]                   - Set motion detection sensitivity\n" \                    
                    "$ot                             - View Onboard Temperature data ( " + String(char(176)) + String("C )") +  " )\n" \
                    "$oh                             - View Onboard Humidity data ( % )\n" \
                    "\n" \
                    "$oa                             - View onboard acceleartion data ( g ) \n" \
                    "$og                             - View Onboard Gyroscope data ( dps ) \n" \
                    "$om                             - View Microcontroller Temperature ( " + String(char(176)) + String("C )") + " )\n\n";


    this->interface->sendBLEstring( dataStr, sendTo);

    return false; 
 }

// *********************************************************
bool ONBOARD_SENSORS::gbrl_commands(String $BLE_CMD, uint8_t sendTo){
  String dataStr="";
  
  if($BLE_CMD.indexOf("$ms ")>-1){
    if( $BLE_CMD == "$ms view" ){
      dataStr = "Current motion sensitivity is " + String(this->interface->config.MOTION_SENSITIVITY) + "\n";
    }else if ($BLE_CMD.indexOf("$ms set ")>-1){
      String value= $BLE_CMD.substring(8, $BLE_CMD.length());
      dataStr = "$ CMD ERR";
      if (isNumeric(value)){
          double val = (double) value.toDouble();
          if(val>0.0 and val<1.0){
            this->interface->config.MOTION_SENSITIVITY = val;
            dataStr = "new motion sensitivity set: " + String(this->interface->config.MOTION_SENSITIVITY) + "\n";       
          }
      }
    }else{
      dataStr = "$ CMD ERR";
    }
    this->interface->sendBLEstring( dataStr, sendTo); 
    return true; 

  }else if($BLE_CMD=="$?" || $BLE_CMD=="$help"){
      return this->helpCommands(sendTo);
  } else if($BLE_CMD=="$ot"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard Temperature is: ") + String(roundFloat(this->onboardTHsensor->measurement[1],2)) + String(char(176)) + String("C") +String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo); 
    return true; 
  } else if($BLE_CMD=="$oh"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard Humidity is: ") + String(this->onboardTHsensor->measurement[0] )+ String(" %") +String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo);  
    return true; 
  } else if($BLE_CMD=="$oa"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard acceleration data ( g ) : ") + String("  X: ")+String(roundFloat(this->onboardMotionSensor->measurement[0],2)) +"  Y:" + String(roundFloat(this->onboardMotionSensor->measurement[1],2)) + "  Z: " + String(roundFloat( this->onboardMotionSensor->measurement[2],2)) +String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo);  
    return true; 
  } else if($BLE_CMD=="$og"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard angular data ( dps ): ") + String("  X: ") + String(roundFloat( this->onboardMotionSensor->measurement[3] ,2)) +"  Y:" + String(roundFloat( this->onboardMotionSensor->measurement[4] ,2)) + "  Z: " + String(roundFloat( this->onboardMotionSensor->measurement[5] ,2)) +String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo);  
    return true; 
  } else if($BLE_CMD=="$om"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current MCU temperature is: ") + String(roundFloat(this->onboardMotionSensor->measurement[6],2))+ String(char(176)) + String("C")  +String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo);  
    return true; 
  }else if($BLE_CMD.indexOf("$sensor port")>-1){
    if( $BLE_CMD == "$sensor port on" ){
      digitalWrite(this->interface->EXT_PLUG_PWR_EN ,HIGH);
      this->interface->POWER_PLUG_IS_ON=true;
      dataStr="Power Enabled on Sensor Ports";
      this->interface->sendBLEstring( dataStr, sendTo);  
      return true;
    }else if( $BLE_CMD == "$sensor port off" ){
      digitalWrite(this->interface->EXT_PLUG_PWR_EN ,LOW);
      this->interface->POWER_PLUG_IS_ON=false;
      dataStr="Power Disabled on Sensor Ports";
      this->interface->sendBLEstring( dataStr, sendTo);  
      return true;
    }
  }

  return false; 
}


// ******************************************************