#pragma once
#include "actors.h"
#include "ctrlLoop.hpp"
#include "peripherals.hpp"

//   Actuator(ControlLoop ctrl, CHN_Param chn_param);
// public:
//     Actuator(ControlLoop ctrl, CHN_Param chn_param);
//     void execute();

//   private:
//     ControlLoop _ctrl;
//     CHN_Param   _chn_param;
// void actuators_run();

Actuator::Actuator(ControlLoop& ctrl, CHN_Param& chn_param) : _ctrl(ctrl), _chn_param(chn_param){};
void Actuator::execute()
{
    float      sctrl_out;
    float      fctrl_out;
    const auto adc_values = read_LD_signals();
    auto       pd_signal  = adc_values[0] * DIG2VOLT16BIT;
    _ctrl.step(pd_signal, sctrl_out, fctrl_out); // actor_CH1
    uint16_t dacout[2] = {fctrl_out * VOLT2DIG16BIT, sctrl_out * VOLT2DIG16BIT};
    transmit_DAC_values(dacout, _chn_param); // CHN1_param
}
// void actuators_run()
// {

//     Actuator act_chn1 = Actuator(actor_CH1, CHN1_param);
//     act_chn1.execute();
// }
