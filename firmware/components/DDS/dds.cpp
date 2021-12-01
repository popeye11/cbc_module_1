#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#include "dds.hpp"


DDS::DDS(
    bool      enable,
    float     amp,
    float     offset,
    float     freq,
    float     phaseOffset,
    float     fclk,
    int       accumulatorWidth,
    int       LUTGridWidth,
    SinusLUT& LUT)
  : _enable(enable),
    _amp(amp),
    _offset(offset),
    _phaseOffset(phaseOffset),
    _accumulatorWidth(accumulatorWidth),
    _LUTGridWidth(LUTGridWidth),
    _LUT(LUT)
{
    auto phi     = _phaseOffset * M_PI / 180.0;
    auto phi_lag = _phaseOffset_1ag * M_PI / 180.0;
    _TW    = round((freq) / ((fclk) / (1 << _accumulatorWidth)));
    _detTW = round((phi - phi_lag) * (1 << _accumulatorWidth) / M_PI_2);
}

void DDS::Calc(float &out, float &shift_out)
{
    _TWSum         += _TW;
    auto phase_out  = _TWSum;

    if (_phaseOffset != _phaseOffset_1ag) {
        _TWSumShift      += _TW + _detTW;
        _phaseOffset_1ag  = _phaseOffset;

    } else {
        _TWSumShift += _TW;
    }

    auto phase_outShift = _TWSumShift;
    if (phase_outShift > (1 << _accumulatorWidth) - 1) {
        phase_outShift = phase_outShift - (1 << _accumulatorWidth) + 1;
        _TWSumShift    = phase_outShift;
    }
    if (phase_out > (1 << _accumulatorWidth) - 1) {
        phase_out = phase_out - (1 << _accumulatorWidth) + 1;
        _TWSum    = phase_out;
    }

    auto _LUTIndexShift = (float)phase_outShift / (float)((1 << (_accumulatorWidth - _LUTGridWidth)));
    auto _LUTIndex      = (float)phase_out / (float)((1 << (_accumulatorWidth - _LUTGridWidth)));

    if (_enable) {
        out       = _LUT.Interp(_LUTIndex);
        shift_out = _LUT.Interp(_LUTIndexShift) * _amp + _offset;
    } else {
        out       = 0.0;
        shift_out = 0.0;
    }
}
