#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <rom/gpio.h>
#include <stdio.h>

#include <main.h>
#define MAINTAG "MAIN"

// tasks
#include "task/uartTask.h"
#include "task/blinkTask.h"
#include "task/hx711Task.h"
#include "task/buttonTask.h"
#include "task/screenTask.h"
#include "task/encoderTask.h"

app_data_t app_data = {
    .btn_enc = 0,
    .btn_red = 0,
    .btn_blue = 0,
    .encoder = 0,
    .raw = 0,
    .scale = 0,
    .xp = xPortGetFreeHeapSize(),
    .k = 278,
};

extern "C" void app_main(void) {
  esp_log_level_set("wifi", ESP_LOG_WARN);
  esp_log_level_set("gpio", ESP_LOG_WARN);
  ESP_LOGW(MAINTAG, "Hello world!!");
  uint32_t min = 768 + configSTACK_OVERHEAD_TOTAL;

  // tasks.
  xTaskCreate(&loop, "loop", min * 3, NULL, 2, NULL);
  xTaskCreate(hx711Task, "hx711", min * 16, NULL, 1, &hx711);
  xTaskCreate(blinkTask, "blink", min * 4, NULL, 1, &blink);
  xTaskCreate(screenTask, "screen", min * 8, NULL, 1, &screen);
  xTaskCreate(buttonTask, "button", min * 4, NULL, 1, &button);
  xTaskCreate(encoderTask, "encoder", min * 6, NULL, 1, &encoder);
  xTaskCreatePinnedToCore(uartTask, "uart", min * 10, NULL, 1, &uart, 0);
  // xTaskCreate(screenTask, "screen", min * 64, NULL, 1, &screen);
}

// Loop Task
void loop(void *pvParameter) {
  uint32_t size = 0;
  uint32_t count = 0;
  const TickType_t xBlockTime = pdMS_TO_TICKS(5 * 1000);
  while (1) {
    count++;
    if ((count % 100) == true) {
      size = xPortGetFreeHeapSize();
      app_data.xp = size;
      // ESP_LOGW(MAINTAG, "xP %ld", size);
      vTaskDelay(xBlockTime);
    }
  }
}
