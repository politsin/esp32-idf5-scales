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
  step.config(&stepConfig);
  step.init();
  step.setSpeed(10000, 1000, 1000);
  step.setStepsPerMm(10);
  // dStep.init(dConfig.step_p, dConfig.dir_p, dConfig.en_p, dConfig.timer_group,
  //            dConfig.timer_idx, MICROSTEP_1, 200);
  // uint16_t accT = 1;
  // uint16_t speed = 10;
  // bool dir = true;
  // vTaskDelay(pdMS_TO_TICKS(250));
  // if (encoder_counter > 0) {
  //   dir = true;
  // } else {
  //   dir = false;
  // }
  // for (size_t i = 0; i < abs(encoder_counter); i++) {
  //   dStep.setSpeed(speed * i, accT);
  //   dStep.runPermanent(dir);
  //   ESP_LOGI(STEPPER_TAG, "ACC: %d", i);
  //   pdMS_TO_TICKS(100);
  // }
  // dStep.setSpeed(speed * abs(encoder_counter), accT);
  // dStep.runPermanent(dir);
  uint32_t notify_value;
  // uint32_t mode;
  // int16_t value;
  // step.runInf(true);
  ESP_LOGW(STEPPER_TAG, "stepper!");
  while (true) {
    if (xTaskNotifyWait(0, 0, &notify_value, 0) == pdTRUE) {
      ESP_LOGI(STEPPER_TAG, "run");
      step.runPos(10000);
      ESP_LOGI(STEPPER_TAG, "done");
      // mode = notify_value & 0xff;
      // value = (int16_t)(notify_value >> 16);
      // switch (mode) {
      // case 1:
      //   ESP_LOGI(STEPPER_TAG, "ENC: %d", value);
      //   if (value != 0) {
      //     dStep.setSpeed(speed * abs(value), accT);
      //   }
      //   if (value > 0) {
      //     dir = true;
      //   } else {
      //     dir = false;
      //   }
      //   break;
      // case 2:
      //   ESP_LOGI(STEPPER_TAG, "ENC_BTN: %d", value);
      //   break;
      // default:
      //   ESP_LOGI(STEPPER_TAG, "enc! %d %x", value, mode);
      //   break;
      // }
    }
    // dStep.runPermanent(dir);
    // if (dStep.getState() <= IDLE) {
    //   dStep.runAbsolute(esp_random() >> 18);
    // }
    // step.runPos(10000);
    vTaskDelay(xBlockTime);
  }
}
