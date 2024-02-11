// #include "uartTask.h"
// #include "screenTask.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
// #include <freertos/queue.h>
#include <freertos/task.h>
#include <math.h>
// Task:.
#include <hx711.h>
#include <hx711Task.h>
// #include "screenTask.h"
// using std::string;
// 10  =  10300
// 20  =  20500
// 30  =  30900
// 40  =  41200
// 50  =  51400
// 60  =  61600
// 70  =  72000
// 80  =  82300
// 90  =  92500
// 100 = 102800
#define PD_SCK_GPIO GPIO_NUM_27
#define DOUT_GPIO GPIO_NUM_26

static const char *hx711TAG = "hx711";
TaskHandle_t hx711;
void hx711Task(void *pvParam) {
  const TickType_t xBlockTime = pdMS_TO_TICKS(1000);
  hx711_t dev = {
      .dout = DOUT_GPIO,
      .pd_sck = PD_SCK_GPIO,
      .gain = HX711_GAIN_A_128,
  };
  while (1) {
    esp_err_t r = hx711_init(&dev);
    if (r == ESP_OK) {
      break;
    }
    ESP_LOGE(hx711TAG, "Could not initialize HX711: %d (%s)", r,
             esp_err_to_name(r));
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  int32_t raw = 0;
  int32_t tare = 0;
  uint32_t notify_value;
  raw = scale(dev);
  if (raw != 0) {
    tare = raw;
  }
  int32_t weight = 0;
  int32_t k = 1150;
  while (true) {
    raw = scale(dev);
    if (raw) {
      weight = k * (raw - tare) / 10000;
      // xTaskNotify(screen, (uint32_t)weight, eSetValueWithOverwrite);
      // if (weight < 0) {
      //   weight = 0;
      // }
      // xTaskNotify(mcp23x17, (uint32_t)weight, eSetValueWithOverwrite);
    }
    if (xTaskNotifyWait(0, 0, &notify_value, 0) == pdTRUE) {
      tare = raw;
    }
    // ESP_LOGI(hx711TAG, "Scale: %ld, Raw delta: %ld", weight, raw - tare);
    vTaskDelay(xBlockTime);
  }
}

int32_t scale(hx711_t dev) {
  int32_t scale = 0;
  uint16_t oversampling = 1;
  for (uint32_t i = 0; i < pow(2, oversampling); i++) {
    esp_err_t r = hx711_wait(&dev, 80);
    if (r != ESP_OK) {
      ESP_LOGI(hx711TAG, "Device not found: %d (%s)", r, esp_err_to_name(r));
      return 0;
    }
    static int32_t data = 0;
    r = hx711_read_data(&dev, &data);
    if (r != ESP_OK) {
      ESP_LOGI(hx711TAG, "Could not read data: %d (%s)", r, esp_err_to_name(r));
      return 0;
    }
    scale = scale + data;
  }
  scale = scale >> oversampling;
  // ESP_LOGI(hx711TAG, "raw: %ld", scale);

  return -scale;
}
