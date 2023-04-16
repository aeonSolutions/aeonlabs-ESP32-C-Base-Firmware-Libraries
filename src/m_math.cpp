#include "m_math.h"

float roundFloat(float value, int decimal_places) {
    const float multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

// --------------------------------------------------------------------------

boolean isNumeric(String str) {
    unsigned int stringLength = str.length();
 
    if (stringLength == 0) {
        return false;
    }
 
    boolean seenDecimal = false;
 
    for(unsigned int i = 0; i < stringLength; ++i) {
        if (isDigit(str.charAt(i))) {
            continue;
        }
 
        if (str.charAt(i) == '.') {
            if (seenDecimal) {
                return false;
            }
            seenDecimal = true;
            continue;
        }
        return false;
    }
    return true;
}

// --------------------------------------------------------

String hexDump(uint8_t *data, uint32_t length){
  char buffer[3];
  String hexStr="0x";
  for (uint32_t i = 0; i < length; i++){    
    snprintf(buffer, sizeof(buffer), "%02X", data[i]);
    hexStr += buffer;
  }
  return hexStr;
}

int char2int(char input){
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
void hex2bin(const char *src, uint8_t *target){
  while (*src && src[1]){
    *(target++) = char2int(*src) * 16 + char2int(src[1]);
    src += 2;
  }
}