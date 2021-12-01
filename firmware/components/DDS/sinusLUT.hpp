#pragma once

#define LUT_LENGTH 1024

class SinusLUT
{
  public:
    SinusLUT();
    float Interp(float index);

  private:
    int   _LUTX[LUT_LENGTH];
    float _LUTd[LUT_LENGTH];
};
