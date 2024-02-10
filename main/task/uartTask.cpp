// #include <uartTask.h> WTF o.O
// Uart #22 #19.
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <hal/gpio_types.h>
#define UART_TAG "UART"

typedef gpio_num_t Pintype;
static constexpr Pintype EN = GPIO_NUM_2;
static constexpr Pintype DIR = GPIO_NUM_15;
static constexpr Pintype STEP = GPIO_NUM_13;


TaskHandle_t uart;
void uartTask(void *pvParam) {
  const TickType_t xBlockTime = pdMS_TO_TICKS(3 * 1000);
  while (true) {
    if (true) {
      ESP_LOGW(UART_TAG, "uart!");
    }
    vTaskDelay(xBlockTime);
  }
}
