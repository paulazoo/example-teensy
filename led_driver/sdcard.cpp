#include "sdcard.h"


bool readIntArrayFromSD(const char* filename, uint32_t* arr, uint32_t* size, uint32_t max_size) {
  File file = SD.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file.");
    return false;
  }

  String token = "";
  bool size_read = false;
  uint32_t index = 0;
  *size = 0;

  while (file.available()) {
    char c = file.read();

    // Split on common separators
    if (c == ',' || c == ' ' || c == '\n' || c == '\r' || c == '\t') {
      if (token.length() > 0) {
        int token_value = token.toInt(); // any other values just become 0
        if (token_value < 0) token_value = 0; // clamp negative values to 0
        uint32_t value = static_cast<uint32_t>(token_value);
        token = "";

        if (!size_read) {
          *size = value;
          size_read = true;

          if (*size > max_size) {
            Serial.println("Invalid array size in file.");
            file.close();
            return false;
          }
        } else if (index < *size) {
          arr[index++] = value;
        } else {
          Serial.println("More integers in file than expected based on size.");
          return false;
        }
      }
    } else {
      token += c;
    }
  }

  // Handle last number if file does not end with separator
  if (token.length() > 0) {
    int token_value = token.toInt(); // any other values just become 0
    if (token_value < 0) token_value = 0; // clamp negative values to 0
    uint32_t value = static_cast<uint32_t>(token_value);
    token = "";

    if (!size_read) {
      *size = value;
      size_read = true;

      if (*size > max_size) {
        Serial.println("Invalid array size in file.");
        file.close();
        return false;
      }
    } else if (index < *size) {
      arr[index++] = value;
    } else {
      Serial.println("More integers in file than expected based on size.");
      return false;
    }
  } 

  file.close();

  if (!size_read) {
    Serial.println("No size found in file.");
    return false;
  }

  return true;
}


bool readMultipleIntArraysFromSD(const char* filenames[], uint32_t* arrays[], uint32_t* sizes[], uint32_t max_size, uint32_t num_files) {
  bool success = true;

  for (uint32_t i = 0; i < num_files; i++) {
    if (!readIntArrayFromSD(filenames[i], arrays[i], sizes[i], max_size)) {
      Serial.print("Failed to read array from ");
      Serial.println(filenames[i]);
      success = false;
    }
  }

  return success;
}