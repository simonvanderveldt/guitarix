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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "GxLevelSlider.h"

#define P_(s) (s)   // FIXME -> gettext

#define get_stock_id(widget) (GX_LEVEL_SLIDER_CLASS(GTK_OBJECT_GET_CLASS(widget))->parent_class.stock_id)

static gboolean gx_level_slider_expose (GtkWidget *widget, GdkEventExpose *event);
static void gx_level_slider_size_request (GtkWidget *widget, GtkRequisition *requisition);
static gboolean gx_level_slider_button_press (GtkWidget *widget, GdkEventButton *event);
static gboolean gx_level_slider_pointer_motion (GtkWidget *widget, GdkEventMotion *event);

G_DEFINE_TYPE(GxLevelSlider, gx_level_slider, GX_TYPE_VSLIDER);

static void gx_level_slider_class_init(GxLevelSliderClass *klass)
{
	GtkWidgetClass *widget_class = (GtkWidgetClass*) klass;

	widget_class->expose_event = gx_level_slider_expose;
    widget_class->size_request = gx_level_slider_size_request;
	widget_class->button_press_event = gx_level_slider_button_press;
	widget_class->motion_notify_event = gx_level_slider_pointer_motion;
	widget_class->enter_notify_event = NULL;
	widget_class->leave_notify_event = NULL;
	klass->parent_class.stock_id = "levelslider";
	gtk_widget_class_install_style_property(
		widget_class,
		g_param_spec_int("slider-width",P_("size of slider"),
		                   P_("Height of movable part of vslider"),
		                 0, 100, 5, GParamFlags(G_PARAM_READABLE|G_PARAM_STATIC_STRINGS)));
}

static void gx_level_slider_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	g_assert(GX_IS_LEVEL_SLIDER(widget));
    GxLevelSlider *ls = GX_LEVEL_SLIDER(widget);
	gint slider_height;
	gtk_widget_style_get(widget, "slider-width", &slider_height, NULL);
	requisition->width = gdk_pixbuf_get_width(ls->pixbuf);
	if (requisition->height < slider_height) requisition->height = slider_height;
	_gx_regler_calc_size_request(GX_REGLER(widget), requisition);
}

static void level_slider_expose(
	GtkWidget *widget, GdkRectangle *rect, gdouble sliderstate)
{
    GxLevelSlider *ls = GX_LEVEL_SLIDER(widget);
	GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));
	sliderstate = rect->height * log_meter(adj->value);
	cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(widget));
	//gdk_cairo_set_source_pixbuf(cr, ls->pixbuf, rect->x, rect->y - (gint)sliderstate);
	//cairo_rectangle(cr, rect->x, rect->y, rect->width, rect->height);
	//cairo_fill(cr);
	cairo_destroy(cr);
}

static gboolean gx_level_slider_expose(GtkWidget *widget, GdkEventExpose *event)
{
	g_assert(GX_IS_LEVEL_SLIDER(widget));
    GxLevelSlider *ls = GX_LEVEL_SLIDER(widget);
	gint slider_height;
	GdkRectangle image_rect, value_rect;
	gtk_widget_style_get(widget, "slider-width", &slider_height, NULL);
	image_rect.width = widget->allocation.width;
	image_rect.height = widget->allocation.height;
	_gx_regler_get_positions(GX_REGLER(widget), &image_rect, &value_rect);
    gdouble sliderstate = _gx_regler_get_step_pos(GX_REGLER(widget), image_rect.height);
	
	level_slider_expose(widget, &image_rect, sliderstate);
    
	_gx_regler_simple_display_value(GX_REGLER(widget), &value_rect);
	return FALSE;
}

static double log_meter_inv(double def)
{
    def *= 115.0;
    if (def <= 0.0) {
        return -70.0;
	}
    if (def <= 2.5) {
        return def/0.25 - 70;
	}
    if (def <= 7.5) {
        return (def-2.5)/0.5 - 60;
	}
    if (def <= 15.0) {
        return (def-7.5)/0.75 - 50;
	}
    if (def <= 30.0) {
        return (def-15.0)/1.5 - 40;
	}
    if (def <= 50.0) {
        return (def-30.0)/2.0 - 30;
	}
    if (def <= 115) {
        return (def-50.0)/2.5 - 20;
	}
    return 6.0;
}

static gboolean slider_set_from_pointer(GtkWidget *widget, int state, gdouble x, gdouble y, gboolean drag, gint button, GdkEventButton *event)
{
	GdkRectangle image_rect, value_rect;
    GxLevelSlider *ls = GX_LEVEL_SLIDER(widget);
	gint slider_height;
	gtk_widget_style_get(widget, "slider-width", &slider_height, NULL);
    image_rect.width  = gdk_pixbuf_get_width(ls->pixbuf);
	image_rect.height = gdk_pixbuf_get_height(ls->pixbuf);
	image_rect.height  = (image_rect.height + slider_height) / 2;
	x += widget->allocation.x;
	y += widget->allocation.y;
	_gx_regler_get_positions(GX_REGLER(widget), &image_rect, &value_rect);
	if (!drag && !_approx_in_rectangle(x, y, &image_rect)) {
		return FALSE;
	}
	if (button == 3) {
		gboolean ret;
		g_signal_emit_by_name(GX_REGLER(widget), "value-entry", &image_rect, event, &ret);
		return FALSE;
	}
	static double last_y = 2e20;
	
	GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));
	double slidery = image_rect.height;
	double posy = slidery - y + image_rect.y; 
	double value;
	if (!drag) {
		last_y = posy;
		if (event && event->type == GDK_2BUTTON_PRESS) {
		    gtk_range_set_value(GTK_RANGE(widget), log_meter_inv(posy/image_rect.height));
		}
		return TRUE;
	}
    double sc = 0.005;
    if (state & GDK_CONTROL_MASK) {
        sc = 0.0005;
    }
	value = (posy - last_y) * sc;
	last_y = posy;
	gtk_range_set_value(GTK_RANGE(widget), adj->value + value * (adj->upper - adj->lower));
	return TRUE;
}

static gboolean gx_level_slider_button_press (GtkWidget *widget, GdkEventButton *event)
{
	g_assert(GX_IS_LEVEL_SLIDER(widget));
	if (event->button != 1 && event->button != 3) {
		return FALSE;
	}
	gtk_widget_grab_focus(widget);
	if (slider_set_from_pointer(widget, event->state, event->x, event->y, FALSE, event->button, event)) {
		gtk_grab_add(widget);
	}
	return FALSE;
}

static gboolean gx_level_slider_pointer_motion(GtkWidget *widget, GdkEventMotion *event)
{
	g_assert(GX_IS_LEVEL_SLIDER(widget));
	if (!gtk_widget_has_grab(widget)) {
		return FALSE;
	}
	gdk_event_request_motions (event);
	slider_set_from_pointer(widget, event->state, event->x, event->y, TRUE, 0, NULL);
	return FALSE;
}


static void gx_level_slider_init(GxLevelSlider *level_slider)
{
	gtk_widget_set_name (GTK_WIDGET(level_slider),"rack_slider");
    level_slider->pixbuf = gtk_widget_render_icon(GTK_WIDGET(level_slider), get_stock_id(GTK_WIDGET(level_slider)), GtkIconSize(-1), NULL);
}
