
#pragma once
#include "adc.h"
#include "adc.hpp"
#include "spi.hpp"
#include <stm32h7xx.h>
#include <stm32h7xx_hal_tim.h>
#include <array>
#include <cstdint>
#include <utility>
#define DEBUGGING_PIN (GPIO_PIN_0)     //  GPIO pin for ADC testing
#define DEBUGGING_PORT (GPIOG)         //  GPIO Port for ADC testing
#define SIGNAL_OUTPUT_PIN (GPIO_PIN_6) //  GPIO Pin for SPI5 NSS signal
#define SIGNAL_OUTPUT_PORT (GPIOF)     //  GPIO Port for SPI5 NSS signal
#define INPUT_ADC (ADC1)               //  ADC1
namespace {
const uint16_t LDCHNs       = 3;                       // LD channel numbers
const uint32_t MainFreq     = 200e6;                   // µC main frequency [Hz]
const uint16_t Tim2PSC      = 0;                       // TIM prescaler
const uint32_t CtrlFreq     = 100e3;                   // loop frequency [Hz]
const uint32_t Tim2ARR      = MainFreq / CtrlFreq - 1; // TIM ARR
const uint8_t  CTRL_CHN2_NR = 0;
} // namespace

void                         peripheral_initialization();
std::array<uint32_t, LDCHNs> read_LD_signals();
void                         transmit_DAC_values(uint16_t dacvalue[2]);
