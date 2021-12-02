#include "lockIn.hpp"
#include "dds.hpp"
#include "fir.hpp"
#include "peripherals.hpp"
#include "sinusLUT.hpp"

LockIn::LockIn(DDSParam& dds_param, float FIRFreq)
{
    SinusLUT sinus_sig;
    dds = new DDS(dds_param);
    fir = new FIR(FIRFreq);
}
float LockIn::LockIn_run(float PD_sig)
{
    float dds_output;
    float dds_output_shift;
    dds->Calc(dds_output, dds_output_shift);
    return fir->FIR_Calc(PD_sig * dds_output_shift);
}
LockIn::~LockIn()
{
    delete dds;
    delete fir;
}