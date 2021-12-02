#pragma once
#include "dds.hpp"
#include "fir.hpp"
#include "sinusLUT.hpp"

class LockIn
{
  public:
    DDS* dds;
    FIR* fir;
    LockIn(float DDSamp, float DDSAmpOffset, float DDSfreq, float DDSphaseOffset, float FIRFreq);
    ~LockIn();
    float LockIn_run(float input);
};