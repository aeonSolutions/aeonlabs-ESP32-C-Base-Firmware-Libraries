
#include "m_atsha204.h"
#include "security/sha204_i2c.h"

// *************************************************************
String CryptoGetRandom(INTERFACE_CLASS* interface){
  uint8_t response[32];
  uint8_t returnValue;
  interface->sha204.simpleWakeup();
  returnValue = interface->sha204.simpleGetRandom(response);
  interface->sha204.simpleSleep();  
  return hexDump(response, sizeof(response));
}

//*****************************************************************
String CryptoICserialNumber(INTERFACE_CLASS* interface){
  uint8_t serialNumber[6];
  uint8_t returnValue;

  interface->sha204.simpleWakeup();
  returnValue = interface->sha204.simpleGetSerialNumber(serialNumber);
  interface->sha204.simpleSleep();
  
  return hexDump(serialNumber, sizeof(serialNumber));
}

//******************************************************************************
String macChallengeDataAuthenticity(INTERFACE_CLASS* interface, String text ){
  static uint32_t n = 0;
  uint8_t command[MAC_COUNT_LONG];
  uint8_t response[MAC_RSP_SIZE];

  int str_len = text.length() + 1;
  char text_char [str_len];
  text.toCharArray(text_char, str_len);
        
  uint8_t challenge[sizeof(text_char)] = {0};

  sprintf((char *)challenge, text_char, n++);

  interface->sha204.simpleWakeup();
  uint8_t ret_code = interface->sha204.sha204m_execute(SHA204_MAC, 0, 0, MAC_CHALLENGE_SIZE, 
    (uint8_t *) challenge, 0, NULL, 0, NULL, sizeof(command), &command[0], 
    sizeof(response), &response[0]);
  
  interface->sha204.simpleSleep(); 

  if (ret_code != SHA204_SUCCESS){
    interface->mserial->printStrln("simpleMac failed.");
    return "Error";
  }

  return hexDump(response, sizeof(response));
}

//***********************************************************************************
String macChallengeDataAuthenticityOffLine(INTERFACE_CLASS* interface, char dataRow[] ){
  static uint32_t n = 0;
  uint8_t mac[32];
  uint8_t challenge[sizeof(dataRow)] = {0}; // MAC_CHALLENGE_SIZE
  
  sprintf((char *)challenge, dataRow, n++);
  interface->mserial->printStr("challenge: ");
  interface->mserial->printStrln((char *)challenge);
  
  uint8_t key[32];
  //Change your key here.
  hex2bin(interface->DATA_VALIDATION_KEY, key);
  uint8_t mac_offline[32];
  interface->sha204.simpleWakeup();
  int ret_code = interface->sha204.simpleMacOffline(challenge, mac_offline, key);
  interface->sha204.simpleSleep();
  interface->mserial->printStr("MAC Offline:\n");
  return hexDump(mac_offline, sizeof(mac_offline));
}

//*********************************************************************
byte macChallengeExample(INTERFACE_CLASS* interface){
  uint8_t command[MAC_COUNT_LONG];
  uint8_t response[MAC_RSP_SIZE];
  char buffer[3];

  const uint8_t challenge[MAC_CHALLENGE_SIZE] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
  };

  uint8_t ret_code = interface->sha204.sha204m_execute(SHA204_MAC, 0, 0, MAC_CHALLENGE_SIZE, 
    (uint8_t *) challenge, 0, NULL, 0, NULL, sizeof(command), &command[0], 
    sizeof(response), &response[0]);

  for (int i=0; i<SHA204_RSP_SIZE_MAX; i++){
    snprintf(buffer, sizeof(buffer), "%02X", response[i]);
    interface->mserial->printStr( String(buffer) + " " );

  }
  
  interface->mserial->printStrln("\n Response Code (" + String(ret_code) + ") " + ErrorCodeMessage(ret_code) );

  return ret_code;
}

//***************************************************************************
uint8_t wakeupExample(INTERFACE_CLASS* interface){
  uint8_t response[SHA204_RSP_SIZE_MIN];
  uint8_t returnValue;
  char buffer[3];
  
  returnValue = interface->sha204.sha204c_wakeup( &response[0] );
  for (int i=0; i<SHA204_RSP_SIZE_MIN; i++){
    snprintf(buffer, sizeof(buffer), "%02X", response[i]);
    interface->mserial->printStr( String(buffer)  + " ");

  }
   interface->mserial->printStrln("");
  
  return returnValue;
}

// ******************************************************************
uint8_t serialNumberExample(INTERFACE_CLASS* interface){
  uint8_t serialNumber[9];
  uint8_t returnValue;
  char buffer[3];

  returnValue = interface->sha204.getSerialNumber(serialNumber);
  for (int i=0; i<9; i++){    
    snprintf(buffer, sizeof(buffer), "%02X", serialNumber[i]);
    interface->mserial->printStr( String(buffer)  + " ");

  }
   interface->mserial->printStrln("");
  
  return returnValue;
}
// ******************************************************************
uint8_t randomExample(INTERFACE_CLASS* interface){
  uint8_t response[32];
  uint8_t returnValue;
  char buffer[65];

  returnValue = interface->sha204.simpleGetRandom(response);
  snprintf(buffer, sizeof(buffer), "%02X", response);
  interface->mserial->printStr( String(buffer)  + " ");

  return returnValue;
}

//******************************************************
void runFingerPrintIDtests(INTERFACE_CLASS* interface){
    interface->mserial->printStrln("Testing the Unique FingerPrind ID for Sensor Data Measurements ============================");

    interface->mserial->printStrln("\nSending a Wakup Command. Response should be:\r\n4 11 33 43:");
    interface->mserial->printStrln("Response is:");
    wakeupExample(interface);

    interface->mserial->printStrln("\nAsking the SHA204's serial number. Response should be:");
    interface->mserial->printStrln("01 23 xx xx xx xx xx xx xx EE");
    interface->mserial->printStrln("Response is:");
    serialNumberExample(interface);

    interface->mserial->printStrln("\nTesting Random Genenator: ");
    randomExample(interface);

    interface->mserial->printStrln("\nSending a MAC Challenge. Response should be:");
    interface->mserial->printStrln("23 06 67 00 4F 28 4D 6E 98 62 04 F4 60 A3 E8 75 8A 59 85 A6 79 96 C4 8A 88 46 43 4E B3 DB 58 A4 FB E5 73");
    interface->mserial->printStrln("Response is:");
    if (macChallengeExample(interface) == 0x00 ){
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
      interface->onBoardLED->statusLED(100, 1);
    }else{
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100, 2);
    }
    interface->mserial->printStrln("====================== done ======================================");
   // mserial->printStrln("Testing Sensor Data Validation hashing");
   // mserial->printStrln( macChallengeDataAuthenticity(interface, "TEST IC"));
   // mserial->printStrln("");
}

String ErrorCodeMessage(uint8_t code){
  switch(code) {
    case 0x00:
      return "Function succeeded.";
      break;
    case 0x02:
      return "response status byte indicates parsing error";
      break;
    case 0x03:
      return "response status byte indicates command execution error";
      break;
    case 0x04:
      return "response status byte indicates CRC error";
      break;
    case 0x05:
      return "response status byte is unknown";
      break;

    case 0xE0:
      return "Function could not execute due to incorrect condition / state.";
      break;
    case 0xE1:
      return "unspecified error";
      break;
    case 0xE2:
      return "bad argument (out of range, null pointer, etc.)";
      break;
    case 0xE3:
      return "invalid device id, id not set";
      break;
    case 0xE4:
      return "Count value is out of range or greater than buffer size";
      break;
    case 0xE5:
      return "incorrect CRC received";
      break;
    case 0xE6:
      return "Timed out while waiting for response. Number of bytes received is > 0.";
      break;
    case 0xE7:
      return "Not an error while the Command layer is polling for a command response.";
      break;
    case 0xE8:
      return "re-synchronization succeeded, but only after generating a Wake-up";
      break;
    case 0xF0:
      return "Communication with device failed.";
      break;
    case 0xF1:
      return "Timed out while waiting for response. Number of bytes received is 0.";
      break;
      
    default:
      return "error code not found";
      break;
  }

}