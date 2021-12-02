#pragma once
#include "dds.hpp"
#include "fir.hpp"
#include "sinusLUT.hpp"

class LockIn
{
  public:
    DDS* dds;
    FIR* fir;
    LockIn(DDSParam& dds_param, float FIRFreq);
    ~LockIn();
    float LockIn_run(float input);
};