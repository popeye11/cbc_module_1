#pragma once
namespace {
const int BUFFER_LEN = 1000;
} // namespace
class FIR
{
  public:
    float FIR_ACCUMULATOR[BUFFER_LEN];
    int   ACCUMLen;
    float FIR_Freq;
    float FIR_Calc(float Input);
    FIR(float Freq);

  private:
    float SUMME;
    int   Idx;
};