// #include <stepperTask.h> WTF o.O
// stepper #22 #19.
#include "DendoStepper.h"
// #include "configTask.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "driver/gpio.h"
#include "driver/uart.h"
#include <esp_err.h>
#include <esp_log.h>
#include <hal/gpio_types.h>
#include <hal/uart_types.h>
#include <rom/gpio.h>
#include <stdio.h>

#include "main.h"
#include <iostream>
using std::string;
using std::to_string;
#define STEPPER_TAG "STEPPER"

DendoStepper_config_t stepConfig = {.stepPin = (uint8_t)GPIO_NUM_13,
                                    .dirPin = (uint8_t)GPIO_NUM_15,
                                    .enPin = (uint8_t)GPIO_NUM_2,
                                    .miStep = MICROSTEP_32,
                                    .stepAngle = 1.8};

DendoStepper step = DendoStepper();

TaskHandle_t stepper;
void stepperTask(void *pvParam) {
  static const TickType_t xBlockTime = pdMS_TO_TICKS(50);
  static const TickType_t xOffTime = pdMS_TO_TICKS(5000);
  step.config(&stepConfig);
  step.init();
  step.setSpeed(10000, 1000, 1000);
  step.setStepsPerMm(10);
  step.disableMotor();

  uint32_t notify_value;
  uint32_t mode;
  int16_t value;
  // step.runInf(true);
  int32_t encoder;
  ESP_LOGW(STEPPER_TAG, "stepper!");
  while (true) {
    if (xTaskNotifyWait(0, 0, &notify_value, 0) == pdTRUE) {
      encoder = (int32_t)notify_value;
      if (notify_value == 5000) {
        ESP_LOGI(STEPPER_TAG, "run");
        step.enableMotor();
        step.runPos(10000);
        // step.runPos(-500);
        // step.stop();
        vTaskDelay(xOffTime);
        step.disableMotor();
        ESP_LOGI(STEPPER_TAG, "done");
      }
      else if (notify_value == 5001) {
        ESP_LOGI(STEPPER_TAG, "Encoder STOP");
        step.enableMotor();
        step.stop(); 
        step.runPos(-1000);
        vTaskDelay(xOffTime);
        step.disableMotor();
        // step.stop();
      } else {
        ESP_LOGI(STEPPER_TAG, "Encoder diff %ld", encoder);
      }
      // Old staff;
      mode = notify_value & 0xff;
      value = (int16_t)(notify_value >> 16);
      switch (mode) {
      case 1:
        ESP_LOGI(STEPPER_TAG, "ENC: %d", value);
        break;
      case 2:
        // ESP_LOGI(STEPPER_TAG, "ENC_BTN: %d", value);
        break;
      default:
        // ESP_LOGI(STEPPER_TAG, "enc! %d %x", value, mode);
        break;
      }
    }
    vTaskDelay(xBlockTime);
  }
}
