// TOF 0x39 - TOF AP3216
#include "sdkconfig.h"
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>
#define TOF_TAG "TOF"

#define I2C_FREQ_HZ 1000000
#define I2C_SDA GPIO_NUM_26
#define I2C_SCL GPIO_NUM_27

#define TOF_ADDRESS 0x39
#define TOF_CONTROL 0x00
#define TOF_START_PS 0x02 // Proximity
#define TOF_START_AMB 0x01 // Ambient
#define TOF_START_AMB_PS 0x03 // ps+amb

#define TOF_PS_LOW 0x0C   // Proximity Data LOW 6 bits
#define TOF_PS_HIGH 0x0F  // Proximity Data HIGH 6 bits
#define TOF_AMB_LOW 0x0E  // Ambient Data LOW 6 bits
#define TOF_AMB_HIGH 0x0D // Ambient Data HIGH 6 bits

#include <i2cdev.h>
#include <string.h>
#include "i2cdev.h"
typedef struct {
  i2c_dev_t i2c_dev;
  int16_t AC1;
  bool ps;
  bool amb;
} tof_t;
#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

void tofTestConnection();
esp_err_t tofBegin(tof_t *tof);
void tofStart(tof_t *tof);
void tofStartAmbient(tof_t *tof);
void tofStartProximity(tof_t *tof);

int16_t tofGetAmbient(tof_t *tof);
int16_t tofGetProximity(tof_t *tof);

esp_err_t tofWrite(i2c_dev_t *dev, uint8_t cmd);
esp_err_t tofRead(i2c_dev_t *dev, uint8_t reg, int8_t *r);
esp_err_t tofRead16(i2c_dev_t *dev, uint8_t reg, int16_t *r);

TaskHandle_t tof;
void tofTask(void *pvParam) {
  tofTestConnection();
  const TickType_t xBlockTime = pdMS_TO_TICKS(500);
  tof_t tof;
  memset(&tof, 0, sizeof(tof_t));
  tofBegin(&tof);
  int16_t amb = 0;
  int16_t prox = 0;
  while (true) {
    if (true) {
      // ESP_LOGI(TOF_TAG, "tof!");
    }
    tofStart(&tof);
    amb = tofGetAmbient(&tof);
    prox = tofGetProximity(&tof);
    ESP_LOGI(TOF_TAG, "TOF abm= %d | prox= %d", amb, prox);
    vTaskDelay(xBlockTime);
  }
}



esp_err_t tofBegin(tof_t *tof) {
  CHECK_ARG(tof);
  tof->i2c_dev.port = I2C_NUM_0;
  tof->i2c_dev.addr = TOF_ADDRESS;
  tof->i2c_dev.cfg.sda_io_num = I2C_SDA;
  tof->i2c_dev.cfg.scl_io_num = I2C_SCL;
  tof->i2c_dev.cfg.master.clk_speed = I2C_FREQ_HZ;
  return i2c_dev_create_mutex(&tof->i2c_dev);
}
void tofStart(tof_t *tof) { tofWrite(&tof->i2c_dev, TOF_START_AMB_PS); }
void tofStartAmbient(tof_t *tof) { tofWrite(&tof->i2c_dev, TOF_START_AMB); };
void tofStartProximity(tof_t *tof) { tofWrite(&tof->i2c_dev, TOF_START_PS); };

int16_t tofGetAmbient(tof_t *tof) {
  int16_t v;
  tofRead16(&tof->i2c_dev, TOF_AMB_LOW, &v);
  // long ambValue =
  //     (tofRead(dev, TOF_AMB_HIGH) << 8) + (tofRead(dev, TOF_AMB_LOW));
  return v;
};
int16_t tofGetProximity(tof_t *tof) {
  int8_t a, b;
  tofRead(&tof->i2c_dev, TOF_PS_HIGH, &a); // PS Data HIGH 6 bits
  tofRead(&tof->i2c_dev, TOF_PS_LOW, &b); // PS Data LOW 4 bits
  long psValue = ((a & 0b0011111) << 4) + (b & 0b00001111);
  return psValue;
};

esp_err_t tofWrite(i2c_dev_t *dev, uint8_t cmd) {
  return i2c_dev_write_reg(dev, TOF_CONTROL, &cmd, 1);
};
esp_err_t tofRead(i2c_dev_t *dev, uint8_t reg, int8_t *r) {
  return i2c_dev_read_reg(dev, reg, r, 1);
};
esp_err_t tofRead16(i2c_dev_t *dev, uint8_t reg, int16_t *r) {
  uint8_t d[] = {0, 0};
  CHECK(i2c_dev_read_reg(dev, reg, d, 2));
  *r = ((int16_t)d[0] << 8) | (d[1]);
  return ESP_OK;
}

// Old test 
void tofTestConnection() {
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
}
