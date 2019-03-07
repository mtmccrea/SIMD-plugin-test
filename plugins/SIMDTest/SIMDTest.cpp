// PluginSIMDTest.cpp
// my name (myusername@site.com)

#include "SC_PlugIn.hpp"
#include "SIMDTest.hpp"

#include "function_attributes.h"

#ifdef NOVA_SIMD
#include "simd_memory.hpp"
#include "simd_binary_arithmetic.hpp"
#include "simd_pan.hpp"
#include "simd_mix.hpp"
using nova::slope_argument;
#endif

#include <boost/align/is_aligned.hpp>

static InterfaceTable *ft;

namespace SIMDTest {

SIMDTest::SIMDTest()
{
//    set_calc_function<SIMDTest, &SIMDTest::next>();
//    next(1);

#if defined(NOVA_SIMD)
//	if (boost::alignment::is_aligned( BUFLENGTH, 16 ))
	if (boost::alignment::is_aligned( bufferSize(), 16 )) {
		Print("setting NOVA calc func\n");
		set_calc_function<SIMDTest, &SIMDTest::next_nova>();
	} else {
		set_calc_function<SIMDTest, &SIMDTest::next>();
	}
#else
	set_calc_function<SIMDTest, &SIMDTest::next>();
#endif
//	set_calc_function<SIMDTest, &SIMDTest::next>();
	
//	DecodeB2_next(unit, 1);
	next(1);
	
	float orientation = in0(3);
	
	int numOutputs = mNumOutputs;
	float angle = twopi_f / numOutputs;
	m_cosa = cos(angle);
	m_sina = sin(angle);
	m_W_amp = 0.7071067811865476f;
	m_X_amp = 0.5f * (float)cos(orientation * angle);
	m_Y_amp = 0.5f * (float)sin(orientation * angle);
}

void SIMDTest::next(int nSamples)
{
//    const float * input = in(0);
//    const float * gain = in(0);
//    float * outbuf = out(0);
//
//    // simple gain function
//    for (int i = 0; i < nSamples; ++i) {
//        outbuf[i] = input[i] * gain[i];
//    }
	const float *Win0 = zin(0);
	const float *Xin0 = zin(1);
	const float *Yin0 = zin(2);
	
	float W_amp = m_W_amp;
	float X_amp = m_X_amp;
	float Y_amp = m_Y_amp;
	float X_tmp;
	float cosa = m_cosa;
	float sina = m_sina;
	
	int numOutputs = mNumOutputs;
	for (int i=0; i<numOutputs; ++i) {
		float *out = zout(i);
		const float *Win = Win0;
		const float *Xin = Xin0;
		const float *Yin = Yin0;
		for (int j=0; j<nSamples; ++j)
		{
//		LOOP1(inNumSamples,
			  ZXP(out) = ZXP(Win) * W_amp + ZXP(Xin) * X_amp + ZXP(Yin) * Y_amp;
//			  );
		}
		X_tmp = X_amp * cosa + Y_amp * sina;
		Y_amp = Y_amp * cosa - X_amp * sina;
		X_amp = X_tmp;
	}
}
	
void SIMDTest::next_nova(int nSamples)
{
	const float *Win0 = zin(0);
	const float *Xin0 = zin(1);
	const float *Yin0 = zin(2);
	
	using namespace nova;
	vec<float> W_amp = m_W_amp;
	vec<float> X_amp = m_X_amp;
	vec<float> Y_amp = m_Y_amp;
	vec<float> X_tmp;
	vec<float> cosa = m_cosa;
	vec<float> sina = m_sina;
	
	int numOutputs = mNumOutputs;
	int vs = vec<float>::size;
	int loops = nSamples / vs;
	for (int i=0; i<numOutputs; ++i) {
		float * outptr = out(i);
//		float * outptr = OUT(i);
//		float * outptr = mOutBuf[i];
		const float *Win = Win0;
		const float *Xin = Xin0;
		const float *Yin = Yin0;
		
		for (int j = 0; j != loops; ++j) {
			vec<float> result, w, x, y;
			w.load_aligned(Win); x.load_aligned(Xin); y.load_aligned(Yin);
			result = w * W_amp + x * X_amp + y * Y_amp;
			result.store_aligned(outptr);
//			for (int k = 0; k != vs; ++k) {
//				outptr[k] = result.data_[k];
//			}
			outptr += vs; Win += vs; Xin += vs; Yin += vs;
		};
		
		X_tmp = X_amp * cosa + Y_amp * sina;
		Y_amp = Y_amp * cosa - X_amp * sina;
		X_amp = X_tmp;
	}
}

} // namespace SIMDTest

PluginLoad(SIMDTestUGens) {
    // Plugin magic
    ft = inTable;
	//	DefineSimpleCantAliasUnit(SIMDTest);
	registerUnit<SIMDTest::SIMDTest>(ft, "SIMDTest");
//	registerUnit<SIMDTest::SIMDTest>(ft, "SIMDTest", 1);
}
/*
template <class Unit>
void registerUnit( InterfaceTable * ft, const char * name )
{
	UnitCtorFunc ctor = detail::constructClass<Unit>;
	UnitDtorFunc dtor = std::is_trivially_destructible<Unit>::value ? nullptr
	: detail::destroyClass<Unit>;
	
	(*ft->fDefineUnit)( name, sizeof(Unit), ctor, dtor, 0 );
}
*/
