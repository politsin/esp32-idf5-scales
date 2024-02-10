#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <rom/gpio.h>
#include <stdio.h>

#define MAINTAG "MAIN"

// tasks
#include "task/uartTask.h"
#include "task/blinkTask.h"
#include "task/hx711Task.h"
#include "task/buttonTask.h"
#include "task/encoderTask.h"
// #include "screen/screenTask.h"
// #include "task/stepperTask.h"

// Loop Task
void loop(void *pvParameter);

extern "C" void app_main(void) {
  esp_log_level_set("wifi", ESP_LOG_WARN);
  esp_log_level_set("gpio", ESP_LOG_WARN);
  ESP_LOGW(MAINTAG, "Hello world!!");
  uint32_t min = 768 + configSTACK_OVERHEAD_TOTAL;

  // tasks.
  xTaskCreate(&loop, "loop", min * 3, NULL, 2, NULL);
  xTaskCreate(uartTask, "uart", min * 10, NULL, 1, &uart);
  xTaskCreate(hx711Task, "hx711", min * 16, NULL, 1, &hx711);
  xTaskCreate(blinkTask, "blink", min * 2, NULL, 1, &blink);
  xTaskCreate(buttonTask, "button", min * 4, NULL, 1, &button);
  xTaskCreate(encoderTask, "encoder", min * 6, NULL, 1, &encoder);
  // xTaskCreate(screenTask, "screen", min * 64, NULL, 1, &screen);
}

void loop(void *pvParameter) {
  uint32_t size = 0;
  uint32_t count = 0;
  const TickType_t xBlockTime = pdMS_TO_TICKS(3 * 1000);
  while (1) {
    count++;
    if ((count % 100) == true) {
      size = xPortGetFreeHeapSize();
      ESP_LOGW(MAINTAG, "xP %ld", size);
      vTaskDelay(xBlockTime);
    }
  }
}
