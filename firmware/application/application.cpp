/**
 * @file application.cpp
 * @brief Application starting point.
 * @copyright (c) 2020 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2020-02-04
 */

#include "application.h"
#include "actors.h"
#include "ctrlLoop.hpp"
#include "dds.hpp"
#include "hard_fault_handler.h"
#include "lockIn.hpp"
#include "peripherals.hpp"
#include "sinusLUT.hpp"
#include "version.h"
#include <stm32h7xx.h>
//@todo #include <tsp/pid.hpp>
// namespace {
// float    DIG2VOLT16BIT  = 3.7e-05;    // 2.45[v]/65535 ->16bit
// float    VOLT2DIG16BIT  = 2.6214e+04; // 2.5[v]/2^16->16bit
// float    kp_f           = 1.0f;
// float    ki_f           = 0.0f;
// float    kd_f           = 0.0f;
// float    kp_s           = 1.0f;
// float    ki_s           = 0.0f;
// float    kd_s           = 0.0f;
// float    DDSamp         = 1.0f;
// float    DDSAmpOffset   = DDSamp;
// float    DDSfreq        = 5000.0f;
// float    DDSphaseOffset = 0.0f;
// float    FIRFreq        = 1000.0f;
// int      AW             = 16;
// int      LUTL           = 10;
// SinusLUT LUT;

// struct PIDParam pid_param = {kp_f, ki_f, kd_f, kp_s, ki_s, kd_s};
// struct DDSParam dds_param = {true, DDSamp, DDSAmpOffset, DDSfreq, DDSphaseOffset, MainFreq, AW, LUTL, LUT};
// DDS*            dds       = new DDS(dds_param);
// LockIn*         lock_in   = new LockIn(dds_param, FIRFreq);
// ControlLoop*    ctrl      = new ControlLoop(pid_param, dds_param, FIRFreq);
// } // namespace

void ctrl_test()
{
    // int   count = 0;
    // float sctrl_out;
    // float fctrl_out;
    HAL_GPIO_TogglePin(DEBUGGING_PORT, DEBUGGING_PIN);
    // while (count < 150) {
    //     count++;
    // }
    act_chn1.run();
    // const auto adc_values = read_LD_signals();
    // auto       pd_signal  = adc_values[0] * DIG2VOLT16BIT;
    // ctrl->step(pd_signal, sctrl_out, fctrl_out);
    // uint16_t dacout[2] = {fctrl_out * VOLT2DIG16BIT, sctrl_out * VOLT2DIG16BIT};
    // transmit_DAC_values(dacout);
}
// void Lock_In_test()
// {
//     int count = 0;
//     HAL_GPIO_TogglePin(DEBUGGING_PORT, DEBUGGING_PIN);
//     while (count < 150) {
//         count++;
//     }
//     const auto adc_values = read_LD_signals();
//     auto       pd_signal  = adc_values[0] * DIG2VOLT16BIT;
//     auto       res        = lock_in->LockIn_run(pd_signal);
//     uint16_t   dacout[2]  = {res * VOLT2DIG16BIT, res * VOLT2DIG16BIT};
//     transmit_DAC_values(dacout);
// }

// void DDS_test() // test function
// {
//     float out;
//     float shift_out;
//     dds->Calc(out, shift_out);
//     uint16_t dacout[2] = {shift_out * VOLT2DIG16BIT, shift_out * VOLT2DIG16BIT};
//     transmit_DAC_values(dacout);
// }
// void ADC_DAC_test() // test function
// {
//     int count = 0;
//     HAL_GPIO_TogglePin(DEBUGGING_PORT, DEBUGGING_PIN);
//     while (count < 150) {
//         count++;
//     }
//     const auto adc_values = read_LD_signals();
//     uint16_t   dacout[2]  = {adc_values[0] * VOLT2DIG16BIT, adc_values[0] * VOLT2DIG16BIT};
//     transmit_DAC_values(dacout);
// }

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
    Actor act_chn1;
    while (true) {
    }
}

void application_interupt()
{
    // ADC_DAC_test();
    // control_test();
    // dds_test();
    // DDS_test();
    // Lock_In_test();
    ctrl_test();
}
