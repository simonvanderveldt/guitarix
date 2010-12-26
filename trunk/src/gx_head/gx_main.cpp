/*
 * Copyright (C) 2009, 2010 Hermann Meyer, James Warden, Andreas Degert
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * ---------------------------------------------------------------------------
 *
 *    name         : "gx_head"
 *    version      : "0.04.7"
 *    authors      : "hermann meyer" & "james warden"
 *    contributors : "Julius O. Smith (jos at ccrma.stanford.edu)"
 *    license      : "GPL"
 *    copyright    : "(c)brummer 2008"
 *    reference    : "http://ccrma.stanford.edu/realsimple/faust_strings/"
 *
 *    Code prototype generated with Faust 0.9.9.4f (http://faust.grame.fr)
 * ----------------------------------------------------------------------------
 *
 *    This is gx_head main.
 *
 * ----------------------------------------------------------------------------
 */

#include <giomm/init.h>
#include <gtkmm/main.h>
#include <gxwmm/init.h>
#include "guitarix.h"

using namespace gx_system;

void init_unix_signal_handlers()
{
	/* ----- block signal USR1 ---------
	** inherited by all threads which are created later
	** USR1 is processed synchronously by gx_signal_helper_thread
	*/
	sigset_t waitset;
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGUSR1);
	sigaddset(&waitset, SIGCHLD);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	//----- set unix signal handlers for proper shutdown
	signal(SIGQUIT, gx_signal_handler);
	signal(SIGTERM, gx_signal_handler);
	signal(SIGHUP,  gx_signal_handler);
	signal(SIGINT,  gx_signal_handler);
	//signal(SIGSEGV, gx_signal_handler); // no good, quits application silently
}

/* --------- Guitarix main ---------- */
int main(int argc, char *argv[])
{
#ifdef ENABLE_NLS
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif
	init_unix_signal_handlers();

	// ----------------------- init basic subsystems ----------------------
	Glib::thread_init();
	Glib::init();
	Gxw::init();

	// ------ initialize parameter list ------
	gx_engine::audio.register_parameter();
	gx_engine::midi.register_parameter();
	gx_engine::register_faust_parameters();
	gx_gui::register_gui_parameter();
	gx_preset::init();
	gx_gui::parameter_map.set_init_values();

	// ---------------------- user options handling ------------------
	string optvar[NUM_SHELL_VAR];
	gx_process_cmdline_options(argc, argv, optvar);
	Gtk::Main main(argc, argv);

	// ---------------- Check for working user directory  -------------
	gx_system::gx_version_check();

	// ------ time measurement (debug) ------
#ifndef NDEBUG
	add_time_measurement();
#endif

	// ----------------------- init GTK interface----------------------
	gx_gui::GxMainInterface* gui = gx_gui::GxMainInterface::instance("gx_head");
	gui->setup();


	// ---------------------- initialize jack client ------------------
	if (gx_jack::gx_jack_init(optvar))
	{
		// -------- initialize gx_head engine --------------------------
		gx_engine::gx_engine_init(optvar);

		// -------- set jack callbacks and activation -------------------
		gx_jack::gx_jack_callbacks_and_activate();

		// -------- init port connections
		gx_jack::gx_jack_init_port_connection(optvar);
	}

	// ----------------------- run GTK main loop ----------------------
	gx_set_override_options(optvar);
	gx_ui::GxUI::updateAllGuis();
	g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	g_object_set (gtk_settings_get_default (), "gtk-menu-images", TRUE, NULL);
	gui->show();

	if (gx_jack::client) {
	    // -------- pitch tracker (needs jack thread running) -------------
		gx_engine::pitch_tracker.init();
	}

	//Gxw::Knob::set_jump_to_mouse(false);
	gui->run();

	// ------------- shut things down
	gx_system::gx_clean_exit(NULL, NULL);

	return 0;
}