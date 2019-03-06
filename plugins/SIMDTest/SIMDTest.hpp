// PluginSIMDTest.hpp
// my name (myusername@site.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace SIMDTest {

class SIMDTest : public SCUnit
{
public:
    SIMDTest();

    // Destructor
    // ~SIMDTest();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace SIMDTest
