#include "onboard_sensors.h"
#include "interface_class.h"
#include "onboard_led.h"
#include "m_math.h"

ONBOARD_SENSORS::ONBOARD_SENSORS(){
    this->NUMBER_OF_ONBOARD_SENSORS=0;

}

void ONBOARD_SENSORS::init(INTERFACE_CLASS* interface, mSerial* mserial){
    this->interface=interface;
    this->mserial=mserial;

    this->mserial->printStrln("Onboard sensors init...");

    this->i2c_err_t[0]="I2C_ERROR_OK";
    this->i2c_err_t[1]="I2C_ERROR_DEV";     // 1
    this->i2c_err_t[2]="I2C_ERROR_ACK";     // 2
    this->i2c_err_t[3]="I2C_ERROR_TIMEOUT"; // 3
    this->i2c_err_t[4]="I2C_ERROR_BUS";     // 4
    this->i2c_err_t[5]="I2C_ERROR_BUSY";    // 5
    this->i2c_err_t[6]="I2C_ERROR_MEMORY";  // 6
    this->i2c_err_t[7]="I2C_ERROR_CONTINUE";// 7
    this->i2c_err_t[8]="I2C_ERROR_NO_BEGIN"; // 8

    this->startSHT();
    this->startLSM6DS3();
    this->prevReadings[0]=0.0;
    this->prevReadings[1]=0.0;
    this->prevReadings[2]=0.0;
    this->prevReadings[3]=0.0;
    this->prevReadings[4]=0.0;
    this->prevReadings[5]=0.0;
    this->ROLL_THRESHOLD  = 1.4;   
    this->numtimesBeforeDetectMotion=5;
    this->numtimesMotionDetected=0;

    time(&this->$espunixtimePrev);
}

// onBoard Sensors
void ONBOARD_SENSORS::request_onBoard_Sensor_Measurements(){
    if (SHTsensorAvail == false) {
      startSHT(); 
    }  

    this->mserial->printStr("SHT31 data: ");
    sht31.read();
    
    sht_temp = sht31.getTemperature();
    sht_humidity = sht31.getHumidity();

    if (! isnan(sht_temp)) { // check if 'is not a number'
      this->mserial->printStr("Temp *C = ");
      this->mserial->printStr(String(sht_temp));
    } else {
      this->mserial->printStrln("Failed to read temperature");
    }

    if (! isnan(sht_humidity)) { // check if 'is not a number'
      this->mserial->printStr("   Hum. % = ");
      this->mserial->printStrln(String(sht_humidity));
    } else {
      this->mserial->printStrln("Failed to read humidity");
    }
    
 
    getLSM6DS3sensorData();
}

// ********************************************************
void ONBOARD_SENSORS::startSHT() {
  
    bool result = sht31.begin(this->SHT31_ADDRESS);
    this->mserial->printStrln("SHT sensor started.");
    
    if (result){
        this->SHTsensorAvail = true;
        this->mserial->printStrln("SHT status code: " + String(sht31.readStatus()));
        if (sht31.readStatus()==0){
          this->mserial->printStr("SHT initialization DONE.");
          this->NUMBER_OF_ONBOARD_SENSORS= this->NUMBER_OF_ONBOARD_SENSORS+1;
          interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
          interface->onBoardLED->statusLED(100,1); 
        }else{
        this->mserial->printStr("SHT initialization ERROR.");
          interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
          interface->onBoardLED->statusLED(100,2); 
        }
        this->mserial->printStrln("");
    }else{
        this->SHTsensorAvail = false;
        this->mserial->printStr("SHT sensor not found ");
        interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
        interface->onBoardLED->statusLED(100,2); 
    }
    this->mserial->printStrln("SHT completed.");
}
// ********************************************************

void ONBOARD_SENSORS::startLSM6DS3(){
  this->mserial->printStr("Starting Motion Sensor...");

  if ( LSM6DS3sensor.begin() != 0 ) {
    this->mserial->printStrln("FAIL!");
    this->mserial->printStrln("Error starting the sensor at specified address");
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->statusLED(100,2); 
    this->MotionSensorAvail=false;
  } else {
    this->mserial->printStrln("started.");
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100,0); 
    this->NUMBER_OF_ONBOARD_SENSORS=this->NUMBER_OF_ONBOARD_SENSORS+1;
    this->MotionSensorAvail = true; 
  }
}

// ********************************************************
void ONBOARD_SENSORS::getLSM6DS3sensorData() {
  if (MotionSensorAvail==false){
    startLSM6DS3();
  }
      

  this->prevReadings[0]=LSM6DS3_Motion_X;
  this->prevReadings[1]=LSM6DS3_Motion_Y;
  this->prevReadings[2]=LSM6DS3_Motion_Z;
  this->prevReadings[3]=LSM6DS3_GYRO_X;
  this->prevReadings[4]=LSM6DS3_GYRO_Y;
  this->prevReadings[5]=LSM6DS3_GYRO_Z;   

    //Get all parameters

  LSM6DS3_Motion_X=LSM6DS3sensor.readFloatAccelX();
  LSM6DS3_Motion_Y=LSM6DS3sensor.readFloatAccelY();
  LSM6DS3_Motion_Z=LSM6DS3sensor.readFloatAccelZ();
  LSM6DS3_GYRO_X=LSM6DS3sensor.readFloatGyroX();
  LSM6DS3_GYRO_Y=LSM6DS3sensor.readFloatGyroY();
  LSM6DS3_GYRO_Z=LSM6DS3sensor.readFloatGyroZ();
  LSM6DS3_TEMP=LSM6DS3sensor.readTempC();
  //mserial->printStrln(String(LSM6DS3sensor.readTempF()));
  LSM6DS3_errors=LSM6DS3sensor.allOnesCounter;
  LSM6DS3_errors=LSM6DS3sensor.nonSuccessCounter;
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
      X += this->LSM6DS3_Motion_X;
      Y += this->LSM6DS3_Motion_Y;
      Z += this->LSM6DS3_Motion_Z;
      delay(1);
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
  this->mserial->printStrln ("I2C scanner. \n Scanning ...");  
  uint8_t count = 0;
  String addr;

  for (uint8_t i = 8; i < 120; i++){

    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    uint8_t error = Wire.endTransmission();
    addr="";
    if (i < 16){
       addr="0";
    }
    addr=addr+String(i, HEX);

    if (error == 0) { // Receive 0 = success (ACK response)
      this->mserial->printStr ("Found address: ");
      this->mserial->printStr (String(i, DEC));
      this->mserial->printStr (" (0x");
      this->mserial->printStr (String(i, HEX));     // PCF8574 7 bit address
      this->mserial->printStrln (")");
      count++;
    } else if (error == 4) {
      this->mserial->printStr("Unknown error at address 0x");
      this->mserial->printStrln(addr);
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100,1); 
    } else{
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->led[1] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->statusLED(100,2); 
    }
  }

  this->mserial->printStr ("Found ");
  this->mserial->printStr (String(count));        // numbers of devices
  this->mserial->printStrln (" device(s).");
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
}

// *********************************************************
 void ONBOARD_SENSORS::helpCommands(String commType){
    String dataStr="Onboard sensors commands:\n" \
                    "$ot                           - View Onboard Temperature data ( " + String(char(176)) + String("C )") +  " )\n" \
                    "$oh                           - View Onboard Humidity data ( % )\n" \
                    "\n" \
                    "$oa                           - View onboard acceleartion data ( g ) \n" \
                    "$og                           - View Onboard Gyroscope data ( dps ) \n" \
                    "$om                           - View Microcontroller Temperature ( " + String(char(176)) + String("C )") + " )\n\n";


    this->interface->sendBLEstring( dataStr, commType);  
 }

// *********************************************************
void ONBOARD_SENSORS::commands(String $BLE_CMD, String commType){
  String dataStr="";
  if($BLE_CMD=="$?" || $BLE_CMD=="$help"){
      this->helpCommands(commType);
  } else if($BLE_CMD=="$ot"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard Temperature is:") + String(roundFloat(this->sht_temp,2)) + String(char(176)) + String("C") +String(char(10));
    this->interface->sendBLEstring( dataStr, commType);  
  } else if($BLE_CMD=="$oh"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard Humidity is:") + String(this->sht_humidity)+ String(" %") +String(char(10));
    this->interface->sendBLEstring( dataStr, commType);  
  } else if($BLE_CMD=="$oa"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard acceleration data ( g ) :") + String("  X: ")+String(roundFloat(this->LSM6DS3_Motion_X,2)) +"  Y:" + String(roundFloat(this->LSM6DS3_Motion_Y,2)) + "  Z: " + String(roundFloat(this->LSM6DS3_Motion_Z,2)) +String(char(10));
    this->interface->sendBLEstring( dataStr, commType);  
  } else if($BLE_CMD=="$og"){
    this->request_onBoard_Sensor_Measurements();
    dataStr=String("Current onboard angular data ( dps ):") + String("  X: ") + String(roundFloat(this->LSM6DS3_GYRO_X,2)) +"  Y:" + String(roundFloat(this->LSM6DS3_GYRO_Y,2)) + "  Z: " + String(roundFloat(this->LSM6DS3_GYRO_Z,2)) +String(char(10));
    this->interface->sendBLEstring( dataStr, commType);  
  } else if($BLE_CMD=="$om"){
    this->request_onBoard_Sensor_Measurements();
    Serial.print("MCU temp: ");
    Serial.println(this->LSM6DS3_TEMP);
    dataStr=String("Current MCU temperature is:") + String(roundFloat(this->LSM6DS3_TEMP,2))+ String(char(176)) + String("C")  +String(char(10));
    this->interface->sendBLEstring( dataStr, commType);  
  }
}


// ******************************************************