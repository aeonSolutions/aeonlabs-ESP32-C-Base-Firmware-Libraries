#include "m_file_class.h"
#include <esp_partition.h>
#include "FFat.h"

FILE_CLASS::FILE_CLASS(){}

void FILE_CLASS::init(String partitionName,  mSerial* mserial, ONBOARD_LED_CLASS* onboardLED){
    this->mserial=mserial;
    this->onBoardLED=onBoardLED;
 
    this->mserial->printStrln("File system mounted");
    this->mserial->printStrln("Total space: " + String(FFat.totalBytes()) );
    this->mserial->printStrln("Free space: " + String(FFat.freeBytes()) );
}

// ************************************************************
void FILE_CLASS::partition_info(){
    Serial.println("Partition list:");
    partloop(ESP_PARTITION_TYPE_APP);
    partloop(ESP_PARTITION_TYPE_DATA);
}

void FILE_CLASS::partloop(esp_partition_type_t part_type) {
  esp_partition_iterator_t iterator = NULL;
  const esp_partition_t *next_partition = NULL;
  iterator = esp_partition_find(part_type, ESP_PARTITION_SUBTYPE_ANY, NULL);
  while (iterator) {
     next_partition = esp_partition_get(iterator);
     if (next_partition != NULL) {
        Serial.printf("partition addr: 0x%06x; size: 0x%06x; label: %s\n", next_partition->address, next_partition->size, next_partition->label);  
     iterator = esp_partition_next(iterator);
    }
  }
}

//***********************************************************
void FILE_CLASS::storage_list_files(fs::FS &fs){
    this->mserial->printStrln("Listing Files and Directories: ");
    // Open dir folder
    auto dir = fs.open("/");
    // Cycle all the content
    onBoardLED->led[1] = onBoardLED->LED_RED;
    onBoardLED->statusLED(100,0); 
    this->printDirectory(dir,1);
    onBoardLED->led[1] = onBoardLED->LED_GREEN;
    onBoardLED->statusLED(100,2); 
    this->mserial->printStrln("");
}

//***********************************************************
bool FILE_CLASS::storage_test_write_file(fs::FS &fs){    
    auto testFile = fs.open(F("/testCreate.txt"), "w"); 
    if (testFile){
      this->mserial->printStr("Writing content to a file...");
      testFile.print("Here the test text!!");
      testFile.close();
      this->mserial->printStrln("DONE.");
      return true;
    }else{
      this->mserial->printStrln("Error openning file(1) !");
      onBoardLED->led[1] = onBoardLED->LED_RED;
      onBoardLED->statusLED(100,5); 
      return false;
    }
}

//***********************************************************
bool FILE_CLASS::storage_test_read_file(fs::FS &fs){
    auto testFile = fs.open(F("/testCreate.txt"), "r");
    if (testFile){
      this->mserial->printStrln("-- Reading file content (below) --");
      this->mserial->printStrln(testFile.readString());
      testFile.close();
      this->mserial->printStrln("DONE.");
      this->mserial->printStrln("-- Reading File completed --");
      return true;
    }else{
      this->mserial->printStrln("Error reading file !");
      onBoardLED->led[1] = onBoardLED->LED_RED;
      onBoardLED->statusLED(100,5); 
      return false;
    }
}


//***********************************************************
void FILE_CLASS::printDirectory(File dir, int numTabs) {
  while (true) {
    auto entry =  dir.openNextFile();
    if (! entry) {
      this->mserial->printStrln("");
      this->mserial->printStrln("no more files or directories found.");
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      this->mserial->printStr("     ");
    }
    this->mserial->printStr(entry.name());
    if (entry.isDirectory()) {
      this->mserial->printStrln("/");
      this->printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      this->mserial->printStr("          ");
      this->mserial->printStrln(String(entry.size()));
    }
    entry.close();
  }
}