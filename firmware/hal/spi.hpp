/**
 * @file spi.hpp
 * @brief SPI interface using the HAL library.
 * @copyright (c) 2021 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2019-09-14
 */
#pragma once

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_hal_spi_ex.h"
#include <stm32h743xx.h>
#include <cstdint>
class SPI
{
  private:
    SPI_TypeDef* const _spi;

  public:
    SPI(SPI_TypeDef* spi);
    void init();
    void transmit(uint16_t data);
    void transmit(uint16_t data, uint8_t channel);
};
