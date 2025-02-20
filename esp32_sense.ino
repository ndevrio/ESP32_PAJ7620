//  Calculation of settings for each frequency band
//  Resolution = log2(Clock(80MHz)/f) + 1   ex: 50.000 HZ = 80.0000/50.000 = 1.600 log2(1600) = 10 + 1 = 11
//  Duty 50%  = (2**Resolution)/2       ex: 2**11 = 2048   2048/2 = 1024

#include "driver/ledc.h"
#include "apds.h"
#include <ESP32DMASPISlave.h>

APDS9500 sensor;
ESP32DMASPI::Slave slave;

static constexpr size_t BUFFER_SIZE = 4054; // should be multiple of 4
static constexpr size_t QUEUE_SIZE = 1;
uint8_t *dma_tx_buf;
uint8_t *dma_rx_buf;


ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution    = LEDC_TIMER_1_BIT,
    .timer_num  = LEDC_TIMER_0,
    .freq_hz    = 40000000
};
ledc_channel_config_t ledc_channel = {
    .gpio_num   = D0,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = LEDC_CHANNEL_0,
    .timer_sel  = LEDC_TIMER_0,
    .duty       = 2
};
//------------------------------------------------------------------------
void setup() {
  Serial.begin(10000000);
  while(!Serial);

  Serial.println("starting up");

  /// DMA init
  // to use DMA buffer, use these methods to allocate buffer
  dma_tx_buf = slave.allocDMABuffer(BUFFER_SIZE);
  dma_rx_buf = slave.allocDMABuffer(BUFFER_SIZE);

  slave.setDataMode(SPI_MODE3);           // default: SPI_MODE0
  slave.setMaxTransferSize(BUFFER_SIZE);  // default: 4092 bytes

  delay(10);

  // begin() after setting
  slave.begin();  // default: HSPI (please refer README for pin assignments)

  Serial.println("start spi slave");


  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&ledc_channel);

  delay(100);
  while(sensor.init() != 0) {
    delay(10);
  }
  while(sensor.user_init() != 0) {
    delay(10);
  }

  Serial.println("through");
}
//------------------------------------------------------------------------
void loop() {
  sensor.writeReg(0x5B, 0x01);

  // start and wait to complete one BIG transaction (same data will be received from slave)
  // if there is no transaction in queue, add transaction
  if (slave.remained() == 0) {
      slave.queue(dma_rx_buf, dma_tx_buf, BUFFER_SIZE);
  }

  // if transaction has completed from master,
  // available() returns size of results of transaction,
  // and buffer is automatically updated
  while (slave.available()) {
      // do something with received data: spi_slave_rx_buf

      slave.pop();
      Serial.write(dma_rx_buf, BUFFER_SIZE-4);

      for (int i = 0; i < BUFFER_SIZE; ++i)
				dma_rx_buf[i] = 0;
  }

  delay(10);
}