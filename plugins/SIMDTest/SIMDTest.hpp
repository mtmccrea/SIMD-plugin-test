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
	void next_nova(int nSamples);

    // Member variables
	float m_cosa, m_sina;
	float m_W_amp, m_X_amp, m_Y_amp;
};

} // namespace SIMDTest
