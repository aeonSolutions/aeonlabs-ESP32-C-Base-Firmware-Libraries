#include <Arduino.h>
#include "time.h"
#include "ESP32Time.h"

#ifndef  M_FILE_FUNCTIONS_DEF
  #define  M_FILE_FUNCTIONS_DEF

    tm CompileDateTime(char const *dateStr, char const *timeStr);

    bool isStringAllSpaces(String str);

    String addThousandSeparators(std::string value, char thousandSep = ',', char decimalSep = '.', char sourceDecimalSep = '.');
  #endif