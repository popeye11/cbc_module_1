#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include "dds.hpp"
#include <cmath>

DDS::DDS(const DDSParam& dds_param)
  : _enable(dds_param.enable),
    _amp(dds_param.amp),
    _offset(dds_param.offset),
    _freq(dds_param.freq),
    _fclk(dds_param.fclk),
    _phaseOffset(dds_param.phaseOffset),
    _accumulatorWidth(dds_param.accumulatorWidth),
    _LUTGridWidth(dds_param.LUTGridWidth),
    _LUT(dds_param.LUT)
{
    auto phi     = _phaseOffset * M_PI / 180.0;
    auto phi_lag = _phaseOffset_1ag * M_PI / 180.0;
    _TW          = round((_freq) / ((_fclk) / (1 << _accumulatorWidth)));
    _detTW       = round((phi - phi_lag) * (1 << _accumulatorWidth) / M_PI_2);
}

void DDS::Calc(float& out, float& shift_out)
{
    _TWSum += _TW;
    auto phase_out = _TWSum;

    if (_phaseOffset != _phaseOffset_1ag) {
        _TWSumShift += _TW + _detTW;
        _phaseOffset_1ag = _phaseOffset;

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
