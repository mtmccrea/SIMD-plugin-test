// PluginSIMDTest.cpp
// my name (myusername@site.com)

#include "SC_PlugIn.hpp"
#include "SIMDTest.hpp"

static InterfaceTable *ft;

namespace SIMDTest {

SIMDTest::SIMDTest()
{
    set_calc_function<SIMDTest, &SIMDTest::next>();
    next(1);
}

void SIMDTest::next(int nSamples)
{
    const float * input = in(0);
    const float * gain = in(0);
    float * outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain[i];
    }
}

} // namespace SIMDTest

PluginLoad(SIMDTestUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<SIMDTest::SIMDTest>(ft, "SIMDTest");
}
