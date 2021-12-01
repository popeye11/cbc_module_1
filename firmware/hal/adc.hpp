/**
 * @file adc.hpp
 * @brief ADC interface using the HAL library.
 * @copyright (c) 2021 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2019-09-13
 */
#pragma once

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_adc_ex.h"
#include <cstdint>
class ADC
{
  private:
    ADC_TypeDef* const _adc;

  public:
    ADC(ADC_TypeDef* adc);
    void     init();
    uint32_t read();
};
