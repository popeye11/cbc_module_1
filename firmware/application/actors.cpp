#pragma once
#include "actors.h"
#include "ctrlLoop.hpp"
struct PIDParam pid_param = {kp_f, ki_f, kd_f, kp_s, ki_s, kd_s};
struct DDSParam dds_param = {true, DDSamp, DDSAmpOffset, DDSfreq, DDSphaseOffset, MainFreq, AW, LUTL, LUT};

Actor::Actor()
{
    actor = new ControlLoop(pid_param, dds_param, FIRFreq);
}
//
void Actor::actors_run()
{
    float      sctrl_out;
    float      fctrl_out;
    const auto adc_values = read_LD_signals();
    auto       pd_signal  = adc_values[0] * DIG2VOLT16BIT;
    actor->step(pd_signal, sctrl_out, fctrl_out);
    uint16_t dacout[2] = {fctrl_out * VOLT2DIG16BIT, sctrl_out * VOLT2DIG16BIT};
    transmit_DAC_values(dacout);
}
Actor::~Actor()
{
    delete actor;
}
