#ifndef DATAVERSE_CLASS_DEF
  #define DATAVERSE_CLASS_DEF
  #include <Arduino.h>

  #include "mserial.h"
  #include "interface_class.h"
  #include "onboard_sensors.h"
  #include "FFat.h"
  #include <ArduinoJson.h>
  #include "m_wifi.h"

  class DATAVERSE_CLASS {

  private:
    INTERFACE_CLASS* interface;
    M_WIFI_CLASS* mWifi;
    mSerial* mserial;

    String DATASET_REPOSITORY_URL; 

  public:
        typedef struct{
            String API_TOKEN ;
            String API_TOKEN_EXPIRATION_DATE;

            String SERVER_URL;
            int SERVER_PORT;
            String PERSISTENT_ID ; 
            String DATASET_ID;
            bool UPLOAD_DATAVERSE_EN;
            unsigned long UPLOAD_DATASET_DELTA_TIME;

            String EXPERIMENTAL_DATA_FILENAME;
        } config_strut;

        config_strut config;

        long int $espunixtimePrev;

        StaticJsonDocument <2*2463+ 14*JSON_ARRAY_SIZE(1) + 122*JSON_OBJECT_SIZE(1) > datasetInfoJson;
        
        constexpr static char* HARVARD_ROOT_CA_RSA_SHA1 PROGMEM = \
        "-----BEGIN CERTIFICATE-----\n" \
        "MIIEMjCCAxqgAwIBAgIBATANBgkqhkiG9w0BAQUFADB7MQswCQYDVQQGEwJHQjEb\n" \
        "MBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYDVQQHDAdTYWxmb3JkMRow\n" \
        "GAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UEAwwYQUFBIENlcnRpZmlj\n" \
        "YXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4MTIzMTIzNTk1OVowezEL\n" \
        "MAkGA1UEBhMCR0IxGzAZBgNVBAgMEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n" \
        "BwwHU2FsZm9yZDEaMBgGA1UECgwRQ29tb2RvIENBIExpbWl0ZWQxITAfBgNVBAMM\n" \
        "GEFBQSBDZXJ0aWZpY2F0ZSBTZXJ2aWNlczCCASIwDQYJKoZIhvcNAQEBBQADggEP\n" \
        "ADCCAQoCggEBAL5AnfRu4ep2hxxNRUSOvkbIgwadwSr+GB+O5AL686tdUIoWMQua\n" \
        "BtDFcCLNSS1UY8y2bmhGC1Pqy0wkwLxyTurxFa70VJoSCsN6sjNg4tqJVfMiWPPe\n" \
        "3M/vg4aijJRPn2jymJBGhCfHdr/jzDUsi14HZGWCwEiwqJH5YZ92IFCokcdmtet4\n" \
        "YgNW8IoaE+oxox6gmf049vYnMlhvB/VruPsUK6+3qszWY19zjNoFmag4qMsXeDZR\n" \
        "rOme9Hg6jc8P2ULimAyrL58OAd7vn5lJ8S3frHRNG5i1R8XlKdH5kBjHYpy+g8cm\n" \
        "ez6KJcfA3Z3mNWgQIJ2P2N7Sw4ScDV7oL8kCAwEAAaOBwDCBvTAdBgNVHQ4EFgQU\n" \
        "oBEKIz6W8Qfs4q8p74Klf9AwpLQwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQF\n" \
        "MAMBAf8wewYDVR0fBHQwcjA4oDagNIYyaHR0cDovL2NybC5jb21vZG9jYS5jb20v\n" \
        "QUFBQ2VydGlmaWNhdGVTZXJ2aWNlcy5jcmwwNqA0oDKGMGh0dHA6Ly9jcmwuY29t\n" \
        "b2RvLm5ldC9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDANBgkqhkiG9w0BAQUF\n" \
        "AAOCAQEACFb8AvCb6P+k+tZ7xkSAzk/ExfYAWMymtrwUSWgEdujm7l3sAg9g1o1Q\n" \
        "GE8mTgHj5rCl7r+8dFRBv/38ErjHT1r0iWAFf2C3BUrz9vHCv8S5dIa2LX1rzNLz\n" \
        "Rt0vxuBqw8M0Ayx9lt1awg6nCpnBBYurDC/zXDrPbDdVCYfeU0BsWO/8tqtlbgT2\n" \
        "G9w84FoVxp7Z8VlIMCFlA2zs6SFz7JsDoeA3raAVGI/6ugLOpyypEBMs1OUIJqsi\n" \
        "l2D4kF501KKaU73yqWjgom7C12yxow+ev+to51byrvLjKzg6CYG1a4XXvi3tPxq3\n" \
        "smPi9WIsgtRqAEFQ8TmDn5XpNpaYbg==\n" \
        "-----END CERTIFICATE-----\n";


    DATAVERSE_CLASS();

    void init(INTERFACE_CLASS* interface,  M_WIFI_CLASS* mWifi, mSerial* mserial);
    
    bool readSettings(fs::FS &fs=FFat);
    bool saveSettings(fs::FS &fs=FFat);

    void UploadToDataverse(bool ble_connected);
    String GetInfoFromDataverse(String url);
    void getDatasetMetadata();

    bool gbrl_commands(String $BLE_CMD, uint8_t sendTo);
    bool helpCommands(String $BLE_CMD, uint8_t sendTo);
    
  };

#endif