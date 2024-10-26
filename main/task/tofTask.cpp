// TOF 0x39 - TOF AP3216
#include "sdkconfig.h"
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>
#define TOF_TAG "TOF"

TaskHandle_t tof;
void tofTask(void *pvParam) {
  // tof_init(&dev);
  // tof_init_desc(&dev, I2C_NUM_0, (gpio_num_t)33, (gpio_num_t)25);
  const TickType_t xBlockTime = pdMS_TO_TICKS(500);
  esp_err_t res;
  int addr = 0x39;
  // TOF? init
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, 1);
  i2c_master_stop(cmd);
  res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
  if (res == 0)
    ESP_LOGI(TOF_TAG, "TOF [%.2x] - OK", addr);
  else
    ESP_LOGE(TOF_TAG, "TOF [%.2x] - FAIL", addr);
  i2c_cmd_link_delete(cmd);
  // TOF done
  while (true) {
    if (true) {
      ESP_LOGI(TOF_TAG, "tof!");

    }
    vTaskDelay(xBlockTime);
  }
}
