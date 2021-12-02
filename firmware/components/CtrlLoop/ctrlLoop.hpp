#pragma once
#include "dds.hpp"
#include "lockIn.hpp"
#include "peripherals.hpp"
#include <tsp/pid.hpp>
namespace {
const float   frq_pids = 1e3;
const float   TS       = 1.0f / CtrlFreq;
const int16_t CountLim = round(CtrlFreq / frq_pids);
} // namespace
struct PIDParam
{
    float kpf; // fast controller kp
    float kif; // fast controller ki
    float kdf; // fast controller kd
    float kps; // slow controller kp
    float kis; // slow controller ki
    float kds; // slow controller kd
};
class ControlLoop
{
  private:
    LockIn*                        _lockin;
    int                            _count;
    toptica::tsp::pid::pid<float>* _pidf; // fast pid
    toptica::tsp::pid::pid<float>* _pids; // slow pid
  public:
    ControlLoop(const PIDParam& pid_param, DDSParam& dds_param, const float FIRfreq);
    ~ControlLoop();
    void                 step(float error, float& sctrl_out, float& fctrl_out);
    float                test_dds();
    std::array<float, 2> test_pid(float error);
};