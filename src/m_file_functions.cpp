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