#include <Arduino.h>
#include "m_math.h"
#include "security/sha204_i2c.h"
#include "interface_class.h"

#ifndef ATSHA204_DEF
  #define ATSHA204_DEF

  String CryptoGetRandom(INTERFACE_CLASS* interface);
  String CryptoICserialNumber(INTERFACE_CLASS* interface);

  String macChallengeDataAuthenticity(INTERFACE_CLASS* interface, String text );
  String macChallengeDataAuthenticityOffLine(INTERFACE_CLASS* interface, char dataRow[] );

  uint8_t macChallengeExample(INTERFACE_CLASS* interface);
  uint8_t wakeupExample(INTERFACE_CLASS* interface);
  uint8_t serialNumberExample(INTERFACE_CLASS* interface);
  uint8_t randomExample(INTERFACE_CLASS* interface);
  void runFingerPrintIDtests(INTERFACE_CLASS* interface);
  String ErrorCodeMessage(uint8_t code);

#endif