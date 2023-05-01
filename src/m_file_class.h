#include "Arduino.h"
#include "mserial.h"
#include "onboard_led.h"

#include <esp_partition.h>
#include "FFat.h"

#ifndef FILE_CLASS_DEF
  #define FILE_CLASS_DEF



class FILE_CLASS  {
  private:
    mSerial* mserial;
    ONBOARD_LED_CLASS* onBoardLED;
    void partloop(esp_partition_type_t part_type);

  public:

    FILE_CLASS(mSerial* mserial); 
    bool init( fs::FS &fs, String partitionName,  uint8_t maxFiles,  mSerial* mserial, ONBOARD_LED_CLASS* onboardLED);
    void partition_info();

    void storage_list_files(fs::FS &fs);
    bool storage_test_write_file(fs::FS &fs);  
    bool storage_test_read_file(fs::FS &fs);
    void printDirectory(File dir, int numTabs);

};

#endif