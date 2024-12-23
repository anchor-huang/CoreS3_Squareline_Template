#include <Arduino.h>
#include <M5CoreS3.h>
#include "m5gfx_lvgl.hpp"
#include <nvs_flash.h>
#include <SPIFFS.h>
#include <UI/ui.h>
#include "wifi_config.hpp"

#define LVGL_TASK_CORE 1  // Pin to core 1
#define LVGL_TASK_PRIORITY 5
#define LVGL_STACK_SIZE 32768



void lvgl_task(void *pvParameters) {
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void setup() {
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);
  log_i("M5Stack initialized");

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
  }
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)) {
    log_e("SPIFFS initialization failed!");  
  }

  // Step 2: Initialize the display subsystem
  //M5.Display.setBrightness(60);
  lv_init();
  m5gfx_lvgl_init();

  ui_init();
  xTaskCreatePinnedToCore(
        lvgl_task,          // Task function
        "lvgl",             // Task name
        LVGL_STACK_SIZE,    // Stack size
        NULL,               // Parameters
        LVGL_TASK_PRIORITY, // Priority
        NULL,               // Task handle
        LVGL_TASK_CORE     // Core ID
  );
  WifiConfig::setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  // lv_timer_handler();
  // vTaskDelay(1);
  WifiConfig::loop();
}
