/**
 * @file adc.cpp
 * @brief ADC function in low level.
 * @copyright (c) 2021 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2019-09-13
 */
#include "adc.hpp"
#include "peripherals.hpp"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_adc_ex.h"
#include <stm32h743xx.h>
#include <cstdint>
ADC::ADC(ADC_TypeDef* adc)
  : _adc(adc){

    };

uint32_t ADC::read()
{
    _adc->ISR |= ADC_ISR_EOC_Msk;
    _adc->CR |= ADC_CR_ADSTART_Msk;
    while ((_adc->ISR & ADC_ISR_EOC_Msk) == 0UL) {
    }
    return _adc->DR;
}
void ADC::init()
{
    // ADC_Enable(&hadc1);
    _adc->ISR |= ADC_ISR_ADRDY;
    _adc->CR |= ADC_CR_ADEN;
}
