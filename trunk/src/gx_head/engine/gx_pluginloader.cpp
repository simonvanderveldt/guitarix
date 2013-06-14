/*
 * Copyright (C) 2011 Hermann Meyer, James Warden, Andreas Degert, Pete Shorthose
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <dlfcn.h>
#include <dirent.h>

#include "engine.h"

namespace gx_engine {

/****************************************************************
 ** class ParamRegImpl
 */

gx_engine::ParamMap *ParamRegImpl::pmap = 0;

ParamRegImpl::ParamRegImpl(gx_engine::ParamMap* pm): ParamReg() {
    pmap = pm;
    plugin = 0;
    registerVar = registerVar_;
    registerBoolVar = registerBoolVar_;
    registerNonMidiVar = registerNonMidiVar_;
    registerEnumVar = registerEnumVar_;
    registerIEnumVar = registerIEnumVar_;
}

float *ParamRegImpl::registerVar_(const char* id, const char* name, const char* tp,
			      const char* tooltip, float* var, float val,
			      float low, float up, float step) {
    assert(step > 0);
    assert(up > low);
    if (!name[0]) {
	assert(strrchr(id, '.'));
	name = strrchr(id, '.')+1;
    }
    int n = strlen(tp);
    if (n && tp[n-1] == 'A') {
	if (pmap->hasId(id)) {
	    gx_engine::Parameter& p = (*pmap)[id];
#ifndef NDEBUG
	    gx_engine::FloatParameter p2(
		id, name, (tp[0] == 'B' ? Parameter::Switch : gx_engine::Parameter::Continuous),
		true, &p.getFloat().get_value(), val, low, up, step, true, false);
	    p2.set_desc(tooltip);
	    gx_engine::compare_parameter("Alias Parameter", &p, &p2);
#endif
	    return &p.getFloat().get_value();
	}
    }
    gx_engine::Parameter *p = 0;
    int i = 0;
    if (tp[0] == 'S') {
	i = 1;
	p = pmap->reg_par(id, name, var, val, low, up, step);
	if (tp[1] == 'L') {
	    assert(step > 1);
	    p->set_log_display();
	    i = 2;
	}
    } else if (tp[0] == 'B') {
	i = 1;
	p = pmap->reg_par(id, name, var, val);
    } else {
	assert(false);
    }
    if (tp[i] == 'O') {
	p->setSavable(false);
    }
    if (tooltip && tooltip[0]) {
        p->set_desc(tooltip);
    }
    return var;
}

void ParamRegImpl::registerBoolVar_(const char* id, const char* name, const char* tp,
			   const char* tooltip, bool* var, bool val) {
    gx_engine::Parameter *p = pmap->reg_par(id, name, var, val);
    if (tooltip && tooltip[0]) {
        p->set_desc(tooltip);
    }
}

void ParamRegImpl::registerEnumVar_(const char *id, const char* name, const char* tp,
			       const char* tooltip, const value_pair* values, float *var,
			       float val, float low, float up, float step) {
    if (!name[0]) {
        assert(strrchr(id, '.'));
        name = strrchr(id, '.')+1;
    }
    assert(step == 1.0);
    gx_engine::Parameter *p = pmap->reg_enum_par(id, name, values, var, val, low);
    if (tooltip && tooltip[0]) {
        p->set_desc(tooltip);
    }
}

void ParamRegImpl::registerIEnumVar_(const char *id, const char* name, const char* tp,
			       const char* tooltip, const value_pair* values,
			       int *var, int val) {
    if (!name[0]) {
        assert(strrchr(id, '.'));
        name = strrchr(id, '.')+1;
    }
    gx_engine::Parameter *p = pmap->reg_enum_par(id, name, values, var, val);
    if (tooltip && tooltip[0]) {
        p->set_desc(tooltip);
    }
}

void ParamRegImpl::registerNonMidiVar_(const char * id, bool*var, bool preset, bool nosave) {
    BoolParameter *p = pmap->reg_non_midi_par(id, var, preset);
    if (nosave) {
	p->setSavable(false);
    }
}


/****************************************************************
 ** class Plugin
 */

Plugin::Plugin(PluginDef *pl)
    : box_visible(false),
      plug_visible(false),
      on_off(false),
      position(0),
      effect_post_pre(1),
      pdef(pl) {
}

Plugin::Plugin(gx_system::JsonParser& jp)
    : box_visible(false),
      plug_visible(false),
      on_off(false),
      position(0),
      effect_post_pre(0),
      pdef(new PluginDef()) {
    jp.next(gx_system::JsonParser::begin_object);
    while (jp.peek() != gx_system::JsonParser::end_object) {
	jp.next(gx_system::JsonParser::value_key);
	if (jp.current_value() == "box_visible") {
	    jp.next();
	    box_visible = true;
	} else if (jp.current_value() == "plug_visible") {
	    jp.next();
	    plug_visible = true;
	} else if (jp.current_value() == "on_off") {
	    jp.next();
	    on_off = true;
	} else if (jp.current_value() == "post_pre") {
	    jp.next(gx_system::JsonParser::value_number);
	    effect_post_pre = jp.current_value_int();
	} else if (jp.current_value() == "position") {
	    jp.next(gx_system::JsonParser::value_number);
	    position = jp.current_value_int();
	} else if (jp.current_value() == "version") {
	    jp.next(gx_system::JsonParser::value_number);
	    pdef->version = jp.current_value_int();
	} else if (jp.current_value() == "flags") {
	    jp.next(gx_system::JsonParser::value_number);
	    pdef->flags = jp.current_value_int();
	} else if (jp.current_value() == "id") {
	    jp.next(gx_system::JsonParser::value_string);
	    pdef->id = strdup(jp.current_value().c_str()); //FIXME
	} else if (jp.current_value() == "name") {
	    jp.next(gx_system::JsonParser::value_string);
	    pdef->name = strdup(jp.current_value().c_str()); //FIXME
	} else if (jp.current_value() == "groups") {
	    jp.next(gx_system::JsonParser::begin_array);
	    std::vector<std::string> v;
	    while (jp.peek() != gx_system::JsonParser::end_array) {
		jp.next(gx_system::JsonParser::value_string);
		v.push_back(jp.current_value());
	    }
	    jp.next(gx_system::JsonParser::end_array);
	    const char **p  = new const char*[v.size()+1];
	    pdef->groups = p;
	    for (std::vector<std::string>::iterator i = v.begin(); i != v.end(); ++i) {
		*p++ = strdup(i->c_str()); //FIXME
	    }
	    *p++ = 0;
	} else if (jp.current_value() == "description") {
	    jp.next(gx_system::JsonParser::value_string);
	    pdef->description = strdup(jp.current_value().c_str()); //FIXME
	} else if (jp.current_value() == "category") {
	    jp.next(gx_system::JsonParser::value_string);
	    pdef->category = strdup(jp.current_value().c_str()); //FIXME
	} else if (jp.current_value() == "shortname") {
	    jp.next(gx_system::JsonParser::value_string);
	    pdef->shortname = strdup(jp.current_value().c_str()); //FIXME
	}
    }
    jp.next(gx_system::JsonParser::end_object);
}

void Plugin::writeJSON(gx_system::JsonWriter& jw) {
    jw.begin_object();
    if (box_visible) {
	jw.write_key("box_visible");
	jw.write(true);
    }
    if (plug_visible) {
	jw.write_key("plug_visible");
	jw.write(true);
    }
    if (on_off) {
	jw.write_key("on_off");
	jw.write(true);
    }
    if (effect_post_pre) {
	jw.write_key("post_pre");
	jw.write(effect_post_pre);
    }
    jw.write_key("position");
    jw.write(position);
    jw.write_key("version");
    jw.write(pdef->version);
    jw.write_key("flags");
    jw.write(pdef->flags); //FIXME
    jw.write_key("id");
    jw.write(pdef->id);
    if (pdef->name) {
	jw.write_key("name");
	jw.write(pdef->name);
    }
    if (pdef->groups) {
	jw.write_key("groups");
	jw.begin_array();
	for (const char **p = pdef->groups; *p; p++) {
	    jw.write(*p);
	}
	jw.end_array();
    }
    if (pdef->description) {
	jw.write_key("description");
	jw.write(pdef->description);
    }
    if (pdef->category) {
	jw.write_key("category");
	jw.write(pdef->category);
    }
    if (pdef->shortname) {
	jw.write_key("shortname");
	jw.write(pdef->shortname);
    }
    jw.end_object();
}

void Plugin::set_box_visible(bool v) {
    box_visible = v;
}

void Plugin::set_plug_visible(bool v) {
    plug_visible = v;
}

void Plugin::set_on_off(int v) {
    on_off = v;
}

void Plugin::set_position(int pos) {
    position = pos;
}

void Plugin::set_effect_post_pre(int v) {
    effect_post_pre = v;
}


/****************************************************************
 ** class PluginList
 */

PluginListBase::PluginListBase() : pmap() {}
PluginListBase::~PluginListBase() {}

PluginList::PluginList(gx_ui::GxUI& ui_, EngineControl& seq_)
    : PluginListBase(), seq(seq_), ui(ui_) {
    plugin_pos[PLUGIN_POS_START]       = -1000;
    plugin_pos[PLUGIN_POS_RACK]        = 1;
    plugin_pos[PLUGIN_POS_END]         = 1000;
    plugin_pos[PLUGIN_POS_RACK_STEREO] = 1;
};

PluginList::~PluginList() {
    for (list<RackChangerUiItemBase*>::iterator i = rackchanger.begin(); i != rackchanger.end(); ++i) {
	delete(*i);
    }
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); ++p) {
	PluginDef *pdef = p->second->pdef;
	if (!(pdef->flags & PGNI_NOT_OWN)) {
	    if (pdef->delete_instance) {
		pdef->delete_instance(pdef);
	    }
	    delete p->second;
	}
    }
}

Plugin *PluginListBase::find_plugin(const char *id) const {
    pluginmap::const_iterator p = pmap.find(id);
    if (p == pmap.end()) {
	return 0;
    }
    return p->second;
}

Plugin *PluginListBase::lookup_plugin(const char *id) const {
    Plugin *p = find_plugin(id);
    if (!p) {
	gx_system::gx_print_fatal(
	    _("lookup plugin"),
	    boost::format("id not found: %1%") % id);
    }
    return p;
}

int* PluginList::pos_var(const char *id) {
    Plugin *p = find_plugin(id);
#ifndef NDEBUG
    if (!p) {
	gx_system::gx_print_error(
	    "Debug Check",
	    string("plugin position unknown id: ") + id);
	static int dummy = 0;
	return &dummy;
    }
#endif
    return &p->position;
}

bool* PluginList::on_off_var(const char *id) {
    Plugin *p = find_plugin(id);
#ifndef NDEBUG
    if (!p) {
	gx_system::gx_print_error(
	    "Debug Check",
	    string("plugin on_off unknown id: ") + id);
	static bool dummy;
	return &dummy;
    }
#endif
    return &p->on_off;
}

int PluginList::load_library(const string& path, PluginPos pos) {
    void* handle = dlopen(path.c_str(), RTLD_LOCAL|RTLD_NOW);
    if (!handle) {
	gx_system::gx_print_error(
	    _("Plugin Loader"),
	    boost::format(_("Cannot open library: %1%")) % dlerror());
        return -1;
    }
    dlerror();    // reset errors
    plugin_inifunc get_gx_plugin = (plugin_inifunc) dlsym(handle, "get_gx_plugin");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
	gx_system::gx_print_error(
	    _("Plugin Loader"),
	    boost::format(_("Cannot load symbol 'get_gx_plugin': %1%")) % dlsym_error);
        dlclose(handle);
        return -1;
    }
    int n = get_gx_plugin(0, 0);
    if (n <= 0) {
	return -1;
    }
    int cnt = 0;
    for (int i = 0; i < n; i++) {
	PluginDef *p;
	if (get_gx_plugin(i, &p) < 0) {
	    continue;
	}
	if (!add(p, pos)) {
	    cnt++;
	    gx_system::gx_print_info(_("Plugin Loader"), Glib::ustring::compose("loaded[%1]: %2", path, p->id));
	}
    }
    return cnt;
}

int PluginList::load_from_path(const string& path, PluginPos pos) {
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(path.c_str())) == NULL) {
	gx_system::gx_print_warning(
	    _("Plugin Loader"),
	    boost::format(_("Error opening '%1%'")) % path);
        return -1;
    }
    int cnt = 0;
    while ((dirp = readdir(dp)) != NULL) {
	string n = dirp->d_name;
	if (n.size() > 3 && n.compare(n.size()-3,3,".so") == 0) {
	    int res = load_library(path+n, pos);
	    if (res > 0) {
		cnt += res;
	    }
	}
    }
    closedir(dp);
    return cnt;
}

int PluginList::check_version(PluginDef *p) {
    if (((p->version & PLUGINDEF_VERMAJOR_MASK) == (PLUGINDEF_VERSION & PLUGINDEF_VERMAJOR_MASK)) &&
	((p->version & PLUGINDEF_VERMINOR_MASK) <= (PLUGINDEF_VERSION & PLUGINDEF_VERMINOR_MASK))) {
	return 0;
    }
    gx_system::gx_print_error(
	_("Plugin Loader"),
	boost::format(_("Plugin '%1%' has wrong version %2$#4x (current version: %3$#4x)"))
	% p->id % p->version % PLUGINDEF_VERSION);
    return -1;
}

void PluginList::delete_module(Plugin *pl, ParamMap& param, ParameterGroups& groups) {
    unregisterPlugin(pl, param, groups);
    PluginDef *p = pl->pdef;
#ifndef NDEBUG // avoid unused variable compiler warning
    size_t n = pmap.erase(p->id);
    assert(n == 1);
#else
    pmap.erase(p->id);
#endif
    if (!(p->flags & PGNI_NOT_OWN)) {
	if (p->delete_instance) {
	    p->delete_instance(p);
	}
	delete pl;
    }
}

int PluginList::add_module(Plugin *pvars, PluginPos pos, int flags) {
    const int mode_mask = (PGN_MODE_NORMAL|PGN_MODE_BYPASS|PGN_MODE_MUTE);  // all mode bits
    PluginDef *p = pvars->pdef;
    p->flags |= flags;
    if (!(p->flags & mode_mask)) {
	p->flags |= PGN_MODE_NORMAL;
    }
    if (p->stereo_audio) {
	p->flags |= PGN_STEREO;
    }
    if (p->load_ui) {
	p->flags |= PGN_GUI;
    }
    int ipos = pos;
    if (ipos == PLUGIN_POS_RACK) {
	p->flags |= PGNI_DYN_POSITION;
	if (p->flags & PGN_STEREO) {
	    ipos = PLUGIN_POS_RACK_STEREO;
	}
    }
    if (!(p->flags & (PGN_GUI|PGN_ALTERNATIVE))) {
	// otherwise would be always off
	pvars->on_off = true;
    }
    pvars->position = plugin_pos[ipos];
    if (p->flags & PGN_POST) {
	pvars->effect_post_pre = 0;
    }
    pair<pluginmap::iterator,bool> ret = pmap.insert(map_pair(p->id, pvars));
    if (!ret.second) {
	gx_system::gx_print_error(
	    _("Plugin Loader"),
	    boost::format(_("Plugin '%1%' already exists: skipped")) % p->id);
	return -1;
    }
    if (!(p->flags & PGN_ALTERNATIVE)) {
	// normal case: position will not be set by ModuleSelector
	plugin_pos[ipos]++;
    }
    return 0;
}

int PluginList::add(Plugin *pvars, PluginPos pos, int flags) {
    if (check_version(pvars->pdef) != 0) {
	return -1;
    }
    return add_module(pvars, pos, flags|PGNI_NOT_OWN);
}

int PluginList::add(PluginDef *p, PluginPos pos, int flags) {
    if (check_version(p) != 0) {
	return -1;
    }
    Plugin *pl = new Plugin(p);
    int ret = add_module(pl, pos, flags);
    if (ret != 0) {
	delete pl;
    }
    return ret;
}

int PluginList::add(PluginDef **p, PluginPos pos, int flags) {
    int count = 0;
    while (*p) {
        if (add(*p++, pos, flags) == 0) {
	    count++;
	}
    }
    return count;
}

int PluginList::add(plugindef_creator *p, PluginPos pos, int flags) {
    int count = 0;
    while (*p) {
        if (add((*p++)(), pos, flags) == 0) {
	    count++;
	}
    }
    return count;
}

static const char* tr_name(const char *name) {
    if (name && name[0]) {
	return gettext(name);
    }
    return "";
}

template<class T>
class RackChangerUiItem: public RackChangerUiItemBase {
private:
    PluginList& pluginlist;
    T *fZone;
    T  fCache;
public :
    RackChangerUiItem(PluginList& pl, T *z);
    ~RackChangerUiItem();
    virtual bool hasChanged();
    virtual void reflectZone();
    virtual void *zone();
};

template<class T>
RackChangerUiItem<T>::RackChangerUiItem(PluginList& pl, T *z)
    : pluginlist(pl), fZone(z), fCache() {
    pl.rackchanger.push_back(this);
    pl.ui.registerZone(z, this);
}

template<class T>
RackChangerUiItem<T>::~RackChangerUiItem() {
    pluginlist.ui.unregisterZone(fZone, this);
}

template<class T>
void *RackChangerUiItem<T>::zone() {
    return fZone;
}

template<class T>
bool RackChangerUiItem<T>::hasChanged() {
    return *fZone != fCache;
}

template<class T>
void RackChangerUiItem<T>::reflectZone() {
    fCache = *fZone;
    pluginlist.seq.set_rack_changed();
}

void PluginList::registerGroup(PluginDef *pd, ParameterGroups& groups) {
    groups.insert(pd->id, tr_name(pd->name));
    const char **gp = pd->groups;
    if (gp) {
	while (*gp) {
	    string id = *gp++;
	    const char *name = *gp++;
	    if (!name) {
		break;
	    }
	    if (id[0] == '.') {
		id = id.substr(1);
	    } else {
		id = string(pd->id) + "." + id;
	    }
	    groups.insert(id, tr_name(name));
	}
    }
}

void PluginList::unregisterGroup(PluginDef *pd, ParameterGroups& groups) {
    groups.erase(pd->id);
    const char **gp = pd->groups;
    if (gp) {
	while (*gp) {
	    string id = *gp++;
	    const char *name = *gp++;
	    if (!name) {
		break;
	    }
	    if (id[0] == '.') {
		id = id.substr(1);
	    } else {
		id = string(pd->id) + "." + id;
	    }
	    groups.erase(id);
	}
    }
}

void PluginList::registerParameter(Plugin *pl, ParamMap& param, ParamRegImpl& preg) {
    PluginDef *pd = pl->pdef;
    if (pd->load_ui || (pd->flags & PGN_GUI)) {
	string s = pd->id;
	param.reg_par((s+".on_off").c_str(),N_("on/off"), &pl->on_off, 0);
	new RackChangerUiItem<bool>(*this, &pl->on_off);
	if (pd->flags & PGNI_DYN_POSITION || !(pd->flags & PGN_FIXED_GUI)) {
	    param.reg_non_midi_par("ui."+s, &pl->box_visible, true);
	    param.reg_non_midi_par(s+".s_h", &pl->plug_visible, false);
	}
	if (pd->flags & PGNI_DYN_POSITION) {
	    // PLUGIN_POS_RACK .. PLUGIN_POS_POST_START-1
	    param.reg_non_midi_par(s+".position", &pl->position, true,
				   pl->position, 0, 999);
	    new RackChangerUiItem<int>(*this, &pl->position);
	    if (pd->mono_audio || (pd->flags & PGN_POST_PRE)) {
		if (pd->flags & PGN_PRE) {
		    pl->effect_post_pre = 1;
		} else if (pd->flags & PGN_POST) {
		    pl->effect_post_pre = 0;
		} else {
		    static const value_pair post_pre[] = {{N_("post")}, {N_("pre")}, {0}};
		    param.reg_enum_par((s+".pp").c_str(), "select", post_pre,
					&(pl->effect_post_pre), 0);
		    new RackChangerUiItem<int>(*this, &pl->effect_post_pre);
		}
	    }
	}
    }
    if (pd->register_params) {
	preg.plugin = pd;
	pd->register_params(preg);
    }
}

void PluginList::unregisterParameter(Plugin *pl, ParamMap& param) {
    PluginDef *pd = pl->pdef;
    string s = pd->id;
    if (pd->load_ui || (pd->flags & PGN_GUI)) {
	param.unregister(s+".on_off");
	if (pd->flags & PGNI_DYN_POSITION || !(pd->flags & PGN_FIXED_GUI)) {
	    param.unregister("ui."+s);
	    param.unregister(s+".s_h");
	}
	if (pd->flags & PGNI_DYN_POSITION) {
	    param.unregister(s+".position");
	    if (pd->mono_audio || (pd->flags & PGN_POST_PRE)) {
		if (!(pd->flags & (PGN_PRE|PGN_POST))) {
		    param.unregister(s+".pp");
		    for (list<RackChangerUiItemBase*>::iterator i = rackchanger.begin(); i != rackchanger.end(); ) {
			if ((*i)->zone() == &pl->on_off || (*i)->zone() == &pl->effect_post_pre) {
			    list<RackChangerUiItemBase*>::iterator j = i;
			    ++i;
			    rackchanger.erase(j);
			} else {
			    ++i;
			}
		    }
		}
	    }
	}
    }
    std::vector<const std::string*> l;
    if (pd->register_params) {
	s += ".";
	for (ParamMap::iterator i = param.begin(); i != param.end(); ++i) {
	    if (i->first.compare(0, s.size(), s) == 0) {
		assert(i->second->isInPreset());
		l.push_back(&i->first);
	    }
	}
    }
    for (std::vector<const std::string*>::iterator i = l.begin(); i != l.end(); ++i) {
	param.unregister(**i);
    }
}

void PluginList::registerPlugin(Plugin *pl, ParamMap& param, ParameterGroups& groups) {
    registerGroup(pl->pdef, groups);
    ParamRegImpl preg(&param);
    registerParameter(pl, param, preg);
}

void PluginList::unregisterPlugin(Plugin *pl, ParamMap& param, ParameterGroups& groups) {
    ParamRegImpl preg(&param);
    unregisterParameter(pl, param);
    unregisterGroup(pl->pdef, groups);
}

void PluginList::registerAllPlugins(ParamMap& param, ParameterGroups& groups) {
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	registerGroup(p->second->pdef, groups);
    }
    ParamRegImpl preg(&param);
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	registerParameter(p->second, param, preg);
    }
}

void PluginListBase::append_rack(UiBuilderBase& ui) {
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	ui.load(p->second);
    }
}

static bool plugin_order(Plugin* p1, Plugin* p2) {
    return p1->position_weight() < p2->position_weight();
}

void PluginList::ordered_mono_list(list<Plugin*>& mono, int mode) {
    mono.clear();
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	Plugin *pl = p->second;
	if (pl->on_off && pl->pdef->mono_audio && (pl->pdef->flags & mode)) {
	    mono.push_back(pl);
	}
    }
    mono.sort(plugin_order);
}

void PluginList::ordered_stereo_list(list<Plugin*>& stereo, int mode) {
    stereo.clear();
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	Plugin *pl = p->second;
	if (pl->on_off && pl->pdef->stereo_audio && (pl->pdef->flags & mode)) {
	    stereo.push_back(pl);
	}
    }
    stereo.sort(plugin_order);
}

void PluginList::ordered_list(list<Plugin*>& l, bool stereo, int flagmask, int flagvalue) {
    flagmask |= (PGN_STEREO | PGN_MODE_NORMAL);
    if (stereo) {
	flagvalue |= PGN_STEREO;
    }
    flagvalue |= PGN_MODE_NORMAL;
    l.clear();
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	PluginDef *pd = p->second->pdef;
	if (((pd->flags & flagmask) == flagvalue) || (!stereo && strcmp(pd->id, "ampstack") == 0)) {
	    l.push_back(p->second);
	}
    }
    l.sort(plugin_order);
}

void PluginListBase::writeJSON(gx_system::JsonWriter& jw) {
    jw.begin_array();
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	p->second->writeJSON(jw);
    }
    jw.end_array();
}

void PluginListBase::readJSON(gx_system::JsonParser& jp) {
    jp.next(gx_system::JsonParser::begin_array);
    while (jp.peek() != gx_system::JsonParser::end_array) {
	Plugin *p = new Plugin(jp);
	pmap.insert(map_pair(p->pdef->id, p));
    }
    jp.next(gx_system::JsonParser::end_array);
}

void PluginList::set_samplerate(int samplerate) {
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	inifunc f = p->second->pdef->set_samplerate;
	if (f) {
	    f(samplerate, p->second->pdef);
	}
    }
}

#ifndef NDEBUG
void PluginList::printlist(bool order) {
    list<Plugin*> pl_mono, pl_stereo;
    for (pluginmap::iterator p = pmap.begin(); p != pmap.end(); p++) {
	if (p->second->pdef->flags & PGN_STEREO) {
	    pl_stereo.push_back(p->second);
	} else {
	    pl_mono.push_back(p->second);
	}
    }
    if (order) {
	pl_mono.sort(plugin_order);
	pl_stereo.sort(plugin_order);
    }
    gx_engine::printlist("Plugin Map", pl_mono);
    gx_engine::printlist(0, pl_stereo, false);
}

void printlist(const char *title, const list<Plugin*>& modules, bool header) {
    if (!getenv("GUITARIX_MODULE_DEBUG")) {
	return;
    }
    const char *fmth = "%1s %-25s %5s %5s %3s %2s %3s %8s\n";
    const char *fmtl = "%1s %-25s %5d %5d %3d %2d %3d %8s\n";
    static int cnt = 0;
    if (header) {
	printf("%d %s:\n", ++cnt, title);
	printf(fmth, "F", "id","wght","pos","pre","on","vis","channels");
    } else {
	printf("\n");
    }
    for (list<Plugin*>::const_iterator i = modules.begin(); i != modules.end(); ++i) {
	Plugin *p = *i;
	PluginDef *pd = p->pdef;
	const char *c = "-";
	if (pd->mono_audio) {
	    c = "mono";
	} else if (pd->stereo_audio) {
	    c = "stereo";
	}
	const char *f;
	if (pd->flags & PGN_GUI) {
	    f = "";
	} else if (pd->flags & PGN_ALTERNATIVE) {
	    f = "A";
	} else {
	    f = "-";
	}
	printf(fmtl, f, pd->id, p->position_weight(), p->position,
	       p->effect_post_pre, p->on_off,p->box_visible, c);
    }
}
#endif

} // !namespace gx_engine
