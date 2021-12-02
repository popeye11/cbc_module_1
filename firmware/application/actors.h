#include "ctrlLoop.hpp"
namespace {
float    DIG2VOLT16BIT  = 3.7e-05;    // 2.45[v]/65535 ->16bit
float    VOLT2DIG16BIT  = 2.6214e+04; // 2.5[v]/2^16->16bit
float    kp_f           = 1.0f;
float    ki_f           = 0.0f;
float    kd_f           = 0.0f;
float    kp_s           = 1.0f;
float    ki_s           = 0.0f;
float    kd_s           = 0.0f;
float    DDSamp         = 1.0f;
float    DDSAmpOffset   = DDSamp;
float    DDSfreq        = 20000.0f;
float    DDSphaseOffset = 0.0f;
float    FIRFreq        = 2000.0f;
int      AW             = 16;
int      LUTL           = 10;
SinusLUT LUT;
} // namespace
class Actuator
{
  public:
    Actuator(ControlLoop& ctrl, CHN_Param& chn_param);
    void execute();

  private:
    ControlLoop _ctrl;
    CHN_Param   _chn_param;
};
void actuators_run();
