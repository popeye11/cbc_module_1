#pragma once
#include "sinusLUT.hpp"

class DDS
{
  private:
    bool     _enable = true;
    float    _amp    = 0.0;
    float    _offset = 0.0;

    float    _phaseOffset     = 0.0;
    float    _phaseOffset_1ag = 0.0;

    int      _TW         = 0;
    int      _TWSum      = 0;
    int      _TWSumShift = 0;
    int      _detTW      = 0;

    int      _accumulatorWidth = 0;
    int      _LUTGridWidth     = 0;;

    SinusLUT _LUT;

  public:
    DDS(bool      enable,
        float     amp,
        float     offset,
        float     freq,
        float     phaseOffset,
        float     fclk,
        int       accumulatorWidth,
        int       LUTGridWidth,
        SinusLUT& LUT);
    void Calc(float &out, float &shift_out);
};
