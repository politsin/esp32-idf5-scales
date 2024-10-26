// #include <counterTask.h> WTF o.O
// Counter #22 #19.
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
typedef gpio_num_t Pintype;
static constexpr Pintype DI = GPIO_NUM_26;
static constexpr Pintype AI = GPIO_NUM_27;
#include "sdkconfig.h"
#include <esp_log.h>
#include <rom/gpio.h>
#define COUNTER_TAG "COUNTER"

TaskHandle_t counter;

// Объявляем счетчик как глобальную переменную
volatile int rot = 0;

// Функция обработки прерывания
static void IRAM_ATTR gpio_isr_handler(void *arg) {
  // Увеличиваем счетчик при каждом прерывании
  rot++;
}

void counterTask(void *pvParam) {
  gpio_pad_select_gpio(DI);
  gpio_set_direction(DI, GPIO_MODE_INPUT);

  gpio_pad_select_gpio(AI);
  gpio_set_direction(AI, GPIO_MODE_OUTPUT);

  // Настраиваем прерывание по нарастающему фронту
  gpio_set_intr_type(DI, GPIO_INTR_POSEDGE);

  // Устанавливаем обработчик прерывания
  gpio_install_isr_service(0);
  gpio_isr_handler_add(DI, gpio_isr_handler, NULL);
  const TickType_t xBlockTime = pdMS_TO_TICKS(50);

  uint32_t i = 0;
  uint32_t notify_value;
  gpio_set_level(AI, 0);
  while (true) {
    if (true) {
      vTaskDelay(xBlockTime * 1000000);
    }
    if (xTaskNotifyWait(0, 0, &notify_value, 0) == pdTRUE) {
      if (notify_value == 5000) {
        ESP_LOGI(COUNTER_TAG, "blue btn");
        // AI ON
        gpio_set_level(AI, 1);
      }
      else if(notify_value == 5001) { 
        ESP_LOGI(COUNTER_TAG, "red btn");
        // AI OFF
        gpio_set_level(AI, 0);
      } else {
        //
      }
    }
    if (rot > 1150) {
      gpio_set_level(AI, 0);
    }
    if ((i++ % 10) == true) {
      // Выводим значение счетчика
      ESP_LOGI(COUNTER_TAG, "Counter: %d", rot);
    }
    vTaskDelay(xBlockTime);
  }
}
