#include "peripherals.hpp"
#include "adc.h"
#include "adc.hpp"
#include "spi.hpp"
#include "tim.h"
#include <stm32h7xx.h>
#include <stm32h7xx_hal_tim.h>
#include <cstdint>

SPI  CTRL_CHN1{SPI2};    // control output channel 1
SPI  CTRL_CHN2{SPI5};    // control output channel 2
ADC  LD_CHN1{INPUT_ADC}; // Laser diode signal channel 1
void peripheral_initialization()
{
    __HAL_TIM_SET_PRESCALER(&htim2, Tim2PSC);
    __HAL_TIM_SET_AUTORELOAD(&htim2, Tim2ARR);
    LD_CHN1.init();
    CTRL_CHN1.init();
    CTRL_CHN2.init();
    HAL_TIM_Base_Start_IT(&htim2);
}
std::array<uint32_t, LDCHNs> read_LD_signals()
{

    const auto adc_value_LD1 = LD_CHN1.read(); // read adc value of channel 1
    const auto adc_value_LD2 = 0;              // reserves for  channel 2
    const auto adc_value_LD3 = 0;              // reserves for  channel 3
    return {adc_value_LD1, adc_value_LD2, adc_value_LD3};
}
void transmit_DAC_values(uint16_t dacvalue)
{
    CTRL_CHN1.transmit(dacvalue); //  transmit adc value for debugging purpose
    CTRL_CHN2.transmit(dacvalue, CTRL_CHN2_NR);
}