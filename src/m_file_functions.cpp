#include "m_file_functions.h"

// Convert compile time to Time Struct
tm CompileDateTime(char const *dateStr, char const *timeStr){
    char s_month[5];
    int year;
    struct tm t;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(dateStr, "%s %d %d", s_month, &t.tm_mday, &year);
    sscanf(timeStr, "%2d %*c %2d %*c %2d", &t.tm_hour, &t.tm_min, &t.tm_sec);
    // Find where is s_month in month_names. Deduce month value.
    t.tm_mon = (strstr(month_names, s_month) - month_names) / 3;    
    t.tm_year = year - 1900;    
    return t;
}

// **************************************************************

bool isStringAllSpaces(String str){
  for (int i = 0; i < str.length(); i++){
    if ( str.charAt(i) != ' ' )
      return false;
  }
  return true;
}


// **************************************************************
/* https://stackoverflow.com/a/54336178/13794189 */

String addThousandSeparators(std::string value, char thousandSep, char decimalSep , char sourceDecimalSep){
    int len = value.length();
    int negative = ((len && value[0] == '-') ? 1: 0);
    int dpos = value.find_last_of(sourceDecimalSep);
    int dlen = 3 + (dpos == std::string::npos ? 0 : (len - dpos));

    if (dpos != std::string::npos && decimalSep != sourceDecimalSep) {
        value[dpos] = decimalSep;
    }

    while ((len - negative) > dlen) {
        value.insert(len - dlen, 1, thousandSep);
        dlen += 4;
        len += 1;
    }
    return String(value.c_str() );
}
