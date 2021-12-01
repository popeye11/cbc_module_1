/**
 * @file application.cpp
 * @brief Application starting point.
 * @copyright (c) 2020 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2020-02-04
 */

#include "application.h"
#include "hard_fault_handler.h"
#include "peripherals.hpp"
#include "version.h"
#include <stm32h7xx.h>
//@todo #include <tsp/pid.hpp>
void ADC_DAC_test() // test function
{

    HAL_GPIO_TogglePin(DEBUGGING_PORT, DEBUGGING_PIN);
    const auto adc_values = read_LD_signals();
    transmit_DAC_values((uint16_t)adc_values[0]);
}
void application()
{
    // enable instruction and data caches
    SCB_EnableICache();
    SCB_EnableDCache();
    // enable debugging in low-power modes
    HAL_DBGMCU_EnableDBGStandbyMode();
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStopMode();
    enable_extra_usage_faults();
    peripheral_initialization();

    while (true) {
    }
}

void application_interupt()
{
    ADC_DAC_test();
    // control_test();
    // dds_test();
}
