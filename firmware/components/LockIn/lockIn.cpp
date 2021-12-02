#include "lockIn.hpp"
#include "dds.hpp"
#include "fir.hpp"
#include "peripherals.hpp"
#include "sinusLUT.hpp"
namespace {
bool EN   = true;
int  AW   = 16;
int  LUTW = 10;

} // namespace
LockIn::LockIn(float DDSamp, float DDSAmpOffset, float DDSfreq, float DDSphaseOffset, float FIRFreq)
{
    SinusLUT sinus_sig;
    // dds = new DDS(EN, DDSamp, DDSAmpOffset, DDSfreq, DDSphaseOffset, CtrlFreq, AW, LUTW, sinus_sig);
    // fir = new FIR(FIRFreq);
}
float LockIn::LockIn_run(float PD_sig)
{
    // float dds_output;
    // float dds_output_shift;
    // dds->Calc(dds_output, dds_output_shift);
    // return fir->FIR_Calc(PD_sig * dds_output_shift);
}
LockIn::~LockIn()
{
    delete dds;
    delete fir;
}