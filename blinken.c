#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

// i2c definitions

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define FREQ 400000

#define ADDRESS 0x74

// register definitions - most of these need to be blanked ->
// mark these as NULL as no electrical interface enabled on a
// pico scroll

// FRAME_MODE -> 0x0 to 0x7 value for _write_ to frames 0 to 7
//               higher bits -> 0 so picture mode
// FRAME_NO -> select picture frame to show
// IMAGE_REGISTER -> 144 byte register for bytes for given frame
//                   only 119 visible (assumed to be not first 119)

#define FRAME_MODE 0x0
#define FRAME_NO 0x1
#define NULL0 0x2
#define NULL1 0x3
#define NULL2 0x6
#define SHUTDOWN 0xa
#define IMAGE_REGISTER 0x24

// general definitions - buffer size of 144 pixels
#define BUFFER_SIZE 0x90

// helper functions
int write_byte_to_register(uint8_t reg, uint8_t value) {
  uint8_t data[2];

  data[0] = reg;
  data[1] = value;

  i2c_write_blocking(I2C_PORT, ADDRESS, data, 2, false);

  return 0;
}

int write_picture_to_register(uint8_t picture, uint8_t *buffer) {
  uint8_t scratch[BUFFER_SIZE + 1];

  // push register to select the picture to write - use first two bytes
  // of scratch
  scratch[0] = FRAME_MODE;
  scratch[1] = picture;

  i2c_write_blocking(I2C_PORT, ADDRESS, scratch, 2, false);

  // can I do this without a copy operation? use memcpy?
  scratch[0] = 0x24;
  for (int j = 0; j < BUFFER_SIZE; j++) {
    scratch[j + 1] = buffer[j];
  }

  i2c_write_blocking(I2C_PORT, ADDRESS, scratch, BUFFER_SIZE + 1, false);

  return 0;
}

int main() {
  uint8_t buffer[BUFFER_SIZE] = {0};

  stdio_init_all();

  i2c_init(I2C_PORT, FREQ);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  // initialise device
  write_byte_to_register(FRAME_MODE, 0x0);
  write_byte_to_register(FRAME_NO, 0x0);
  write_byte_to_register(NULL0, 0x0);
  write_byte_to_register(NULL1, 0x0);
  write_byte_to_register(NULL2, 0x0);
  write_byte_to_register(SHUTDOWN, 0x0);

  // write something interesting (ish) to the buffer
  int odd = 0;

  while (true) {
    if (odd == 0) {
      odd = 1;
    } else {
      odd = 0;
    }

    for (int j = 0; j < BUFFER_SIZE; j++) {
      buffer[j] = (odd + j % 2) * 0x8;
    }

    write_picture_to_register(0x0, buffer);
    write_byte_to_register(FRAME_NO, 0x0);

    sleep_ms(100);
  }
  return 0;
}
