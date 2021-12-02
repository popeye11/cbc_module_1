#include "ctrlLoop.hpp"
#include "peripherals.hpp"
#include <math.h>

ControlLoop::ControlLoop(const PIDParam& pid_param, DDSParam& dds_param, const float FIRfreq)
{
    _count = 0;
    _pids  = new toptica::tsp::pid::pid<float>{CountLim * TS};
    _pidf  = new toptica::tsp::pid::pid<float>{TS};
    _pids->set_p(pid_param.kps);
    _pids->set_i(pid_param.kis);
    _pids->set_d(pid_param.kds);
    _pidf->set_p(pid_param.kpf);
    _pidf->set_i(pid_param.kif);
    _pidf->set_d(pid_param.kdf);
    _pids->enable();
    _pidf->enable();
    _lockin = new LockIn(dds_param, FIRfreq);
}
void ControlLoop::step(float error, float& sctrl_out, float& fctrl_out)
{

    _count++;
    auto lockinSig = _lockin->LockIn_run(error);
    fctrl_out      = _pidf->run(error);
    if (_count < CountLim) {
        _count++;
        sctrl_out = _pids->run(error);

    } else {
        _count = 0;
    }
}

ControlLoop::~ControlLoop()
{
    delete _pids;
    delete _pidf;
    delete _lockin;
}
