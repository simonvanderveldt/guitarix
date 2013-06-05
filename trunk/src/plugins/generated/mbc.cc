// generated from file '../src/plugins/mbc.dsp' by dsp2cc:
// Code generated with Faust 0.9.46 (http://faust.grame.fr)

#include "gx_faust_support.h"
#include "gx_plugin.h"

namespace pluginlib {
namespace mbc {

class Dsp: public PluginDef {
private:
	int fSamplingFreq;
	FAUSTFLOAT 	fslider0;
	int 	iConst0;
	double 	fConst1;
	double 	fConst2;
	FAUSTFLOAT 	fslider1;
	double 	fConst3;
	FAUSTFLOAT 	fslider2;
	double 	fVec0[2];
	double 	fRec6[2];
	double 	fRec5[3];
	double 	fVec1[2];
	double 	fRec4[2];
	double 	fRec3[3];
	FAUSTFLOAT 	fslider3;
	FAUSTFLOAT 	fslider4;
	double 	fRec2[2];
	double 	fRec1[2];
	FAUSTFLOAT 	fslider5;
	FAUSTFLOAT 	fslider6;
	double 	fRec0[2];
	FAUSTFLOAT 	fslider7;
	double 	fRec7[2];
	FAUSTFLOAT 	fslider8;
	double 	fRec12[2];
	double 	fRec11[3];
	FAUSTFLOAT 	fslider9;
	FAUSTFLOAT 	fslider10;
	double 	fRec10[2];
	double 	fRec9[2];
	FAUSTFLOAT 	fslider11;
	FAUSTFLOAT 	fslider12;
	double 	fRec8[2];
	FAUSTFLOAT 	fslider13;
	double 	fRec13[2];
	FAUSTFLOAT 	fslider14;
	FAUSTFLOAT 	fslider15;
	double 	fRec20[2];
	double 	fRec19[3];
	double 	fVec2[2];
	double 	fRec18[2];
	double 	fRec17[3];
	FAUSTFLOAT 	fslider16;
	FAUSTFLOAT 	fslider17;
	double 	fRec16[2];
	double 	fRec15[2];
	FAUSTFLOAT 	fslider18;
	FAUSTFLOAT 	fslider19;
	double 	fRec14[2];
	FAUSTFLOAT 	fslider20;
	double 	fRec21[2];
	FAUSTFLOAT 	fslider21;
	FAUSTFLOAT 	fslider22;
	double 	fRec28[2];
	double 	fRec27[3];
	double 	fVec3[2];
	double 	fRec26[2];
	double 	fRec25[3];
	FAUSTFLOAT 	fslider23;
	FAUSTFLOAT 	fslider24;
	double 	fRec24[2];
	double 	fRec23[2];
	FAUSTFLOAT 	fslider25;
	FAUSTFLOAT 	fslider26;
	double 	fRec22[2];
	FAUSTFLOAT 	fslider27;
	double 	fRec29[2];
	FAUSTFLOAT 	fslider28;
	double 	fRec34[2];
	double 	fRec33[3];
	FAUSTFLOAT 	fslider29;
	FAUSTFLOAT 	fslider30;
	double 	fRec32[2];
	double 	fRec31[2];
	FAUSTFLOAT 	fslider31;
	FAUSTFLOAT 	fslider32;
	double 	fRec30[2];
	FAUSTFLOAT 	fslider33;
	double 	fRec35[2];
	void clear_state_f();
	int load_ui_f(const UiBuilder& b);
	void init(unsigned int samplingFreq);
	void compute(int count, float *input0, float *output0);
	int register_par(const ParamReg& reg);

	static void clear_state_f_static(PluginDef*);
	static int load_ui_f_static(const UiBuilder& b);
	static void init_static(unsigned int samplingFreq, PluginDef*);
	static void compute_static(int count, float *input0, float *output0, PluginDef*);
	static int register_params_static(const ParamReg& reg);
	static void del_instance(PluginDef *p);
public:
	Dsp();
	~Dsp();
};



Dsp::Dsp()
	: PluginDef() {
	version = PLUGINDEF_VERSION;
	flags = 0;
	id = "mbc";
	name = N_("Multi Band Compressor");
	groups = 0;
	description = N_("Multi Band Compressor contributed by kokoko3k"); // description (tooltip)
	category = N_("Guitar Effects");       // category
	shortname = N_("MB Compresssor");     // shortname
	mono_audio = compute_static;
	stereo_audio = 0;
	set_samplerate = init_static;
	activate_plugin = 0;
	register_params = register_params_static;
	load_ui = load_ui_f_static;
	clear_state = clear_state_f_static;
	delete_instance = del_instance;
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int i=0; i<2; i++) fVec0[i] = 0;
	for (int i=0; i<2; i++) fRec6[i] = 0;
	for (int i=0; i<3; i++) fRec5[i] = 0;
	for (int i=0; i<2; i++) fVec1[i] = 0;
	for (int i=0; i<2; i++) fRec4[i] = 0;
	for (int i=0; i<3; i++) fRec3[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<2; i++) fRec1[i] = 0;
	for (int i=0; i<2; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) fRec7[i] = 0;
	for (int i=0; i<2; i++) fRec12[i] = 0;
	for (int i=0; i<3; i++) fRec11[i] = 0;
	for (int i=0; i<2; i++) fRec10[i] = 0;
	for (int i=0; i<2; i++) fRec9[i] = 0;
	for (int i=0; i<2; i++) fRec8[i] = 0;
	for (int i=0; i<2; i++) fRec13[i] = 0;
	for (int i=0; i<2; i++) fRec20[i] = 0;
	for (int i=0; i<3; i++) fRec19[i] = 0;
	for (int i=0; i<2; i++) fVec2[i] = 0;
	for (int i=0; i<2; i++) fRec18[i] = 0;
	for (int i=0; i<3; i++) fRec17[i] = 0;
	for (int i=0; i<2; i++) fRec16[i] = 0;
	for (int i=0; i<2; i++) fRec15[i] = 0;
	for (int i=0; i<2; i++) fRec14[i] = 0;
	for (int i=0; i<2; i++) fRec21[i] = 0;
	for (int i=0; i<2; i++) fRec28[i] = 0;
	for (int i=0; i<3; i++) fRec27[i] = 0;
	for (int i=0; i<2; i++) fVec3[i] = 0;
	for (int i=0; i<2; i++) fRec26[i] = 0;
	for (int i=0; i<3; i++) fRec25[i] = 0;
	for (int i=0; i<2; i++) fRec24[i] = 0;
	for (int i=0; i<2; i++) fRec23[i] = 0;
	for (int i=0; i<2; i++) fRec22[i] = 0;
	for (int i=0; i<2; i++) fRec29[i] = 0;
	for (int i=0; i<2; i++) fRec34[i] = 0;
	for (int i=0; i<3; i++) fRec33[i] = 0;
	for (int i=0; i<2; i++) fRec32[i] = 0;
	for (int i=0; i<2; i++) fRec31[i] = 0;
	for (int i=0; i<2; i++) fRec30[i] = 0;
	for (int i=0; i<2; i++) fRec35[i] = 0;
}

void Dsp::clear_state_f_static(PluginDef *p)
{
	static_cast<Dsp*>(p)->clear_state_f();
}

inline void Dsp::init(unsigned int samplingFreq)
{
	fSamplingFreq = samplingFreq;
	iConst0 = min(192000, max(1, fSamplingFreq));
	fConst1 = (2.0 / iConst0);
	fConst2 = (1.0 / iConst0);
	fConst3 = (3.141592653589793 / iConst0);
	clear_state_f();
}

void Dsp::init_static(unsigned int samplingFreq, PluginDef *p)
{
	static_cast<Dsp*>(p)->init(samplingFreq);
}

void always_inline Dsp::compute(int count, float *input0, float *output0)
{
	double 	fSlow0 = fslider0;
	double 	fSlow1 = exp((0 - (fConst1 / fSlow0)));
	double 	fSlow2 = exp((0 - (fConst2 / fSlow0)));
	double 	fSlow3 = tan((fConst3 * fslider1));
	double 	fSlow4 = (1.0 / faustpower<2>(fSlow3));
	double 	fSlow5 = (2 * (1 - fSlow4));
	double 	fSlow6 = (1.0 / fSlow3);
	double 	fSlow7 = (1 + ((fSlow6 - 1.0000000000000004) / fSlow3));
	double 	fSlow8 = (1 + ((1.0000000000000004 + fSlow6) / fSlow3));
	double 	fSlow9 = (1.0 / fSlow8);
	double 	fSlow10 = (1 + fSlow6);
	double 	fSlow11 = (0 - ((1 - fSlow6) / fSlow10));
	double 	fSlow12 = tan((fConst3 * fslider2));
	double 	fSlow13 = (1.0 / faustpower<2>(fSlow12));
	double 	fSlow14 = (2 * (1 - fSlow13));
	double 	fSlow15 = (1.0 / fSlow12);
	double 	fSlow16 = (1 + ((fSlow15 - 1.0000000000000004) / fSlow12));
	double 	fSlow17 = (1 + ((1.0000000000000004 + fSlow15) / fSlow12));
	double 	fSlow18 = (1.0 / fSlow17);
	double 	fSlow19 = (1 + fSlow15);
	double 	fSlow20 = (0 - ((1 - fSlow15) / fSlow19));
	double 	fSlow21 = (1.0 / fSlow19);
	double 	fSlow22 = (1.0 / (fSlow17 * fSlow3));
	double 	fSlow23 = (0 - fSlow6);
	double 	fSlow24 = (1.0 / fSlow10);
	double 	fSlow25 = (2 * (0 - fSlow4));
	double 	fSlow26 = fslider3;
	double 	fSlow27 = max(0, (fSlow26 - 1));
	int 	iSlow28 = int(fSlow27);
	double 	fSlow29 = exp((0 - (fConst2 / fslider4)));
	double 	fSlow30 = (1.0 - fSlow29);
	double 	fSlow31 = (1.0 - fSlow2);
	double 	fSlow32 = fslider5;
	double 	fSlow33 = ((1.0 - fSlow1) * ((1.0 / double(fslider6)) - 1));
	double 	fSlow34 = (0.0010000000000000009 * pow(10,(0.05 * ((fSlow32 - fslider7) * fabs((fSlow27 - 1))))));
	double 	fSlow35 = fabs((max(0, (fSlow26 - 2)) - 1));
	double 	fSlow36 = fslider8;
	double 	fSlow37 = exp((0 - (fConst1 / fSlow36)));
	double 	fSlow38 = exp((0 - (fConst2 / fSlow36)));
	double 	fSlow39 = (0 - fSlow15);
	double 	fSlow40 = (2 * (0 - fSlow13));
	double 	fSlow41 = fslider9;
	double 	fSlow42 = max(0, (fSlow41 - 1));
	int 	iSlow43 = int(fSlow42);
	double 	fSlow44 = exp((0 - (fConst2 / fslider10)));
	double 	fSlow45 = (1.0 - fSlow44);
	double 	fSlow46 = (1.0 - fSlow38);
	double 	fSlow47 = fslider11;
	double 	fSlow48 = ((1.0 - fSlow37) * ((1.0 / double(fslider12)) - 1));
	double 	fSlow49 = (0.0010000000000000009 * pow(10,(0.05 * ((fSlow47 - fslider13) * fabs((fSlow42 - 1))))));
	double 	fSlow50 = fabs((max(0, (fSlow41 - 2)) - 1));
	double 	fSlow51 = fslider14;
	double 	fSlow52 = exp((0 - (fConst1 / fSlow51)));
	double 	fSlow53 = exp((0 - (fConst2 / fSlow51)));
	double 	fSlow54 = tan((fConst3 * fslider15));
	double 	fSlow55 = (1.0 / faustpower<2>(fSlow54));
	double 	fSlow56 = (2 * (1 - fSlow55));
	double 	fSlow57 = (1.0 / fSlow54);
	double 	fSlow58 = (1 + ((fSlow57 - 1.0000000000000004) / fSlow54));
	double 	fSlow59 = (1 + ((1.0000000000000004 + fSlow57) / fSlow54));
	double 	fSlow60 = (1.0 / fSlow59);
	double 	fSlow61 = (1 + fSlow57);
	double 	fSlow62 = (0 - ((1 - fSlow57) / fSlow61));
	double 	fSlow63 = (1.0 / (fSlow8 * fSlow54));
	double 	fSlow64 = (0 - fSlow57);
	double 	fSlow65 = (1.0 / fSlow61);
	double 	fSlow66 = (2 * (0 - fSlow55));
	double 	fSlow67 = fslider16;
	double 	fSlow68 = max(0, (fSlow67 - 1));
	int 	iSlow69 = int(fSlow68);
	double 	fSlow70 = exp((0 - (fConst2 / fslider17)));
	double 	fSlow71 = (1.0 - fSlow70);
	double 	fSlow72 = (1.0 - fSlow53);
	double 	fSlow73 = fslider18;
	double 	fSlow74 = ((1.0 - fSlow52) * ((1.0 / double(fslider19)) - 1));
	double 	fSlow75 = (0.0010000000000000009 * pow(10,(0.05 * ((fSlow73 - fslider20) * fabs((fSlow68 - 1))))));
	double 	fSlow76 = fabs((max(0, (fSlow67 - 2)) - 1));
	double 	fSlow77 = fslider21;
	double 	fSlow78 = exp((0 - (fConst1 / fSlow77)));
	double 	fSlow79 = exp((0 - (fConst2 / fSlow77)));
	double 	fSlow80 = tan((fConst3 * fslider22));
	double 	fSlow81 = (1.0 / faustpower<2>(fSlow80));
	double 	fSlow82 = (2 * (1 - fSlow81));
	double 	fSlow83 = (1.0 / fSlow80);
	double 	fSlow84 = (1 + ((fSlow83 - 1.0000000000000004) / fSlow80));
	double 	fSlow85 = (1.0 / (1 + ((1.0000000000000004 + fSlow83) / fSlow80)));
	double 	fSlow86 = (1 + fSlow83);
	double 	fSlow87 = (0 - ((1 - fSlow83) / fSlow86));
	double 	fSlow88 = (1.0 / (fSlow59 * fSlow80));
	double 	fSlow89 = (0 - fSlow83);
	double 	fSlow90 = (1.0 / fSlow86);
	double 	fSlow91 = (2 * (0 - fSlow81));
	double 	fSlow92 = fslider23;
	double 	fSlow93 = max(0, (fSlow92 - 1));
	int 	iSlow94 = int(fSlow93);
	double 	fSlow95 = exp((0 - (fConst2 / fslider24)));
	double 	fSlow96 = (1.0 - fSlow95);
	double 	fSlow97 = (1.0 - fSlow79);
	double 	fSlow98 = fslider25;
	double 	fSlow99 = ((1.0 - fSlow78) * ((1.0 / double(fslider26)) - 1));
	double 	fSlow100 = (0.0010000000000000009 * pow(10,(0.05 * ((fSlow98 - fslider27) * fabs((fSlow93 - 1))))));
	double 	fSlow101 = fabs((max(0, (fSlow92 - 2)) - 1));
	double 	fSlow102 = fslider28;
	double 	fSlow103 = exp((0 - (fConst1 / fSlow102)));
	double 	fSlow104 = exp((0 - (fConst2 / fSlow102)));
	double 	fSlow105 = fslider29;
	double 	fSlow106 = max(0, (fSlow105 - 1));
	int 	iSlow107 = int(fSlow106);
	double 	fSlow108 = exp((0 - (fConst2 / fslider30)));
	double 	fSlow109 = (1.0 - fSlow108);
	double 	fSlow110 = (1.0 - fSlow104);
	double 	fSlow111 = fslider31;
	double 	fSlow112 = ((1.0 - fSlow103) * ((1.0 / double(fslider32)) - 1));
	double 	fSlow113 = (0.0010000000000000009 * pow(10,(0.05 * ((fSlow111 - fslider33) * fabs((fSlow106 - 1))))));
	double 	fSlow114 = fabs((max(0, (fSlow105 - 2)) - 1));
	for (int i=0; i<count; i++) {
		double fTemp0 = (double)input0[i];
		fVec0[0] = fTemp0;
		double fTemp1 = (fVec0[0] + fVec0[1]);
		fRec6[0] = ((fSlow21 * fTemp1) + (fSlow20 * fRec6[1]));
		fRec5[0] = (fRec6[0] - (fSlow18 * ((fSlow16 * fRec5[2]) + (fSlow14 * fRec5[1]))));
		double fTemp2 = (fRec5[2] + (fRec5[0] + (2 * fRec5[1])));
		fVec1[0] = (fSlow18 * fTemp2);
		fRec4[0] = ((fSlow24 * ((fSlow23 * fVec1[1]) + (fSlow22 * fTemp2))) + (fSlow11 * fRec4[1]));
		fRec3[0] = (fRec4[0] - (fSlow9 * ((fSlow7 * fRec3[2]) + (fSlow5 * fRec3[1]))));
		double fTemp3 = (fSlow9 * (((fSlow4 * fRec3[0]) + (fSlow25 * fRec3[1])) + (fSlow4 * fRec3[2])));
		double fTemp4 = ((iSlow28)?0:fTemp3);
		double fTemp5 = fabs(fTemp4);
		fRec2[0] = ((fSlow30 * fTemp5) + (fSlow29 * max(fTemp5, fRec2[1])));
		fRec1[0] = ((fSlow31 * fRec2[0]) + (fSlow2 * fRec1[1]));
		fRec0[0] = ((fSlow33 * max((fSlow32 + (20 * log10(fRec1[0]))), 0)) + (fSlow1 * fRec0[1]));
		fRec7[0] = (fSlow34 + (0.999 * fRec7[1]));
		fRec12[0] = ((fSlow21 * ((fSlow39 * fVec0[1]) + (fSlow15 * fVec0[0]))) + (fSlow20 * fRec12[1]));
		fRec11[0] = (fRec12[0] - (fSlow18 * ((fSlow16 * fRec11[2]) + (fSlow14 * fRec11[1]))));
		double fTemp6 = (fSlow18 * (((fSlow13 * fRec11[0]) + (fSlow40 * fRec11[1])) + (fSlow13 * fRec11[2])));
		double fTemp7 = ((iSlow43)?0:fTemp6);
		double fTemp8 = fabs(fTemp7);
		fRec10[0] = ((fSlow45 * fTemp8) + (fSlow44 * max(fTemp8, fRec10[1])));
		fRec9[0] = ((fSlow46 * fRec10[0]) + (fSlow38 * fRec9[1]));
		fRec8[0] = ((fSlow48 * max((fSlow47 + (20 * log10(fRec9[0]))), 0)) + (fSlow37 * fRec8[1]));
		fRec13[0] = (fSlow49 + (0.999 * fRec13[1]));
		fRec20[0] = ((fSlow24 * fTemp1) + (fSlow11 * fRec20[1]));
		fRec19[0] = (fRec20[0] - (fSlow9 * ((fSlow7 * fRec19[2]) + (fSlow5 * fRec19[1]))));
		double fTemp9 = (fRec19[2] + (fRec19[0] + (2 * fRec19[1])));
		fVec2[0] = (fSlow9 * fTemp9);
		fRec18[0] = ((fSlow65 * ((fSlow64 * fVec2[1]) + (fSlow63 * fTemp9))) + (fSlow62 * fRec18[1]));
		fRec17[0] = (fRec18[0] - (fSlow60 * ((fSlow58 * fRec17[2]) + (fSlow56 * fRec17[1]))));
		double fTemp10 = (fSlow60 * (((fSlow55 * fRec17[0]) + (fSlow66 * fRec17[1])) + (fSlow55 * fRec17[2])));
		double fTemp11 = ((iSlow69)?0:fTemp10);
		double fTemp12 = fabs(fTemp11);
		fRec16[0] = ((fSlow71 * fTemp12) + (fSlow70 * max(fTemp12, fRec16[1])));
		fRec15[0] = ((fSlow72 * fRec16[0]) + (fSlow53 * fRec15[1]));
		fRec14[0] = ((fSlow74 * max((fSlow73 + (20 * log10(fRec15[0]))), 0)) + (fSlow52 * fRec14[1]));
		fRec21[0] = (fSlow75 + (0.999 * fRec21[1]));
		fRec28[0] = ((fSlow65 * fTemp1) + (fSlow62 * fRec28[1]));
		fRec27[0] = (fRec28[0] - (fSlow60 * ((fSlow58 * fRec27[2]) + (fSlow56 * fRec27[1]))));
		double fTemp13 = (fRec27[2] + (fRec27[0] + (2 * fRec27[1])));
		fVec3[0] = (fSlow60 * fTemp13);
		fRec26[0] = ((fSlow90 * ((fSlow89 * fVec3[1]) + (fSlow88 * fTemp13))) + (fSlow87 * fRec26[1]));
		fRec25[0] = (fRec26[0] - (fSlow85 * ((fSlow84 * fRec25[2]) + (fSlow82 * fRec25[1]))));
		double fTemp14 = (fSlow85 * (((fSlow81 * fRec25[0]) + (fSlow91 * fRec25[1])) + (fSlow81 * fRec25[2])));
		double fTemp15 = ((iSlow94)?0:fTemp14);
		double fTemp16 = fabs(fTemp15);
		fRec24[0] = ((fSlow96 * fTemp16) + (fSlow95 * max(fTemp16, fRec24[1])));
		fRec23[0] = ((fSlow97 * fRec24[0]) + (fSlow79 * fRec23[1]));
		fRec22[0] = ((fSlow99 * max((fSlow98 + (20 * log10(fRec23[0]))), 0)) + (fSlow78 * fRec22[1]));
		fRec29[0] = (fSlow100 + (0.999 * fRec29[1]));
		fRec34[0] = ((fSlow90 * fTemp1) + (fSlow87 * fRec34[1]));
		fRec33[0] = (fRec34[0] - (fSlow85 * ((fSlow84 * fRec33[2]) + (fSlow82 * fRec33[1]))));
		double fTemp17 = (fSlow85 * (fRec33[2] + (fRec33[0] + (2 * fRec33[1]))));
		double fTemp18 = ((iSlow107)?0:fTemp17);
		double fTemp19 = fabs(fTemp18);
		fRec32[0] = ((fSlow109 * fTemp19) + (fSlow108 * max(fTemp19, fRec32[1])));
		fRec31[0] = ((fSlow110 * fRec32[0]) + (fSlow104 * fRec31[1]));
		fRec30[0] = ((fSlow112 * max((fSlow111 + (20 * log10(fRec31[0]))), 0)) + (fSlow103 * fRec30[1]));
		fRec35[0] = (fSlow113 + (0.999 * fRec35[1]));
		output0[i] = (FAUSTFLOAT)((fSlow114 * (fRec35[0] * ((iSlow107)?fTemp17:(fTemp18 * pow(10,(0.05 * fRec30[0])))))) + ((fSlow101 * (fRec29[0] * ((iSlow94)?fTemp14:(fTemp15 * pow(10,(0.05 * fRec22[0])))))) + ((fSlow76 * (fRec21[0] * ((iSlow69)?fTemp10:(fTemp11 * pow(10,(0.05 * fRec14[0])))))) + ((fSlow50 * (fRec13[0] * ((iSlow43)?fTemp6:(fTemp7 * pow(10,(0.05 * fRec8[0])))))) + (fSlow35 * (fRec7[0] * ((iSlow28)?fTemp3:(fTemp4 * pow(10,(0.05 * fRec0[0]))))))))));
		// post processing
		fRec35[1] = fRec35[0];
		fRec30[1] = fRec30[0];
		fRec31[1] = fRec31[0];
		fRec32[1] = fRec32[0];
		fRec33[2] = fRec33[1]; fRec33[1] = fRec33[0];
		fRec34[1] = fRec34[0];
		fRec29[1] = fRec29[0];
		fRec22[1] = fRec22[0];
		fRec23[1] = fRec23[0];
		fRec24[1] = fRec24[0];
		fRec25[2] = fRec25[1]; fRec25[1] = fRec25[0];
		fRec26[1] = fRec26[0];
		fVec3[1] = fVec3[0];
		fRec27[2] = fRec27[1]; fRec27[1] = fRec27[0];
		fRec28[1] = fRec28[0];
		fRec21[1] = fRec21[0];
		fRec14[1] = fRec14[0];
		fRec15[1] = fRec15[0];
		fRec16[1] = fRec16[0];
		fRec17[2] = fRec17[1]; fRec17[1] = fRec17[0];
		fRec18[1] = fRec18[0];
		fVec2[1] = fVec2[0];
		fRec19[2] = fRec19[1]; fRec19[1] = fRec19[0];
		fRec20[1] = fRec20[0];
		fRec13[1] = fRec13[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fRec11[2] = fRec11[1]; fRec11[1] = fRec11[0];
		fRec12[1] = fRec12[0];
		fRec7[1] = fRec7[0];
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fVec1[1] = fVec1[0];
		fRec5[2] = fRec5[1]; fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
		fVec0[1] = fVec0[0];
	}
}

void __rt_func Dsp::compute_static(int count, float *input0, float *output0, PluginDef *p)
{
	static_cast<Dsp*>(p)->compute(count, input0, output0);
}

int Dsp::register_par(const ParamReg& reg)
{
	reg.registerVar("mbc.Attack1 (sec)","","S",N_("Time before the compressor starts to kick in"),&fslider28, 0.012, 0.0, 1.0, 0.001);
	static const value_pair fslider29_values[] = {{"Compress"},{"Bypass"},{"Mute"},{0}};
	reg.registerEnumVar("mbc.Mode1","","S",N_("Compress or Mute the selected band, or Bypass The Compresssor"),fslider29_values,&fslider29, 1.0, 1.0, 3.0, 1.0);
	reg.registerVar("mbc.Ratio2","","S",N_("Compresssion ratio"),&fslider26, 2.0, 1.0, 1e+02, 0.1);
	reg.registerVar("mbc.Makeup-Threshold2 (db)","","S",N_("Threshold correction, an anticlip measure"),&fslider27, 2.0, 0.0, 1e+01, 0.1);
	reg.registerVar("mbc.Release2 (sec)","","S",N_("Time before the compressor releases the sound"),&fslider24, 1.25, 0.0, 1e+01, 0.01);
	reg.registerVar("mbc.Makeup2 (db)","","S",N_("Post amplification and threshold"),&fslider25, 1e+01, -5e+01, 5e+01, 0.1);
	reg.registerVar("mbc.show_always.Crossover B1-B2 (hz)","","S",N_("Crossover bandpass frequency"),&fslider22, 8e+01, 2e+01, 2e+04, 1.0);
	static const value_pair fslider23_values[] = {{"Compress"},{"Bypass"},{"Mute"},{0}};
	reg.registerEnumVar("mbc.Mode2","","S",N_("Compress or Mute the selected band, or Bypass The Compresssor"),fslider23_values,&fslider23, 1.0, 1.0, 3.0, 1.0);
	reg.registerVar("mbc.Makeup-Threshold3 (db)","","S",N_("Threshold correction, an anticlip measure"),&fslider20, 2.0, 0.0, 1e+01, 0.1);
	reg.registerVar("mbc.Attack2 (sec)","","S",N_("Time before the compressor starts to kick in"),&fslider21, 0.012, 0.0, 1.0, 0.001);
	reg.registerVar("mbc.Makeup-Threshold5 (db)","","S",N_("Threshold correction, an anticlip measure"),&fslider13, 2.0, 0.0, 1e+01, 0.1);
	reg.registerVar("mbc.Ratio5","","S",N_("Compresssion ratio"),&fslider12, 2.0, 1.0, 1e+02, 0.1);
	reg.registerVar("mbc.Makeup5 (db)","","S",N_("Post amplification and threshold"),&fslider11, 11.0, -5e+01, 5e+01, 0.1);
	reg.registerVar("mbc.Release5 (sec)","","S",N_("Time before the compressor releases the sound"),&fslider10, 1.25, 0.0, 1e+01, 0.01);
	reg.registerVar("mbc.Release3 (sec)","","S",N_("Time before the compressor releases the sound"),&fslider17, 1.25, 0.0, 1e+01, 0.01);
	static const value_pair fslider16_values[] = {{"Compress"},{"Bypass"},{"Mute"},{0}};
	reg.registerEnumVar("mbc.Mode3","","S",N_("Compress or Mute the selected band, or Bypass The Compresssor"),fslider16_values,&fslider16, 1.0, 1.0, 3.0, 1.0);
	reg.registerVar("mbc.show_always.Crossover B2-B3 (hz)","","S",N_("Crossover bandpass frequency"),&fslider15, 2.1e+02, 2e+01, 2e+04, 1.0);
	reg.registerVar("mbc.Attack3 (sec)","","S",N_("Time before the compressor starts to kick in"),&fslider14, 0.012, 0.0, 1.0, 0.001);
	reg.registerVar("mbc.Ratio3","","S",N_("Compresssion ratio"),&fslider19, 2.0, 1.0, 1e+02, 0.1);
	reg.registerVar("mbc.Makeup3 (db)","","S",N_("Post amplification and threshold"),&fslider18, 4.0, -5e+01, 5e+01, 0.1);
	reg.registerVar("mbc.Makeup1 (db)","","S",N_("Post amplification and threshold"),&fslider31, 13.0, -5e+01, 5e+01, 0.1);
	reg.registerVar("mbc.Release1 (sec)","","S",N_("Time before the compressor releases the sound"),&fslider30, 1.25, 0.0, 1e+01, 0.01);
	reg.registerVar("mbc.Makeup-Threshold1 (db)","","S",N_("Threshold correction, an anticlip measure"),&fslider33, 2.0, 0.0, 1e+01, 0.1);
	reg.registerVar("mbc.Ratio1","","S",N_("Compresssion ratio"),&fslider32, 2.0, 1.0, 1e+02, 0.1);
	reg.registerVar("mbc.Makeup-Threshold4 (db)","","S",N_("Threshold correction, an anticlip measure"),&fslider7, 2.0, 0.0, 1e+01, 0.1);
	reg.registerVar("mbc.Ratio4","","S",N_("Compresssion ratio"),&fslider6, 2.0, 1.0, 1e+02, 0.1);
	reg.registerVar("mbc.Makeup4 (db)","","S",N_("Post amplification and threshold"),&fslider5, 8.0, -5e+01, 5e+01, 0.1);
	reg.registerVar("mbc.Release4 (sec)","","S",N_("Time before the compressor releases the sound"),&fslider4, 1.25, 0.0, 1e+01, 0.01);
	static const value_pair fslider3_values[] = {{"Compress"},{"Bypass"},{"Mute"},{0}};
	reg.registerEnumVar("mbc.Mode4","","S",N_("Compress or Mute the selected band, or Bypass The Compresssor"),fslider3_values,&fslider3, 1.0, 1.0, 3.0, 1.0);
	reg.registerVar("mbc.show_always.Crossover B4-B5 (hz)","","S",N_("Crossover bandpass frequency"),&fslider2, 5e+03, 2e+01, 2e+04, 1.0);
	reg.registerVar("mbc.show_always.Crossover B3-B4 (hz)","","S",N_("Crossover bandpass frequency"),&fslider1, 1.7e+03, 2e+01, 2e+04, 1.0);
	reg.registerVar("mbc.Attack4 (sec)","","S",N_("Time before the compressor starts to kick in"),&fslider0, 0.012, 0.0, 1.0, 0.001);
	static const value_pair fslider9_values[] = {{"Compress"},{"Bypass"},{"Mute"},{0}};
	reg.registerEnumVar("mbc.Mode5","","S",N_("Compress or Mute the selected band, or Bypass The Compresssor"),fslider9_values,&fslider9, 1.0, 1.0, 3.0, 1.0);
	reg.registerVar("mbc.Attack5 (sec)","","S",N_("Time before the compressor starts to kick in"),&fslider8, 0.012, 0.0, 1.0, 0.001);
	return 0;
}

int Dsp::register_params_static(const ParamReg& reg)
{
	return static_cast<Dsp*>(reg.plugin)->register_par(reg);
}

inline int Dsp::load_ui_f(const UiBuilder& b)
{
#define PARAM(p) ("mbc" "." p)


b.openHorizontalhideBox("");
b.closeBox();

b.openHorizontalBox("");
b.openVerticalBox("");

b.openTabBox("");

b.openHorizontalBox(N_("Band 1"));
b.openVerticalBox("");
b.openHorizontalBox("");
b.create_small_rackknob(PARAM("Makeup1 (db)"), N_("Makeup (db)"));
b.create_small_rackknob(PARAM("Makeup-Threshold1 (db)"), N_("Anticlip (db)"));
b.create_small_rackknob(PARAM("show_always.Crossover B1-B2 (hz)"), N_("Low Shelf (Hz)"));
b.closeBox();
b.openHorizontalBox("");
b.create_selector_no_caption(PARAM("Mode1"));
b.create_small_rackknob(PARAM("Ratio1"), N_("Ratio"));
b.create_small_rackknob(PARAM("Attack1 (sec)"), N_("Attack"));
b.create_small_rackknob(PARAM("Release1 (sec)"), N_("Release"));
b.closeBox();
b.closeBox();
b.closeBox();

b.openHorizontalBox(N_("Band 2"));
b.openVerticalBox("");
b.openHorizontalBox("");
b.create_small_rackknob(PARAM("Makeup2 (db)"),  N_("Makeup (db)"));
b.create_small_rackknob(PARAM("Makeup-Threshold2 (db)"), N_("Anticlip (db)"));
b.create_small_rackknob(PARAM("show_always.Crossover B1-B2 (hz)"), N_("Low (hz)"));
b.create_small_rackknob(PARAM("show_always.Crossover B2-B3 (hz)"), N_("High (hz)"));
b.closeBox();
b.openHorizontalBox("");
b.create_selector_no_caption(PARAM("Mode2"));
b.create_small_rackknob(PARAM("Ratio2"), N_("Ratio"));
b.create_small_rackknob(PARAM("Attack2 (sec)"), N_("Attack"));
b.create_small_rackknob(PARAM("Release2 (sec)"), N_("Release"));
b.closeBox();
b.closeBox();
b.closeBox();

b.openHorizontalBox(N_("Band 3"));
b.openVerticalBox("");
b.openHorizontalBox("");
b.create_small_rackknob(PARAM("Makeup3 (db)"), N_("Makeup (db)"));
b.create_small_rackknob(PARAM("Makeup-Threshold3 (db)"), N_("Anticlip (db)"));
b.create_small_rackknob(PARAM("show_always.Crossover B2-B3 (hz)"), N_("Low (hz)"));
b.create_small_rackknob(PARAM("show_always.Crossover B3-B4 (hz)"), N_("High (hz)"));
b.closeBox();
b.openHorizontalBox("");
b.create_selector_no_caption(PARAM("Mode3"));
b.create_small_rackknob(PARAM("Ratio3"), N_("Ratio"));
b.create_small_rackknob(PARAM("Attack3 (sec)"), N_("Attack"));
b.create_small_rackknob(PARAM("Release3 (sec)"), N_("Release"));
b.closeBox();
b.closeBox();
b.closeBox();

b.openHorizontalBox(N_("Band 4"));
b.openVerticalBox("");
b.openHorizontalBox("");
b.create_small_rackknob(PARAM("Makeup4 (db)"), N_("Makeup (db)"));
b.create_small_rackknob(PARAM("Makeup-Threshold4 (db)"), N_("Anticlip (db)"));
b.create_small_rackknob(PARAM("show_always.Crossover B3-B4 (hz)"), N_("Low (hz)"));
b.create_small_rackknob(PARAM("show_always.Crossover B4-B5 (hz)"), N_("High (hz)"));
b.closeBox();
b.openHorizontalBox("");
b.create_selector_no_caption(PARAM("Mode4"));
b.create_small_rackknob(PARAM("Ratio4"), N_("Ratio"));
b.create_small_rackknob(PARAM("Attack4 (sec)"), N_("Attack"));
b.create_small_rackknob(PARAM("Release4 (sec)"), N_("Release"));
b.closeBox();
b.closeBox();
b.closeBox();

b.openHorizontalBox(N_("Band 5"));
b.openVerticalBox("");
b.openHorizontalBox("");
b.create_small_rackknob(PARAM("Makeup5 (db)"), N_("Makeup (db)"));
b.create_small_rackknob(PARAM("Makeup-Threshold5 (db)"), N_("Anticlip (db)"));
b.create_small_rackknob(PARAM("show_always.Crossover B4-B5 (hz)"), N_("High Shelf (hz)"));
b.closeBox();
b.openHorizontalBox("");
b.create_selector_no_caption(PARAM("Mode5"));
b.create_small_rackknob(PARAM("Ratio5"), N_("Ratio"));
b.create_small_rackknob(PARAM("Attack5 (sec)"), N_("Attack"));
b.create_small_rackknob(PARAM("Release5 (sec)"), N_("Release"));
b.closeBox();
b.closeBox();
b.closeBox();

b.closeBox();
b.closeBox();
b.closeBox();

#undef PARAM
	return 0;
}

int Dsp::load_ui_f_static(const UiBuilder& b)
{
	return static_cast<Dsp*>(b.plugin)->load_ui_f(b);
}

PluginDef *plugin() {
	return new Dsp();
}

void Dsp::del_instance(PluginDef *p)
{
	delete static_cast<Dsp*>(p);
}

} // end namespace mbc
} // end namespace pluginlib
