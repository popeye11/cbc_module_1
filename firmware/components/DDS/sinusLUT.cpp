#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#include "sinusLUT.hpp"


SinusLUT::SinusLUT()
{
    float StepEnc = (float)(2.0f * M_PI - 2.0f * M_PI / (LUT_LENGTH - 1)) / (LUT_LENGTH - 1);
    float tmp     = 0.0;

    for (int li = 0; li < LUT_LENGTH; li++) {
        _LUTX[li] = li;
        _LUTd[li] = sinf(tmp);
        tmp = tmp + StepEnc;
    }
}

float SinusLUT::Interp(float index)
{
    if ((index < 0)) {
        return 0;
    }

    int  int_index = (int)index;
    auto lfm         = (_LUTd[int_index + 1] - _LUTd[int_index]) / (_LUTX[int_index + 1] - _LUTX[int_index]);
    auto root_interp = (lfm * (index - _LUTX[int_index])) + _LUTd[int_index];

    if (index >= (float)LUT_LENGTH - 1) {
        lfm = ((_LUTd[0] - _LUTd[LUT_LENGTH - 1]));
        root_interp = ((lfm * (index - _LUTX[LUT_LENGTH - 1]))) + _LUTd[LUT_LENGTH - 1];
    }

    return (root_interp);
}
