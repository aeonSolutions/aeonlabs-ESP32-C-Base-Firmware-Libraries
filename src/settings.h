#include <Arduino.h>
#include "sqlite/sqlite3.h"

#ifndef INTERFACE_CLASS_DEF
  #include "interface_class.h"
#endif

#ifndef ONBOARD_LED_CLASS_DEF
  #include "onboard_led.h"
#endif

#ifndef MSERIAL_DEF
  #include "mserial.h"
#endif

#ifndef SETTINGS_CLASS_DEF
  #define SETTINGS_CLASS_DEF

class SETTINGS_CLASS {
  private:
    sqlite3 *db_settings;
    sqlite3_stmt *result;
    char *tail;

    int db_exec(sqlite3 *db, const char *sql);
    int db_open(const char *filename, sqlite3 **db);

    const char* data = "Callback function called";
    //static int callback(void *data, int argc, char **argv, char **azColName);

  public:
    mSerial*            mserial=nullptr;
    INTERFACE_CLASS*    interface=nullptr;
    ONBOARD_LED_CLASS*  onBoardLED;


    SETTINGS_CLASS();
    
    void init(INTERFACE_CLASS* interface, mSerial* mserial,  ONBOARD_LED_CLASS* onBoardLED);
    void load();

    void query(String query);

};

#endif