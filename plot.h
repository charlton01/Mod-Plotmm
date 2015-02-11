/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#ifndef PLOTMM_PLOT_H
#define PLOTMM_PLOT_H

#include <map>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

#include <cairomm/context.h>
#include <cairomm/fontoptions.h>
#include <cairo/cairo.h>

#include <gtkmm/drawingarea.h>
#include <gtkmm/table.h>
#include <gtkmm/frame.h>

#include "scale.h"
#include "rectangle.h"
#include "doubleintmap.h"


namespace Gdk {
class Pixmap;
}


namespace Pango {
class Layout;
class FontDescription;
}

namespace PlotMM
{
class Plot;
class Curve;

// test class
class TestArea : public Gtk::DrawingArea
    {
    public:

    protected:
//	virtual bool on_expose_event(GdkEventExpose* event);
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);

    private:
    };

// ************
    /*! @brief Class to draw labels in a plot.
     *
     */
class PlotLabel : public Gtk::DrawingArea
    {
    public:
	PlotLabel(Gtk::Widget *w,Gtk::Orientation o,const Glib::ustring & ="");
	void set_text(const Glib::ustring &);
	Glib::ustring get_text();
	Pango::FontDescription *font() { return &font_; }

	void set_enabled(bool b);
	bool enabled() const { return enabled_; }

    protected:
//	virtual bool on_expose_event(GdkEventExpose* event);
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);

	virtual void on_realize();
	virtual void update_();

	Glib::RefPtr<Gdk::Pixbuf> rotate_right_(Glib::RefPtr<Gdk::Pixbuf>);
	Glib::RefPtr<Gdk::Pixbuf> rotate_left_(Glib::RefPtr<Gdk::Pixbuf>);

	Glib::RefPtr<Gdk::Window> window_;

	Pango::FontDescription font_;

    private:
	Glib::RefPtr<Gdk::Pixbuf> buffer_;
	Glib::RefPtr<Pango::Layout> layout_;
	Gtk::Widget *widget_;
	Gtk::Orientation orientation_;
	bool tainted_;
	bool enabled_;
    };

    /*! @brief Class to draw curves in a plot.
     *
     */
     //
class PlotCanvas : public Gtk::DrawingArea
    {
    public:
//	PlotCanvas(Plot* pp, Gtk::Widget* w);
	PlotCanvas(Plot* pp);
	virtual ~PlotCanvas();

	virtual bool begin_replot();
	virtual void clear();
	virtual void end_replot();

	//sigc::signal3<void,int,int,GdkEventButton*> signal_plot_mouse_press;
	//sigc::signal3<void,int,int,GdkEventButton*> signal_plot_mouse_release;
	//sigc::signal3<void,int,int,GdkEventMotion*> signal_plot_mouse_move;

    protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&cr);
	virtual void on_realize();
	//virtual bool on_button_press_event(GdkEventButton* event);
	//virtual bool on_button_release_event(GdkEventButton* event);
	//virtual bool on_motion_notify_event (GdkEventMotion* event);


	Glib::RefPtr<Gdk::Window> window_;
    private:
	bool replotting_;
	Gdk::Color black_, grey_, white_;

	Plot* plot_ptr;
    };

    /*! brief Class relating a curve to a plot.
     *
     */
    /*
class PlotCurve
    {
    public:
	void set_enabled(bool b) { enabled_= b; }
	bool enabled() { return enabled_; }
    };
    */

    /*! @brief defines available plot axes
     */
    enum PlotAxisID
    {
	AXIS_TOP,
	AXIS_BOTTOM,
	AXIS_LEFT,
	AXIS_RIGHT
    };

    /*! @brief Class implementing a plot widget.
     *
     */
class Plot : public Gtk::Frame
    {
    public:
	Plot();
	virtual ~Plot();

	int add_curve(const Glib::RefPtr<Curve> &,
		     PlotAxisID xaxis=AXIS_BOTTOM,
		     PlotAxisID yaxis=AXIS_LEFT,
		     bool enable= true);

	bool replot();
	bool replot2(const Cairo::RefPtr<Cairo::Context> &);
	bool reset_autoscale();

    TestArea *myarea() {return &myarea_;}
    PlotCanvas *canvas() {return &canvas_;}
	PlotLabel *title() { return &title_; }
	Scale *scale(PlotAxisID id) { return tickMark_[id]; }
	PlotLabel *label(PlotAxisID id) { return axisLabel_[id]; }

	//sigc::signal3<void,int,int,GdkEventButton*> signal_plot_mouse_press();
	//sigc::signal3<void,int,int,GdkEventButton*> signal_plot_mouse_release();
	//sigc::signal3<void,int,int,GdkEventMotion*> signal_plot_mouse_move();

	void set_selection(const Rectangle &r);
	Rectangle get_selection() { return select_; }
	void enable_selection();
	void disable_selection();


    protected:
    Gtk::Box m_box1;
	Gtk::Table layout_;
	TestArea myarea_;

	std::vector<Scale*> tickMark_;
	std::vector<ScaleLabels*> tickLabel_;
	std::vector<PlotLabel*> axisLabel_;
	PlotLabel title_;
	PlotCanvas canvas_;
//	virtual bool on_canvas_expose_event(GdkEventExpose* event);
	virtual void draw_selection_();

    private:
    Cairo::RefPtr<Cairo::Pattern> pattern_;
	int loop;
	double alpha;
	int curveidx_;
	bool draw_select_;
	Rectangle select_;
	Gdk::Color black_, grey_, white_;
	struct CurveInfo {
	    Glib::RefPtr<Curve> curve;
	    PlotAxisID xaxis;
	    PlotAxisID yaxis;
	};

	std::map<int,CurveInfo> plotDict_;
    };

}

#endif//PLOTMM_PLOT_H
