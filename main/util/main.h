#include <iostream>
using std::string;

#include "driver/gpio.h"
typedef gpio_num_t Pintype;

void mqtt_callback(string param, string value);

void loop(void *pvParameter);

#ifndef APP_MAIN_H_
#define APP_MAIN_H_
typedef struct {
  uint8_t btn_enc;
  uint8_t btn_red;
  uint8_t btn_blue;
  int32_t encoder;
  int32_t raw;
  int32_t scale;
  uint32_t xp;
} app_data_t;
#endif /* APP_MAIN_H_ */

extern app_data_t app_data;
// i2c::
// void icScan();
// #define SCL_GPIO 25
// #define SDA_GPIO 33

