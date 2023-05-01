#include "settings.h"


SETTINGS_CLASS::SETTINGS_CLASS(){}

// ****************************************************

void SETTINGS_CLASS::init(INTERFACE_CLASS* interface, mSerial* mserial,  ONBOARD_LED_CLASS* onBoardLED){
   this->interface=interface;
   this->mserial=mserial;
   this->onBoardLED=onBoardLED;

   sqlite3_initialize();
    int rc;

  String sql= "CREATE TABLE \"settings\" ( \"id\" INTEGER UNIQUE, \"ntp_server\" TEXT, \"gmt_offset\" TEXT, \"daylight_offset\" TEXT, \"ntp_request_interval\" TEXT, ";
  sql+="\"debug_enable\" TEXT, \"data_validation_key\" TEXT, \"firmware_update\" TEXT, \" is_battery_powered \" TEXT, \"wifi_enabled\" TEXT, ";
  sql+=" PRIMARY KEY(\"id\" AUTOINCREMENT) );";
    if (db_open("/settings.db", &this->db_settings)==false)
        rc = this->db_exec(this->db_settings, sql.c_str());
    
    if (rc != SQLITE_OK) {
        sqlite3_close(this->db_settings);
        this->mserial->printStrln("Error accessing settings database");
        return;
    }
}

// ****************************************************

void SETTINGS_CLASS::load(){
    if (db_open("/settings.db", &this->db_settings)==false)
        return;

    String sql="SELECT from settings ";
    int rc = sqlite3_prepare_v2(this->db_settings, sql.c_str(), -1, &this->result, 0);
    if (rc != SQLITE_OK) {
        String resp = "Failed to fetch data: ";
        this->mserial->printStrln(sqlite3_errmsg(this->db_settings));
        sqlite3_close(this->db_settings);
        return;
    }

    /*
        sqlite3_column_int(res, 0);
        sqlite3_column_text(res, 1);
        resp += sqlite3_column_double(res, 6);
    */

    while (sqlite3_step(this->result) == SQLITE_ROW) {
        this->interface->config.ntpServer= (char*) sqlite3_column_text(this->result, 1) ;
        this->interface->config.gmtOffset_sec= atol( (char *) sqlite3_column_text(this->result, 1) );
        this->interface->config.daylightOffset_sec= atol( (char *) sqlite3_column_text(this->result, 1) );
        this->interface->config.NTP_request_interval= atol( (char *) sqlite3_column_text(this->result, 1) );
    }


   sqlite3_close(this->db_settings);
};

// ****************************************************

void SETTINGS_CLASS::query(String query){
    /*
    CREATE TABLE "sondas" ( "id" INTEGER UNIQUE, "fecha" TEXT, "dispositivo" TEXT, "CPU" REAL,"Temperatura" REAL,"Memoria" INTEGER, PRIMARY KEY("id" AUTOINCREMENT) );
    */
   int rc = this->db_exec(this->db_settings, query.c_str()); // "INSERT INTO settings VALUES (1, 'Hello, World from test1');"
   if (rc != SQLITE_OK) {
       sqlite3_close(this->db_settings);
       return;
   }
};

// ****************************************************

int SETTINGS_CLASS::db_open(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       this->mserial->printStrln("Can't open database: " + String(sqlite3_errmsg(*db)));
       return rc;
   } else {
       this->mserial->printStrln("settings.db ppened successfully");
   }
   return rc;
}

// ****************************************************

int SETTINGS_CLASS::db_exec(sqlite3 *db, const char *sql) {
    char *zErrMsg = 0;
    this->mserial->printStrln(sql);
    int rc = sqlite3_exec(db, sql, NULL, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        this->mserial->printStrln("SQL error: " +String(zErrMsg));
        sqlite3_free(zErrMsg);
    } else {
        this->mserial->printStrln("Operation done successfully");
    }
    return rc;
}

// *********************************************************

/*
const char* data = "Callback function called";
static int SETTINGS_CLASS::callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}
*/