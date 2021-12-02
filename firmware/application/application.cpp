/**
 * @file application.cpp
 * @brief Application starting point.
 * @copyright (c) 2020 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2020-02-04
 */

#include "application.h"
#include "dds.hpp"
#include "hard_fault_handler.h"
#include "peripherals.hpp"
#include "sinusLUT.hpp"
#include "version.h"
#include <stm32h7xx.h>
//@todo #include <tsp/pid.hpp>
namespace {
float    DIG2VOLT16BIT  = 3.738e-05;  // 2.45[v]/65535 ->16bit
float    VOLT2DIG16BIT  = 2.6214e+04; // 2.5[v]/2^16->16bit
float    kp_f           = 0.8f;
float    ki_f           = 0.0f;
float    kd_f           = 0.0f;
float    kp_s           = 1.0f;
float    ki_s           = 0.0f;
float    kd_s           = 0.0f;
float    DDSamp         = 1.0f;
float    DDSAmpOffset   = DDSamp;
float    DDSfreq        = 5000.0f;
float    DDSphaseOffset = 0.0f;
float    FIRFreq        = 2 * 5000.0f;
int      AW             = 16;
int      LUTL           = 10;
SinusLUT LUT;

struct DDSParam dds_param = {true, DDSamp, DDSAmpOffset, DDSfreq, DDSphaseOffset, FIRFreq, AW, LUTL, LUT};
DDS*            dds       = new DDS(dds_param, DDSfreq, MainFreq);
// DDS* dds = new DDS(true, DDSamp, DDSAmpOffset, DDSfreq, DDSphaseOffset, MainFreq, AW, LUTL, LUT);

} // namespace
float out;
float shift_out;
void  DDS_test() // test function
{

    dds->Calc(out, shift_out);
    transmit_DAC_values((uint16_t)(shift_out * VOLT2DIG16BIT));
}
void ADC_DAC_test() // test function
{
    int count = 0;
    HAL_GPIO_TogglePin(DEBUGGING_PORT, DEBUGGING_PIN);
    while (count < 150) {
        count++;
    }
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
    // ADC_DAC_test();
    // control_test();
    // dds_test();
    DDS_test();
}
