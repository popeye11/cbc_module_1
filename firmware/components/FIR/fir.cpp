#include "fir.hpp"
#include "peripherals.hpp"
#include <math.h>
FIR::FIR(float Freq)
{
    FIR_Freq = Freq;
    for (int li = 0; li < BUFFER_LEN - 1; li++) {
        FIR_ACCUMULATOR[li] = 0;
    }
    Idx   = 0;
    SUMME = 0.0f;
}
float FIR::FIR_Calc(float Input)
{
    SUMME                = SUMME - FIR_ACCUMULATOR[Idx] + Input;
    FIR_ACCUMULATOR[Idx] = Input;
    ACCUMLen             = round(CtrlFreq / FIR_Freq);
    if (Idx < (ACCUMLen - 1)) {
        Idx++;
    } else {
        Idx = 0;
    }
    return (SUMME / ACCUMLen);
}