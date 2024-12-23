// put function declarations here:
#include "M5CoreS3.h"
#include <SPIFFS.h>
#include <M5Unified.h>

void play_wave(const char* file_name) {
  File file = SPIFFS.open(file_name, "r");
  if (!file) {
    log_e("Failed to open file: %s", file_name);
    return;
  }

  size_t fileSize = file.size();
  uint8_t* buffer = (uint8_t*)malloc(fileSize);
  if (!buffer) {
    log_e("Failed to allocate memory");
    file.close();
    return;
  }

  size_t bytesRead = file.read(buffer, fileSize);
  if (bytesRead != fileSize) {
    log_e("Failed to read file completely");
    free(buffer);
    file.close();
    return;
  }

  M5.Speaker.playWav(buffer, fileSize, 1, 1);
  
  free(buffer);
  file.close();
}