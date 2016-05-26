// generated from file '../src/LV2/faust/gx_ampmodul.dsp' by dsp2cc:
// Code generated with Faust 0.9.65 (http://faust.grame.fr)

#include "valve.h"

namespace gx_ampmodul {

class Dsp: public PluginLV2 {
private:
	uint32_t fSamplingFreq;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT	*fslider0_;
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT	*fslider1_;
	double 	fRec5[2];
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT	*fslider2_;
	double 	fRec11[2];
	FAUSTFLOAT 	fslider3;
	FAUSTFLOAT	*fslider3_;
	double 	fRec17[2];
	double 	fVec0[2];
	double 	fRec16[2];
	double 	fRec15[3];
	double 	fVec1[2];
	int 	iConst0;
	double 	fConst1;
	double 	fConst2;
	double 	fConst3;
	double 	fConst4;
	double 	fRec18[2];
	double 	fRec14[2];
	double 	fConst5;
	double 	fConst6;
	double 	fConst7;
	double 	fConst8;
	double 	fConst9;
	double 	fRec13[2];
	double 	fConst10;
	double 	fConst11;
	double 	fConst12;
	double 	fConst13;
	double 	fRec12[2];
	double 	fVec2[2];
	double 	fRec10[2];
	double 	fRec9[3];
	double 	fVec3[2];
	double 	fConst14;
	double 	fConst15;
	double 	fConst16;
	double 	fConst17;
	double 	fRec19[2];
	double 	fRec8[2];
	double 	fRec7[2];
	double 	fRec6[2];
	double 	fVec4[2];
	double 	fRec4[2];
	double 	fRec3[3];
	double 	fVec5[2];
	double 	fConst18;
	double 	fConst19;
	double 	fConst20;
	double 	fConst21;
	double 	fRec20[2];
	double 	fRec2[2];
	double 	fRec1[2];
	double 	fRec0[6];
	FAUSTFLOAT 	fslider4;
	FAUSTFLOAT	*fslider4_;
	FAUSTFLOAT 	fslider5;
	FAUSTFLOAT	*fslider5_;
	double 	fRec21[6];
	double 	fVec6[2];
	double 	fRec36[2];
	double 	fRec35[3];
	double 	fVec7[2];
	double 	fRec37[2];
	double 	fRec34[2];
	double 	fRec33[2];
	double 	fRec32[2];
	double 	fVec8[2];
	double 	fRec31[2];
	double 	fRec30[3];
	double 	fVec9[2];
	double 	fRec38[2];
	double 	fRec29[2];
	double 	fRec28[2];
	double 	fRec27[2];
	double 	fVec10[2];
	double 	fRec26[2];
	double 	fRec25[3];
	double 	fVec11[2];
	double 	fRec39[2];
	double 	fRec24[2];
	double 	fRec23[2];
	double 	fRec22[6];
	double 	fRec40[6];
	void connect(uint32_t port,void* data);
	void clear_state_f();
	void init(uint32_t samplingFreq);
	void compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *input1, FAUSTFLOAT *output0, FAUSTFLOAT *output1);

	static void clear_state_f_static(PluginLV2*);
	static void init_static(uint32_t samplingFreq, PluginLV2*);
	static void compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *input1, FAUSTFLOAT *output0, FAUSTFLOAT *output1, PluginLV2*);
	static void del_instance(PluginLV2 *p);
	static void connect_static(uint32_t port,void* data, PluginLV2 *p);
public:
	Dsp();
	~Dsp();
};



static const char* parm_groups[] = {
	"amp2.stage2", N_("Postamp Tube2"),
	"amp2.stage1", N_("Postamp Tube1"),
	0
	};

Dsp::Dsp()
	: PluginLV2() {
	version = PLUGINLV2_VERSION;
	id = "ampmodul";
	name = N_("Postamp");
	mono_audio = 0;
	stereo_audio = compute_static;
	set_samplerate = init_static;
	activate_plugin = 0;
	connect_ports = connect_static;
	clear_state = clear_state_f_static;
	delete_instance = del_instance;
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int i=0; i<2; i++) fRec5[i] = 0;
	for (int i=0; i<2; i++) fRec11[i] = 0;
	for (int i=0; i<2; i++) fRec17[i] = 0;
	for (int i=0; i<2; i++) fVec0[i] = 0;
	for (int i=0; i<2; i++) fRec16[i] = 0;
	for (int i=0; i<3; i++) fRec15[i] = 0;
	for (int i=0; i<2; i++) fVec1[i] = 0;
	for (int i=0; i<2; i++) fRec18[i] = 0;
	for (int i=0; i<2; i++) fRec14[i] = 0;
	for (int i=0; i<2; i++) fRec13[i] = 0;
	for (int i=0; i<2; i++) fRec12[i] = 0;
	for (int i=0; i<2; i++) fVec2[i] = 0;
	for (int i=0; i<2; i++) fRec10[i] = 0;
	for (int i=0; i<3; i++) fRec9[i] = 0;
	for (int i=0; i<2; i++) fVec3[i] = 0;
	for (int i=0; i<2; i++) fRec19[i] = 0;
	for (int i=0; i<2; i++) fRec8[i] = 0;
	for (int i=0; i<2; i++) fRec7[i] = 0;
	for (int i=0; i<2; i++) fRec6[i] = 0;
	for (int i=0; i<2; i++) fVec4[i] = 0;
	for (int i=0; i<2; i++) fRec4[i] = 0;
	for (int i=0; i<3; i++) fRec3[i] = 0;
	for (int i=0; i<2; i++) fVec5[i] = 0;
	for (int i=0; i<2; i++) fRec20[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<2; i++) fRec1[i] = 0;
	for (int i=0; i<6; i++) fRec0[i] = 0;
	for (int i=0; i<6; i++) fRec21[i] = 0;
	for (int i=0; i<2; i++) fVec6[i] = 0;
	for (int i=0; i<2; i++) fRec36[i] = 0;
	for (int i=0; i<3; i++) fRec35[i] = 0;
	for (int i=0; i<2; i++) fVec7[i] = 0;
	for (int i=0; i<2; i++) fRec37[i] = 0;
	for (int i=0; i<2; i++) fRec34[i] = 0;
	for (int i=0; i<2; i++) fRec33[i] = 0;
	for (int i=0; i<2; i++) fRec32[i] = 0;
	for (int i=0; i<2; i++) fVec8[i] = 0;
	for (int i=0; i<2; i++) fRec31[i] = 0;
	for (int i=0; i<3; i++) fRec30[i] = 0;
	for (int i=0; i<2; i++) fVec9[i] = 0;
	for (int i=0; i<2; i++) fRec38[i] = 0;
	for (int i=0; i<2; i++) fRec29[i] = 0;
	for (int i=0; i<2; i++) fRec28[i] = 0;
	for (int i=0; i<2; i++) fRec27[i] = 0;
	for (int i=0; i<2; i++) fVec10[i] = 0;
	for (int i=0; i<2; i++) fRec26[i] = 0;
	for (int i=0; i<3; i++) fRec25[i] = 0;
	for (int i=0; i<2; i++) fVec11[i] = 0;
	for (int i=0; i<2; i++) fRec39[i] = 0;
	for (int i=0; i<2; i++) fRec24[i] = 0;
	for (int i=0; i<2; i++) fRec23[i] = 0;
	for (int i=0; i<6; i++) fRec22[i] = 0;
	for (int i=0; i<6; i++) fRec40[i] = 0;
}

void Dsp::clear_state_f_static(PluginLV2 *p)
{
	static_cast<Dsp*>(p)->clear_state_f();
}

inline void Dsp::init(uint32_t samplingFreq)
{
	fSamplingFreq = samplingFreq;
	iConst0 = min(192000, max(1, fSamplingFreq));
	fConst1 = (1.0 / tan((270.1769682087222 / double(iConst0))));
	fConst2 = (1 + fConst1);
	fConst3 = (1.0 / fConst2);
	fConst4 = (0 - ((1 - fConst1) / fConst2));
	fConst5 = (1.0 / tan((97.38937226128358 / double(iConst0))));
	fConst6 = (0 - fConst5);
	fConst7 = (1 + fConst5);
	fConst8 = (0.025 / fConst7);
	fConst9 = (0 - ((1 - fConst5) / fConst7));
	fConst10 = (1.0 / tan((20517.741620594938 / double(iConst0))));
	fConst11 = (1 + fConst10);
	fConst12 = (1.0 / fConst11);
	fConst13 = (0 - ((1 - fConst10) / fConst11));
	fConst14 = (1.0 / tan((414.6902302738527 / double(iConst0))));
	fConst15 = (1 + fConst14);
	fConst16 = (1.0 / fConst15);
	fConst17 = (0 - ((1 - fConst14) / fConst15));
	fConst18 = (1.0 / tan((609.4689747964198 / double(iConst0))));
	fConst19 = (1 + fConst18);
	fConst20 = (1.0 / fConst19);
	fConst21 = (0 - ((1 - fConst18) / fConst19));
	clear_state_f();
}

void Dsp::init_static(uint32_t samplingFreq, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->init(samplingFreq);
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *input1, FAUSTFLOAT *output0, FAUSTFLOAT *output1)
{
#define fslider0 (*fslider0_)
#define fslider1 (*fslider1_)
#define fslider2 (*fslider2_)
#define fslider3 (*fslider3_)
#define fslider4 (*fslider4_)
#define fslider5 (*fslider5_)
	double 	fSlow0 = double(fslider0);
	double 	fSlow1 = (0.0010000000000000009 * pow(10,(0.05 * double(fslider1))));
	double 	fSlow2 = (0.0010000000000000009 * pow(10,(0.05 * double(fslider2))));
	double 	fSlow3 = (0.0010000000000000009 * pow(10,(0.05 * double(fslider3))));
	double 	fSlow4 = double(fslider4);
	double 	fSlow5 = (1 - max((double)0, (0 - fSlow4)));
	double 	fSlow6 = double(fslider5);
	double 	fSlow7 = (1 - max((double)0, fSlow4));
	for (int i=0; i<count; i++) {
		fRec5[0] = ((0.999 * fRec5[1]) + fSlow1);
		fRec11[0] = ((0.999 * fRec11[1]) + fSlow2);
		fRec17[0] = ((0.999 * fRec17[1]) + fSlow3);
		double fTemp0 = (double)input0[i];
		double fTemp1 = (1e-15 + (fTemp0 * fRec17[0]));
		fVec0[0] = fTemp1;
		fRec16[0] = ((0.9302847925323914 * (fVec0[0] + fVec0[1])) - (0.8605695850647829 * fRec16[1]));
		fRec15[0] = (fRec16[0] - ((1.8405051250752198 * fRec15[1]) + (0.8612942439318627 * fRec15[2])));
		double fTemp2 = (1e-15 + (0.027 * fRec14[1]));
		fVec1[0] = fTemp2;
		fRec18[0] = ((fConst4 * fRec18[1]) + (fConst3 * (fVec1[0] + fVec1[1])));
		fRec14[0] = (Ftube(TUBE_TABLE_6V6_68k, ((fRec18[0] + (0.9254498422517706 * (fRec15[2] + (fRec15[0] + (2.0 * fRec15[1]))))) - 2.29615)) - 164.9574074074074);
		fRec13[0] = ((fConst9 * fRec13[1]) + (fConst8 * ((fConst5 * fRec14[0]) + (fConst6 * fRec14[1]))));
		fRec12[0] = ((fConst13 * fRec12[1]) + (fConst12 * (fRec13[0] + fRec13[1])));
		double fTemp3 = (1e-15 + (fRec12[0] * fRec11[0]));
		fVec2[0] = fTemp3;
		fRec10[0] = ((0.9302847925323914 * (fVec2[0] + fVec2[1])) - (0.8605695850647829 * fRec10[1]));
		fRec9[0] = (fRec10[0] - ((1.8405051250752198 * fRec9[1]) + (0.8612942439318627 * fRec9[2])));
		double fTemp4 = (1e-15 + (0.015 * fRec8[1]));
		fVec3[0] = fTemp4;
		fRec19[0] = ((fConst17 * fRec19[1]) + (fConst16 * (fVec3[0] + fVec3[1])));
		fRec8[0] = (Ftube(TUBE_TABLE_6V6_250k, ((fRec19[0] + (0.9254498422517706 * (fRec9[2] + (fRec9[0] + (2.0 * fRec9[1]))))) - 1.675587)) - 138.2942);
		fRec7[0] = ((fConst9 * fRec7[1]) + (fConst8 * ((fConst5 * fRec8[0]) + (fConst6 * fRec8[1]))));
		fRec6[0] = ((fConst13 * fRec6[1]) + (fConst12 * (fRec7[0] + fRec7[1])));
		double fTemp5 = (1e-15 + (fRec6[0] * fRec5[0]));
		fVec4[0] = fTemp5;
		fRec4[0] = ((0.9302847925323914 * (fVec4[0] + fVec4[1])) - (0.8605695850647829 * fRec4[1]));
		fRec3[0] = (fRec4[0] - ((1.8405051250752198 * fRec3[1]) + (0.8612942439318627 * fRec3[2])));
		double fTemp6 = (1e-15 + (0.0082 * fRec2[1]));
		fVec5[0] = fTemp6;
		fRec20[0] = ((fConst21 * fRec20[1]) + (fConst20 * (fVec5[0] + fVec5[1])));
		fRec2[0] = (Ftube(TUBE_TABLE_6V6_250k, ((fRec20[0] + (0.9254498422517706 * (fRec3[2] + (fRec3[0] + (2.0 * fRec3[1]))))) - 1.130462)) - 112.13878048780487);
		fRec1[0] = ((fConst9 * fRec1[1]) + (fConst8 * ((fConst5 * fRec2[0]) + (fConst6 * fRec2[1]))));
		fRec0[0] = (fRec1[0] - (fSlow0 * fRec0[5]));
		fRec21[0] = (fTemp0 - (fSlow6 * fRec21[5]));
		output0[i] = (FAUSTFLOAT)((fSlow7 * fRec21[0]) + (fSlow5 * fRec0[0]));
		double fTemp7 = (double)input1[i];
		double fTemp8 = (1e-15 + (fTemp7 * fRec17[0]));
		fVec6[0] = fTemp8;
		fRec36[0] = ((0.9302847925323914 * (fVec6[0] + fVec6[1])) - (0.8605695850647829 * fRec36[1]));
		fRec35[0] = (fRec36[0] - ((1.8405051250752198 * fRec35[1]) + (0.8612942439318627 * fRec35[2])));
		double fTemp9 = (1e-15 + (0.027 * fRec34[1]));
		fVec7[0] = fTemp9;
		fRec37[0] = ((fConst4 * fRec37[1]) + (fConst3 * (fVec7[0] + fVec7[1])));
		fRec34[0] = (Ftube(TUBE_TABLE_6V6_68k, ((fRec37[0] + (0.9254498422517706 * (fRec35[2] + (fRec35[0] + (2.0 * fRec35[1]))))) - 2.29615)) - 164.9574074074074);
		fRec33[0] = ((fConst9 * fRec33[1]) + (fConst8 * ((fConst5 * fRec34[0]) + (fConst6 * fRec34[1]))));
		fRec32[0] = ((fConst13 * fRec32[1]) + (fConst12 * (fRec33[0] + fRec33[1])));
		double fTemp10 = (1e-15 + (fRec11[0] * fRec32[0]));
		fVec8[0] = fTemp10;
		fRec31[0] = ((0.9302847925323914 * (fVec8[0] + fVec8[1])) - (0.8605695850647829 * fRec31[1]));
		fRec30[0] = (fRec31[0] - ((1.8405051250752198 * fRec30[1]) + (0.8612942439318627 * fRec30[2])));
		double fTemp11 = (1e-15 + (0.015 * fRec29[1]));
		fVec9[0] = fTemp11;
		fRec38[0] = ((fConst17 * fRec38[1]) + (fConst16 * (fVec9[0] + fVec9[1])));
		fRec29[0] = (Ftube(TUBE_TABLE_6V6_250k, ((fRec38[0] + (0.9254498422517706 * (fRec30[2] + (fRec30[0] + (2.0 * fRec30[1]))))) - 1.675587)) - 138.2942);
		fRec28[0] = ((fConst9 * fRec28[1]) + (fConst8 * ((fConst5 * fRec29[0]) + (fConst6 * fRec29[1]))));
		fRec27[0] = ((fConst13 * fRec27[1]) + (fConst12 * (fRec28[0] + fRec28[1])));
		double fTemp12 = (1e-15 + (fRec5[0] * fRec27[0]));
		fVec10[0] = fTemp12;
		fRec26[0] = ((0.9302847925323914 * (fVec10[0] + fVec10[1])) - (0.8605695850647829 * fRec26[1]));
		fRec25[0] = (fRec26[0] - ((1.8405051250752198 * fRec25[1]) + (0.8612942439318627 * fRec25[2])));
		double fTemp13 = (1e-15 + (0.0082 * fRec24[1]));
		fVec11[0] = fTemp13;
		fRec39[0] = ((fConst21 * fRec39[1]) + (fConst20 * (fVec11[0] + fVec11[1])));
		fRec24[0] = (Ftube(TUBE_TABLE_6V6_250k, ((fRec39[0] + (0.9254498422517706 * (fRec25[2] + (fRec25[0] + (2.0 * fRec25[1]))))) - 1.130462)) - 112.13878048780487);
		fRec23[0] = ((fConst9 * fRec23[1]) + (fConst8 * ((fConst5 * fRec24[0]) + (fConst6 * fRec24[1]))));
		fRec22[0] = (fRec23[0] - (fSlow0 * fRec22[5]));
		fRec40[0] = (fTemp7 - (fSlow6 * fRec40[5]));
		output1[i] = (FAUSTFLOAT)((fSlow7 * fRec40[0]) + (fSlow5 * fRec22[0]));
		// post processing
		for (int i=5; i>0; i--) fRec40[i] = fRec40[i-1];
		for (int i=5; i>0; i--) fRec22[i] = fRec22[i-1];
		fRec23[1] = fRec23[0];
		fRec24[1] = fRec24[0];
		fRec39[1] = fRec39[0];
		fVec11[1] = fVec11[0];
		fRec25[2] = fRec25[1]; fRec25[1] = fRec25[0];
		fRec26[1] = fRec26[0];
		fVec10[1] = fVec10[0];
		fRec27[1] = fRec27[0];
		fRec28[1] = fRec28[0];
		fRec29[1] = fRec29[0];
		fRec38[1] = fRec38[0];
		fVec9[1] = fVec9[0];
		fRec30[2] = fRec30[1]; fRec30[1] = fRec30[0];
		fRec31[1] = fRec31[0];
		fVec8[1] = fVec8[0];
		fRec32[1] = fRec32[0];
		fRec33[1] = fRec33[0];
		fRec34[1] = fRec34[0];
		fRec37[1] = fRec37[0];
		fVec7[1] = fVec7[0];
		fRec35[2] = fRec35[1]; fRec35[1] = fRec35[0];
		fRec36[1] = fRec36[0];
		fVec6[1] = fVec6[0];
		for (int i=5; i>0; i--) fRec21[i] = fRec21[i-1];
		for (int i=5; i>0; i--) fRec0[i] = fRec0[i-1];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec20[1] = fRec20[0];
		fVec5[1] = fVec5[0];
		fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fVec4[1] = fVec4[0];
		fRec6[1] = fRec6[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec19[1] = fRec19[0];
		fVec3[1] = fVec3[0];
		fRec9[2] = fRec9[1]; fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fVec2[1] = fVec2[0];
		fRec12[1] = fRec12[0];
		fRec13[1] = fRec13[0];
		fRec14[1] = fRec14[0];
		fRec18[1] = fRec18[0];
		fVec1[1] = fVec1[0];
		fRec15[2] = fRec15[1]; fRec15[1] = fRec15[0];
		fRec16[1] = fRec16[0];
		fVec0[1] = fVec0[0];
		fRec17[1] = fRec17[0];
		fRec11[1] = fRec11[0];
		fRec5[1] = fRec5[0];
	}
#undef fslider0
#undef fslider1
#undef fslider2
#undef fslider3
#undef fslider4
#undef fslider5
}

void __rt_func Dsp::compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *input1, FAUSTFLOAT *output0, FAUSTFLOAT *output1, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->compute(count, input0, input1, output0, output1);
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case FEEDBAC: 
		fslider5_ = (float*)data; // , 0.0, -1.0, 1.0, 0.01 
		break;
	case FEEDBACK: 
		fslider0_ = (float*)data; // , 0.0, -1.0, 1.0, 0.01 
		break;
	case LEVEL: 
		fslider3_ = (float*)data; // , -2e+01, -4e+01, 4.0, 0.1 
		break;
	case TUBE1: 
		fslider2_ = (float*)data; // , 6.0, -2e+01, 2e+01, 0.1 
		break;
	case TUBE2: 
		fslider1_ = (float*)data; // , 6.0, -2e+01, 2e+01, 0.1 
		break;
	case WET_DRY: 
		fslider4_ = (float*)data; // , 0.0, -1.0, 1.0, 0.1 
		break;
	default:
		break;
	}
}

void Dsp::connect_static(uint32_t port,void* data, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->connect(port, data);
}


PluginLV2 *plugin() {
	return new Dsp();
}

void Dsp::del_instance(PluginLV2 *p)
{
	delete static_cast<Dsp*>(p);
}

/*
typedef enum
{
   FEEDBAC, 
   FEEDBACK, 
   LEVEL, 
   TUBE1, 
   TUBE2, 
   WET_DRY, 
} PortIndex;
*/

} // end namespace gx_ampmodul
