from __future__ import division
import os, quik


class MyLoader(object):

    def load_text(self, fname):
        if fname.startswith("%"):
            # hack for directly including guitarix gx_plugin.h
            guitarix_headers = "../../../src/headers"
            with open(os.path.join(guitarix_headers, fname[1:])) as f:
                return "".join([line for line in f if not (line.startswith("#include") or line.startswith("#pragma once"))])
        else:
            with open(fname) as f:
                return f.read()

    def load_template(self, name):
        return globals()[name]


class Template(quik.Template):
    def render(self, namespace, loader=MyLoader()):
        return quik.Template.render(self, namespace, loader)

c_template_top = Template("""\
%if (@build_script)\
// generated by @build_script
%else\
// generated automatically
%end
// DO NOT MODIFY!
#include <iostream>
#include <cmath>
%% %if (@method == "hybr" || @method == "lm")
%if (@method == "lm")
#include <cminpack.h>
%else
#include <float.h>
%end
#include <Eigen/Core>
%if (@np > 0)
#include <Eigen/LU>
%end
%if (@dev_interface)%% just for convenience, to make stand alone sources
%include ("%gx_compiler.h")
%include ("%gx_plugin.h")
%else
#include "gx_compiler.h"
#include "gx_plugin.h"
%end

using namespace Eigen;

#define N_(x) (x)

#define creal @c_real

static inline int sign(creal v) {
    return v < 0 ? -1 : 1;
}

static inline int Heaviside(creal v) {
    return v < 0 ? 0 : 1;  // Heaviside(0) == 1/2 in sympy but shouldn't matter
}

static Matrix<creal, @nx, 1> g_x;
creal g_v_data[@nn];
static Map<Matrix<creal, @nn, 1> >g_v(g_v_data);
static int g_info;
static int g_nfev;

%if (@dev_interface)
static creal g_fnorm;
static Array<creal, @nni, 1> g_min;
static Array<creal, @nni, 1> g_max;

#define INTERFACE_VERSION 4

extern "C" __attribute__ ((visibility ("default")))
int get_interface_version() {
    return INTERFACE_VERSION;
}

extern "C" __attribute__ ((visibility ("default")))
void get_structure(const char **name, int *data_size, int *samplerate, const int **shapes, const int (**comp_sz)[6],
                   const char ***pins, const char ***comp_names, const char **method, const char ***pot_vars,
                   const double **pot, const char ***out_labels, const char **comment) {
    static const char *n = "@name";
    static int sz[] = { @nx, @ni, @no, @npl, @nn, @nni, @nno, @nc, -1 }; // nx, ni, no, npl, nn, nni, nno, nc, -1
    static int nn_sz[][6] = { // component v_slice, p_slice, i_slice
    %for @c in @components:\
        {@c.v_slice.start, @c.v_slice.stop, @c.p_slice.start, @c.p_slice.stop, @c.i_slice.start, @c.i_slice.stop},
    %end
    };
    static const char *nn_name[] = { // component names
    %for @c in @components:\
        "@c.name", "@c.namespace",
    %end 0
    };
    static const char *nn_pins[] = { // component pins
    %for @c in @nlin_elements:\
        "@c",
    %end 0
    };
    static const char *m = "@method";
    static const char *pvars[] = {@pot_vars};
    static double pvalues[] = {@pot};
    static const char *ol[] = {@out_labels};
    static const char *c = "@comment";
    if (name) *name = n;
    if (data_size) *data_size = sizeof(creal);
    if (samplerate) *samplerate = @fs;
    if (shapes) *shapes = sz;
    if (comp_sz) *comp_sz = nn_sz;
    if (pins) *pins = nn_pins;
    if (comp_names) *comp_names = nn_name;
    if (method) *method = m;
    if (pot_vars) *pot_vars = pvars;
    if (pot) *pot = pvalues;
    if (out_labels) *out_labels = ol;
    if (comment) *comment = c;
}
%end

static creal x0_data[] = {@x0_data};
%if (@dev_interface)
static creal v0_data[] = {@v0_data};
static creal p0_data[] = {@p0_data};
static creal o0_data[] = {@o0_data};
static creal op_data[] = {@op_data};

extern "C" __attribute__ ((visibility ("default")))
void get_dc(creal *v0, creal *x0, creal *p0, creal *o0, creal *op) {
    if (v0) {
        for (int i = 0; i < @nn; i++) {
            v0[i] = v0_data[i];
        }
    }
    if (x0) {
        for (int i = 0; i < @nx; i++) {
            x0[i] = x0_data[i];
        }
    }
    if (p0) {
        for (int i = 0; i < @nni; i++) {
            p0[i] = p0_data[i];
        }
    }
    if (o0) {
        for (int i = 0; i < @no; i++) {
            o0[i] = o0_data[i];
        }
    }
    if (op) {
        for (int i = 0; i < @ni; i++) {
            op[i] = op_data[i];
        }
    }
}

extern "C" __attribute__ ((visibility ("default")))
void get_info(creal *v, creal *x, creal *minval, creal *maxval, int *info, int *nfev, creal *fnorm) {
    Map<Matrix<creal, @nn, 1> > V(v);
    V = g_v;
    Map<Matrix<creal, @nx, 1> > X(x);
    X = g_x;
    Map<Matrix<creal, @nni, 1> > Mi(minval);
    Mi = g_min;
    Map<Matrix<creal, @nni, 1> > Ma(maxval);
    Ma = g_max;
    *info = g_info;
    *nfev = g_nfev;
    *fnorm = g_fnorm;
}

extern "C" __attribute__ ((visibility ("default")))
void set_state(creal *v, creal *x) {
    Map<Matrix<creal, @nn, 1> > V(v);
    g_v = V;
    Map<Matrix<creal, @nx, 1> > X(x);
    g_x = X;
}
%end

#define real realtype  // real conflicts with Eigen::real of new eigen library version
typedef double real;
%if (@method == "table")
%include ("../intpp.h")
#define NO_INTPP_INCLUDES
%include ("../intpp.cc")
@extra_sources.intpp_inst
%%@extra_sources.data_h
@extra_sources.data_c
%end

@global_matrices

static Matrix<creal, @npl, 1> last_pot;

@struct_def

%if (@dev_interface || @npl)
void calc_inv_update(const creal *pot, nonlin_param& par) {
    Map<const Matrix<creal, @npl, 1> >pm(pot);
    last_pot = pm;
    @update_pot
}
%end
%if (@dev_interface)
nonlin_param par@nonlin_mat_list;

extern "C" __attribute__ ((visibility ("default")))
void calc_inv_update(const creal *pot) {
    calc_inv_update(pot, par);
}
%end

@nonlin_code

%if (@dev_interface)
extern "C" __attribute__ ((visibility ("default")))
int calc_stream(creal *u, creal *o, int n) {
    Matrix<creal, @nn, 1> mi;
%if (@nn)
    g_min = Matrix<creal, @nni, 1>::Constant(HUGE_VAL);
    g_max = Matrix<creal, @nni, 1>::Constant(-HUGE_VAL);
    g_fnorm = 0;
    par.v = &g_v;
    par.i = &mi;
    Array<creal, @nni, 1> p_val;
    par.p_val = &p_val;
    creal fnorm;
    par.fnorm = &fnorm;
    Matrix<creal, @nn, 1> mp;
    par.p = &mp;
%end
    for (int j = 0; j < n; j++) {
#define GET_U (u+j*@ni)
#define DTP_U creal
        @pre_filter
%if (@nn)
        Matrix<creal, @mp_cols, 1> dp;
        dp << g_x, Map<Matrix<creal,@ni,1> >(GET_U);
        @gen_mp
        int ret = nonlin::nonlin_solve(par);
        if (fnorm > g_fnorm) {
            g_fnorm = fnorm;
        }
        if (ret != 0) {
            return ret;
        }
        g_min = g_min.min(p_val.array());
        g_max = g_max.max(p_val.array());
%end
        Matrix<creal, @m_cols, 1> d;
%if (@nn)
        d << g_x, Map<Matrix<creal,@ni,1> >(GET_U), mi@iblock;
%else
        d << g_x, Map<Matrix<creal,@ni,1> >(GET_U);
%end
%if (@nx)
        Matrix<creal, @nx, 1>& xn = g_x;
        @gen_xn
%end
        Map<Matrix<creal, @no, 1> > xo(o+@no*j);
        @gen_xo
#undef GET_U
#undef DTP_U
    }
    return 0;
}

extern "C" __attribute__ ((visibility ("default")))
int calc(creal *u, creal *x, creal *v, creal *x_new, creal *o, int *info, int *nfev, creal *fnorm) {
#define GET_U (u)
#define DTP_U creal
    @pre_filter
    int ret = 0;
    Matrix<creal, @nn, 1> mi;
%if (@nn)
    par.fnorm = fnorm;
    par.i = &mi;
    Matrix<creal, @mp_cols, 1> dp;
    dp << Map<Matrix<creal,@nx,1> >(x), Map<Matrix<creal,@ni,1> >(u);
    Matrix<creal, @nn, 1> mp;
    par.p = &mp;
    Map<Matrix<creal, @nn, 1> >Mv(v);
    par.v = &Mv;
    Array<creal, @nni, 1> p_val;
    par.p_val = &p_val;
    @gen_mp
    ret = nonlin::nonlin(par);
%else
    *info = 1;
    *nfev = 0;
    *fnorm = 0;
%end
    Matrix<creal, @m_cols, 1> d;
    d << Map<Matrix<creal,@nx,1> >(x), Map<Matrix<creal,@ni,1> >(u), mi@iblock;
    Map<Matrix<creal, @nx, 1> > xn(x_new);
    @gen_xn
    Map<Matrix<creal, @no, 1> > xo(o);
    @gen_xo
    return ret;
#undef GET_U
#undef DTP_U
}
%end

%if (@resample)
#include <zita-resampler/resampler.h>

class FixedRateResampler {
private:
    Resampler r_up, r_down;
    int inputRate, outputRate;
    int last_in_count;
public:
    int setup(int _inputRate, int _outputRate);
    int up(int count, float *input, float *output);
    void down(float *input, float *output);
    int max_out_count(int in_count) {
	return static_cast<int>(ceil((in_count*static_cast<double>(outputRate))/inputRate)); }
};

int FixedRateResampler::setup(int _inputRate, int _outputRate)
{
    const int qual = 16; // resulting in a total delay of 2*qual (0.7ms @44100)
    inputRate = _inputRate;
    outputRate = _outputRate;
    if (inputRate == outputRate) {
	return 0;
    }
    // upsampler
    int ret = r_up.setup(inputRate, outputRate, 1, qual);
    if (ret) {
	return ret;
    }
    // k == filtlen() == 2 * qual
    // pre-fill with k-1 zeros
    r_up.inp_count = r_up.filtlen() - 1;
    r_up.out_count = 1;
    r_up.inp_data = r_up.out_data = 0;
    r_up.process();
    // downsampler
    ret = r_down.setup(outputRate, inputRate, 1, qual);
    if (ret) {
	return ret;
    }
    // k == filtlen() == 2 * qual * fact
    // pre-fill with k-2 zeros
    r_down.inp_count = r_down.filtlen() - 2;
    r_down.out_count = 1;
    r_down.inp_data = r_down.out_data = 0;
    r_down.process();
    return 0;
}

int FixedRateResampler::up(int count, float *input, float *output)
{
    if (inputRate == outputRate) {
	memcpy(output, input, count*sizeof(float));
	r_down.out_count = count;
	return count;
    }
    r_up.inp_count = count;
    r_down.out_count = count+1; // +1 == trick to drain input
    r_up.inp_data = input;
    int m = max_out_count(count);
    r_up.out_count = m;
    r_up.out_data = output;
    r_up.process();
    assert(r_up.inp_count == 0);
    assert(r_up.out_count <= 1);
    r_down.inp_count = m - r_up.out_count;
    return r_down.inp_count;
}

void FixedRateResampler::down(float *input, float *output)
{
    if (inputRate == outputRate) {
	memcpy(output, input, r_down.out_count*sizeof(float));
	return;
    }
    r_down.inp_data = input;
    r_down.out_data = output;
    r_down.process();
    assert(r_down.inp_count == 0);
    assert(r_down.out_count == 1);
}

FixedRateResampler smp;
%end

class DKPlugin: public PluginDef {
public:
    float pots[@npl+@add_npl];
private:
    creal pots_last[@npl+@add_npl];
    Matrix<creal, @nx, 1> x_last;
    @DKPlugin_fields
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    DKPlugin();
    static void init(unsigned int samplingFreq, PluginDef *plugin);
    static void process(int count, float *input, float *output, PluginDef *plugin);
    static int registerparam(const ParamReg& reg);
    static int uiloader(const UiBuilder& builder, int form);
    static void del_instance(PluginDef *plugin);
};

DKPlugin::DKPlugin():
    PluginDef(), pots(), pots_last(), x_last()@DKPlugin_init {
    version = PLUGINDEF_VERSION;
    flags = 0;
    id = @plugindef.s_id;
    name = @plugindef.l_name;
    groups = 0;
    description = @plugindef.l_description;
    category = @plugindef.l_category;
    shortname = @plugindef.l_shortname;
    mono_audio = process;
    set_samplerate = init;
    register_params = registerparam;
    load_ui = uiloader;
    delete_instance = del_instance;
    for (int i = 0; i < @nx; i++) {
        x_last(i) = x0_data[i];
    }
}

#define PARAM(p) ("@id" "." p)

int DKPlugin::registerparam(const ParamReg& reg) {
    %if (@regs)
        DKPlugin& self = *static_cast<DKPlugin*>(reg.plugin);
    %end
    %for @r in @regs:
        reg.registerVar(PARAM("@r.id"), N_("@r.name"), "S", N_("@r.desc"), &self.pots[@r.varidx], 0.5, 0, 1, 0.01);
    %end
    return 0;
}

void DKPlugin::init(unsigned int samplingFreq, PluginDef *plugin) {
%if (@resample)
    smp.setup(samplingFreq, @fs);
%end
%if (@filter_init)
    DKPlugin& self = *static_cast<DKPlugin*>(plugin);
    @filter_init
%end
}

void DKPlugin::process(int n, float *u, float *o, PluginDef *plugin) {
    DKPlugin& self = *static_cast<DKPlugin*>(plugin);
%if (@npl || @add_npl)
    creal t[@npl+@add_npl];
    @calc_pots
%end
    @process_add
// start copied and modified code
    Matrix<creal, @nn, 1> mi;
%if (@nn)
%%    g_min = Matrix<creal, @nni, 1>::Constant(HUGE_VAL);
%%    g_max = Matrix<creal, @nni, 1>::Constant(-HUGE_VAL);
%%    g_fnorm = 0;
    creal fnorm;
    Matrix<creal, @nn, 1> mp;
    Array<creal, @nni, 1> p_val;
    nonlin_param par@nonlin_mat_list_calc;
%end
%if (@resample)
    float buf[smp.max_out_count(n)];
    n = smp.up(n, u, buf);
#define GET_U (buf+j*@ni)
%else
#define GET_U (u+j*@ni)
%end
    for (int j = 0; j < n; j++) {
#define DTP_U float
        @pre_filter
%if (@npl)
        for (int k = 0; k < @npl; k++) {
            self.pots_last[k] = @timecst * t[k] + (1-@timecst) * self.pots_last[k];
        }
        calc_inv_update(self.pots_last, par);
%end
%if (@nn)
        Matrix<creal, @mp_cols, 1> dp;
        dp << self.x_last, Map<Matrix<float,@ni,1> >(GET_U).cast<creal>();
        @gen_mp
        nonlin::nonlin(par);
%%        int ret = nonlin::nonlin_solve(par);
%%        if (fnorm > g_fnorm) {
%%            g_fnorm = fnorm;
%%        }
%%        if (ret != 0) {
%%            return;
%%        }
%end
        Matrix<creal, @m_cols, 1> d;
        d << self.x_last, Map<Matrix<float,@ni,1> >(GET_U).cast<creal>(), mi@iblock;
%if (@nx)
        Matrix<creal, @nx, 1>& xn = self.x_last;
        @gen_xn
%end
%if (@resample)
        Map<Matrix<float, @no, 1> > xo(buf+@no*j);
%else
        Map<Matrix<float, @no, 1> > xo(o+@no*j);
%end
        @gen_xo_float
        @post_filter
#undef GET_U
#undef DTP_U
    }
%if (@resample)
    smp.down(buf, o);
%end
// end copied code
}

int DKPlugin::uiloader(const UiBuilder& b, int form) {
    if (!(form & UI_FORM_STACK)) {
        return -1;
    }
    %parse ("module_ui_template")
    return 0;
}

void DKPlugin::del_instance(PluginDef *p)
{
    delete static_cast<DKPlugin*>(p);
}

%if (@dev_interface)
extern "C" __attribute__ ((visibility ("default")))
int get_gx_plugin(unsigned int idx, PluginDef **pplugin)
{
    const int count = 1;
    if (!pplugin) {
        return count;
    }
    switch (idx) {
    case 0: *pplugin = new DKPlugin; return count;
    default: *pplugin = 0; return -1;
    }
}
%else
namespace pluginlib { namespace @plugindef.namespace {
PluginDef *plugin() {
    return new DKPlugin;
}
}}
%end

%if (@{plugindef.lv2_plugin_type})
%parse ("lv2_interface")
%end
""")

lv2_manifest = Template("""
\@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .
\@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .

<http://guitarix.sourceforge.net/plugins/@id#@{plugindef.lv2_versioned_id}>
    a lv2:Plugin ;
    lv2:binary <@{plugindef.lv2_versioned_id}.so>  ;
    rdfs:seeAlso <@{plugindef.lv2_versioned_id}.ttl> .
""")

lv2_ttl = Template("""
\@prefix doap: <http://usefulinc.com/ns/doap#> .
\@prefix foaf: <http://xmlns.com/foaf/0.1/> .
\@prefix lv2: <http://lv2plug.in/ns/lv2core#> .
\@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
\@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
\@prefix guiext: <http://lv2plug.in/ns/extensions/ui#>.

<http://guitarix.sourceforge.net#devel>
	a foaf:Group ;
	foaf:name "Guitarix team" ;
	foaf:mbox <mailto:guitarix-developer\@lists.sourceforge.net> ;
	rdfs:seeAlso <http://guitarix.sourceforge.net> .

<http://guitarix.sourceforge.net/plugins/@id>
	a doap:Project ;
	doap:maintainer <http://guitarix.sourceforge.net#devel> ;
	doap:name "@{plugindef.name}" .

<http://guitarix.sourceforge.net/plugins/@id#@{plugindef.lv2_versioned_id}>
    a lv2:Plugin ,
        lv2:@{plugindef.lv2_plugin_type} ;
    doap:maintainer <http://guitarix.sourceforge.net#devel> ;
    doap:name "@{plugindef.name}";
    doap:license <http://usefulinc.com/doap/licenses/gpl> ;
    lv2:project <http://guitarix.sourceforge.net/plugins/@id> ;
    lv2:optionalFeature lv2:hardRTCapable ;
    lv2:minorVersion @{plugindef.lv2_minor_version};
    lv2:microVersion @{plugindef.lv2_micro_version};

    lv2:port\
%for @p in @lv2_ports:\
 [
        a @{p.type_list} ;
        lv2:index @{p.index} ;
        lv2:symbol "@{p.symbol}" ;
        lv2:name "@{p.name}" ;
%if (@{p.control_index} >= 0)\
        lv2:default @{p.default} ;
        lv2:minimum @{p.minimum} ;
        lv2:maximum @{p.maximum} ;
%end
    ]%if(@velocityHasNext),%end\
%end.
""")

lv2_interface = Template("""
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define LV2_PLUGIN_URI "http://guitarix.sourceforge.net/plugins/@id#@{plugindef.lv2_versioned_id}"

typedef enum {
%for @p in @lv2_ports:\
	PORT_@{p.symbol} = @{p.index},
%end
} PortIndex;

class LV2_DKPlugin: public DKPlugin {
public:
	// Port buffers
	float* ports[@{lv2_ports.port_count()}];
public:
    LV2_DKPlugin(): DKPlugin() {}
};

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    LV2_DKPlugin *p = new LV2_DKPlugin;
    p->set_samplerate(rate, p);
    return static_cast<LV2_Handle>(p);
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	static_cast<LV2_DKPlugin*>(instance)->ports[port] = static_cast<float*>(data);
}

static void
activate(LV2_Handle instance)
{
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
    LV2_DKPlugin* p = static_cast<LV2_DKPlugin*>(instance);

%for @p in @lv2_ports:\
%if (@{p.control_index} >= 0)\
	p->pots[@{p.control_index}] = *(p->ports[@{p.index}]);
%end
%end
%if (@ni == 1 && @no == 1)
	p->process(n_samples, p->ports[0], p->ports[1], p);
%else
	p->process(n_samples, p->ports[0], p->ports[1], p->ports[2], p->ports[3], p);
%end
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
    LV2_DKPlugin* p = static_cast<LV2_DKPlugin*>(instance);
    p->delete_instance(p);
}

static const void*
extension_data(const char* uri)
{
	return NULL;
}

static const LV2_Descriptor descriptor = {
	LV2_PLUGIN_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
extern "C" __attribute__ ((visibility ("default")))
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}
""")

c_template_calc_nonlin = Template("""
%if (@dev_interface)
extern "C" __attribute__ ((visibility ("default")))
int calc_@{namespace}(int n, creal *p, creal *i, creal *v, int *info, int *nfev, creal *fnorm) {
    int ret = 0;
    Matrix<creal, @g_nn, 1> mp;
    Matrix<creal, @g_nn, 1> mi;
    Map<Matrix<creal, @g_nn, 1> >Mv(v);
    Array<creal, @g_nni, 1> p_val;
    nonlin_param par@nonlin_mat_list;
    for (int k = 0; k < n; k++) {
        calc_inv_update(p+k*(@nni+@npl), par);
        mp@pblockV << Map<Matrix<creal, @nni, 1> >(p+k*(@nni+@npl)+@npl);
        ret = @namespace::nonlin_solve(par);
        Map<Matrix<creal, @nno, 1> >(i+k*@nno) << mi@iblockV;
    }
    return ret;
}
%end
""")

c_template_nonlin_func_hybr = Template("""
static int fcn(void *p, int n, const creal *v, creal *fvec, int iflag ) {
    nonlin_param& par = *static_cast<nonlin_param *>(p);
    @expression
    return 0;
}
""")

c_template_nonlin_func_lm = Template("""
static int fcn(void *p, int m, int n, const creal *v, creal *fvec, int iflag ) {
    nonlin_param& par = *static_cast<nonlin_param *>(p);
    @expression
    return 0;
}
""")

c_template_nonlin_func_hybrCC = Template("""
typedef int root_fcn(void*p, const creal *v, creal *fvec, int iflag);
static int fcn(void *p, const creal *v, creal *fvec, int iflag) {
    nonlin_param& par = *static_cast<nonlin_param *>(p);
    @expression
    return 0;
}
""")

c_template_nonlin_solver_hybr = Template("""
static int nonlin(struct nonlin_param &par) {
    int maxfev, mode, nprint, ldfjac;
    creal xtol, epsfcn, factor;
    creal __attribute__((aligned(16))) fvec[@nn];
    creal __attribute__((aligned(16))) fjac[@nn*@nn];
    creal __attribute__((aligned(16))) qtf[@nn];
    creal __attribute__((aligned(16))) wa1[@nn];
    creal __attribute__((aligned(16))) wa2[@nn];
    creal __attribute__((aligned(16))) wa3[@nn];
    creal __attribute__((aligned(16))) wa4[@nn];
    creal diag[@nn] = {%for @j in @solver_diag:@j,%end};
    int ml, mu, lr;
    creal r[(@nn*(@nn+1))/2];
    lr = (@nn*(@nn+1))/2;
    ml = @nn-1; /* unbanded jacobian */
    mu = @nn-1; /* unbanded jacobian */
    ldfjac = @nn;

    /* parameter */
    xtol = @solver_xtol;
    maxfev = @solver_maxfev;
    epsfcn = 0.;
    //mode = 2;  /* explicit variable scaling with diag */
    mode = 1;  /* automatic variable scaling */
    factor = @solver_factor;
    nprint = 0;
    /**/

    @setup
    @store_p
    @p_transform
    *par.info = __cminpack_func__(hybrd)(
        fcn, &par, @nn, @var_v_ref, fvec, xtol, maxfev, ml, mu, epsfcn, diag,
        mode, factor, nprint, par.nfev, fjac, ldfjac, r, lr, qtf, wa1, wa2, wa3, wa4);
    *par.fnorm = __cminpack_func__(enorm)(@nn, fvec);
    @i_transform
    int ret = 0;
    if (*par.info != 1) {
        if (!(*par.info == 5 && *par.fnorm < 1e-20)) {
            ret = -1;
        }
    }
    @cleanup
    return ret;
}
""")

c_template_nonlin_solver_lm = Template("""
static int nonlin(struct nonlin_param &par) {
    int maxfev, mode, nprint, ldfjac;
    creal xtol, epsfcn, factor;
    creal __attribute__((aligned(16))) fvec[@nn];
    creal __attribute__((aligned(16))) fjac[@nn*@nn];
    creal __attribute__((aligned(16))) qtf[@nn];
    creal __attribute__((aligned(16))) wa1[@nn];
    creal __attribute__((aligned(16))) wa2[@nn];
    creal __attribute__((aligned(16))) wa3[@nn];
    creal __attribute__((aligned(16))) wa4[@nn];
    creal diag[@nn] = {%for @j in @solver_diag:@j,%end};
    int __attribute__((aligned(16))) ipvt[@nn];
    creal ftol, gtol;
    ftol = sqrt(__cminpack_func__(dpmpar)(1)); // parameter
    gtol = 0.; // parameter
    ldfjac = @nn;

    /* parameter */
    xtol = @solver_xtol;
    maxfev = @solver_maxfev;
    epsfcn = 0.;
    //mode = 2;  /* explicit variable scaling with diag */
    mode = 1;  /* automatic variable scaling */
    factor = @solver_factor;
    nprint = 0;
    /**/

    @setup
    @store_p
    @p_transform

    *par.info = __cminpack_func__(lmdif)(
        fcn, &par, @nn, @nn, @var_v_ref, fvec, ftol, xtol, gtol, maxfev, epsfcn, diag,
        mode, factor, nprint, par.nfev, fjac, ldfjac, ipvt, qtf, wa1, wa2, wa3, wa4);
    *par.fnorm = __cminpack_func__(enorm)(@nn, fvec);
    @i_transform
    @cleanup
    return (*par.info < 1 || *par.info > 4) ? -1 : 0;
}
""")

c_template_nonlin_solver_hybrCC = Template("""
%include ("hybrd.cc")

static int nonlin(struct nonlin_param &par) {
    int maxfev, mode, nprint, ldfjac;
    creal xtol, epsfcn, factor;
    creal __attribute__((aligned(16))) fvec[@nn];
    creal __attribute__((aligned(16))) fjac[@nn*@nn];
    creal __attribute__((aligned(16))) qtf[@nn];
    creal __attribute__((aligned(16))) wa1[@nn];
    creal __attribute__((aligned(16))) wa2[@nn];
    creal __attribute__((aligned(16))) wa3[@nn];
    creal __attribute__((aligned(16))) wa4[@nn];
    creal diag[@nn] = {%for @j in @solver_diag:@j,%end};
    int ml, mu, lr;
    creal r[(@nn*(@nn+1))/2];
    lr = (@nn*(@nn+1))/2;
    ml = @nn-1; /* unbanded jacobian */
    mu = @nn-1; /* unbanded jacobian */
    ldfjac = @nn;

    /* parameter */
    xtol = @solver_xtol;
    maxfev = @solver_maxfev;
    epsfcn = 0.;
    //mode = 2;  /* explicit variable scaling with diag */
    mode = 1;  /* automatic variable scaling */
    factor = @solver_factor;
    nprint = 0;
    /**/

    @setup
    @store_p
    @p_transform

    *par.info = hybrdX<@nn>(fcn, &par, @var_v_ref, fvec, xtol, maxfev, ml, mu, epsfcn,
                          diag, mode, factor, nprint, par.nfev, fjac, ldfjac, r, lr,
                          qtf, wa1, wa2, wa3, wa4);
    *par.fnorm = enorm<@nn>(fvec);
    @i_transform
    int ret = 0;
    if (*par.info != 1) {
        if (!(*par.info == 5 && *par.fnorm < 1e-20)) {
            ret = -1;
        }
    }
    @cleanup
    return ret;
}
""")

c_template_nonlin_chained = Template("""
namespace @namespace {
@global_data_def

static int nonlin(struct nonlin_param &par) {
    @chained_code
}

%if (@dev_interface)
static inline int nonlin_solve(nonlin_param& par) {
    return nonlin(par);
}
%end
} // end namespace @namespace
""")

c_template_nonlin_solver = Template("""
namespace @namespace {
@global_data_def

@fcn_def

@nonlin_def

%if (@dev_interface)
static Matrix<creal, @nni, 1> last_good;
static Matrix<creal, @nn, 1> last_v0;

int nonlin_homotopy(int *n, Matrix<creal, @nni, 1>& start, nonlin_param& par) {
    Matrix<creal, @nni, 1> end = @par_p;
    for (int j = 1; j <= *n; j++) {
        @par_p = start + (j * (end - start)) / *n;
        int ret = nonlin(par);
        if (ret != 0) {
            @par_p = end;
            *n = j;
            return ret;
        }
        last_good = @par_p;
        last_v0 = @par_v;
    }
    @par_p = end;
    return 0;
}

static inline int nonlin_solve(nonlin_param& par) {
    int ret = nonlin(par);
    if (ret != 0) {
        int n = 2;
        for (int j = 0; j < @solver_max_homotopy_iter; j++) {
            @par_v = last_v0;
            int n2 = n;
            ret = nonlin_homotopy(&n, last_good, par);
            if (ret == 0) {
                break;
            }
            if (n == 1) {
                n = n2 * 2;
            } else {
                n = 2;
            }
        }
        if (ret != 0) {
            return ret;
        }
    }
    last_good = @par_p;
    last_v0 = @par_v;
    return 0;
}
%end
} // end namespace @namespace

%if (@extern_nonlin)
%parse ("c_template_calc_nonlin")
%end
""")

c_template_table = Template("""
namespace @namespace {
static inline int nonlin(nonlin_param& par) {
    real t[AmpData::@namespace::sd.m];
    real m[@nni+@npl];
    Map<Matrix<real, @nni+@npl, 1> >mp(m);
    mp << last_pot.cast<real>(), (*par.p)@{pblockV}.cast<real>();
    for (int j = 0; j < AmpData::@namespace::sd.m; j++) {
        splinecoeffs<AmpData::@namespace::maptype> *pc = &AmpData::@namespace::sd.sc[j];
        check(&AmpData::@namespace::sd, m, (*pc->eval)(pc, m, &t[j]));
    }
    (*par.i)@{iblockV} = Map<Matrix<real, @nno, 1> >(t).cast<creal>();
    return 0;
}

%if (@dev_interface)
static inline int nonlin_solve(nonlin_param& par) {
    return nonlin(par);
}
%end

%parse ("c_template_calc_nonlin")
} // end @namespace
""")

#
#-mavx -ffast-math
#
# ffast-math seems to trigger a compiler error on gcc 4.7.3
# according to documentation it expands into list of settings, including
# funsafe-math-optimizations, which seems to be responsible for the bug.
# But funsafe-math-optimizations in turn just expands into another list of
# settings. When using the expanded list of settings the bug is gone (?!).
#
# maybe -mavx is the culprit (removed for now)??
#
build_script_template = Template("""\
#! /bin/bash
cd `dirname $0`
fname="${1-@sourcename}"
lname="${2-@soname}"
lname_="${2-@soname_}"
mo="-fwrapv -ffast-math -msse2"
#mo="-fwrapv -fno-math-errno -ffinite-math-only -fno-rounding-math"
#mo="$mo -fno-signaling-nans -fcx-limited-range -fno-signed-zeros"
#mo="$mo -fno-trapping-math -fassociative-math -freciprocal-math"
: ${CC:=c++}
%if (@debug)
dbg="-g -O2"
%else
dbg="-O2 -Wl,--strip-all -DNDEBUG"
%end
co="-shared -fPIC -Wall -fvisibility=hidden"
lo="-Wl,-O1,-Bsymbolic-functions,-z,relro"
libs="@libraries"
inc="@includes"
def="@defines"
opt="-fno-stack-protector"
set -e
%if (@optimize)
$CC $co $lo $mo $opt -fprofile-arcs $dbg $CFLAGS $def $inc "$fname" -o "$lname_" $libs
python t.py
$CC $co $lo $mo $opt -fprofile-use $dbg $CFLAGS $def $inc "$fname" -o "$lname" $libs
%else
$CC $co $lo $mo $opt $dbg $CFLAGS $def $inc "$fname" -o "$lname" $libs
%end
""")

faust_filter_template = Template("""\
%if (@build_script)\
// generated by @build_script
%else\
// generated automatically
%end
// DO NOT MODIFY!
declare id "@plugindef.id";
declare name "@plugindef.name";
declare category "@plugindef.category";
%if (@plugindef.shortname)\
declare shortname "@plugindef.shortname";
%end
%if (@plugindef.description)\
declare description "@plugindef.description";
%end

import("filter.lib");

process = pre : iir((@b_list),(@a_list)) with {
    LogPot(a, x) = if(a, (exp(a * x) - 1) / (exp(a) - 1), x);
    Inverted(b, x) = if(b, 1 - x, x);
    s = 0.993;
    fs = float(SR);
    pre = @pre_filter;

    %for @sl in @sliders:
        @sl.id = vslider("@sl.id[name:@sl.name]", 0.5, 0, 1, 0.01) : Inverted(@sl.inv) : LogPot(@sl.loga) : smooth(s);
    %end

    @coeffs
};
""")

module_ui_template = Template("""
b.openHorizontalhideBox("");
%if (@have_master_slider)
    b.create_master_slider(PARAM("@master_slider_id"), 0);
%end
b.closeBox();
b.openHorizontalBox("");
%for @k in @knob_ids:
    b.create_small_rackknobr(PARAM("@k"), 0);
%end
b.closeBox();
""")