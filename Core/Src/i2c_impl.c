#include "i2c_impl.h"

#include "board.h"
#include "gpio.h"
#include "i2c.h"
#include "stm32l0xx_hal.h"

static I2C_HandleTypeDef hi2c;

bool _i2c_master_init_impl(void) {
    hi2c.Instance = I2C1;
    hi2c.Init.Timing = 0x0000020B;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c) != HAL_OK) {
        return false;
    }
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        return false;
    }
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c, 0) != HAL_OK) {
        return false;
    }
    return true;
}

bool _i2c_master_write_impl(
  const uint8_t device, const uint8_t reg, const void* buf, uint16_t len) {
    return HAL_I2C_Mem_Write(&hi2c, device << 1, reg, 1, (uint8_t*)buf, len, HAL_MAX_DELAY) ==
           HAL_OK;
}

bool _i2c_master_read_impl(const uint8_t device, const uint8_t reg, void* buf, uint16_t len) {
    return HAL_I2C_Mem_Read(&hi2c, device << 1, reg, 1, (uint8_t*)buf, len, HAL_MAX_DELAY) ==
           HAL_OK;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle) {
    if (i2cHandle->Instance == I2C1) {
        __HAL_RCC_GPIOB_CLK_ENABLE();

        gpio_config_t config = {
            .mode = GPIO_MODE_AF_OD,
            .pull = GPIO_PULLUP,
            .speed = GPIO_SPEED_FREQ_VERY_HIGH,
            .af = GPIO_AF1_I2C1,
        };
        gpio_config_pin(SDA_PORT, SDA_PIN, &config);
        gpio_config_pin(SCL_PORT, SCL_PIN, &config);

        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle) {
    if (i2cHandle->Instance == I2C1) {
        __HAL_RCC_I2C1_CLK_DISABLE();

        gpio_unconfig_pin(SDA_PORT, SDA_PIN);
        gpio_unconfig_pin(SCL_PORT, SCL_PIN);
    }
}