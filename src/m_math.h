#include <Arduino.h>

#ifndef EXTENDED_MATH_DEF
  #define EXTENDED_MATH_DEF

    float roundFloat(float value, int decimal_places);
    
    boolean isNumeric(String str); 

    String hexDump(uint8_t *data, uint32_t length);

    int char2int(char input);

    void hex2bin(const char *src, uint8_t *target);

  #endif