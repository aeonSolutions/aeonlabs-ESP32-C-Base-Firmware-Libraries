#include "FFat.h"
#include "Files.h"

char* readFromFile(const char* filename) {
  File file = FFat.open(filename, "r");
  if (!file) {
    char* result = (char*) malloc(sizeof(char));
    result[0] = '\0';
    return result;
  }
  char* result = (char*) malloc(file.size() + sizeof(char));
  int i;
  for (i = 0; file.available(); i++) {
      result[i] = (char) file.read();
  }
  result[i] = '\0';
  file.close();
  return result;
}

bool writeToFile(const char* filename, char content[]) {  
  File file = FFat.open(filename, "w");
  if (!file) {
   Serial.print("Error writing "+ String(filename)); 
   return false;
  }
  
  if (file.print(content) == 0) {
   Serial.print("Error writing "+ String(filename)); 
   return false;
  }
  
  file.close();
  return true;
}
