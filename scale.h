/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#ifndef PLOTMM_SCALE_H
#define PLOTMM_SCALE_H

#include <vector>
#include <map>

#include <sigc++/sigc++.h>

#include <gtkmm/drawingarea.h>

#include "doubleintmap.h"
#include "scalediv.h"

namespace Pango {
    class Layout;
    class FontDescription;
}

namespace PlotMM
{
    /*! @brief Class to draw scale labels
     *
     */
class ScaleLabels : public Gtk::DrawingArea {
    public:
	ScaleLabels(Gtk::PositionType p);
	virtual ~ScaleLabels();

	//! Return a pointer to the labels' font
	Pango::FontDescription *font() { return &font_; }
    void set_font(Pango::FontDescription fd) { font_ = fd;}
	void set_labels(int offs, const std::map<int,double> &labels);

	void set_enabled(bool b);
	//! Return if the labels are enabled
	bool enabled() const { return enabled_; }
	Glib::ustring format(double) const;
	int text_width(const Glib::ustring &) const;
	int text_height(const Glib::ustring &) const;
	//! Return where the labels are positioned
	Gtk::PositionType position() const { return position_; }

    protected:
	virtual void update_() {}
	virtual void newsize_();
	virtual void requestsize_() {}

    Glib::RefPtr<Gdk::Window> window_;
	int offset_;
	int layw_, layh_;
	int range_;
	std::map<int,double> labels_;
	Glib::RefPtr<Pango::Layout> layout_;
	Pango::FontDescription font_;

    private:
	Gtk::PositionType position_;
	bool enabled_;
    };

    /*! @brief Class to draw vertical scale labels
     *
     */
class VScaleLabels : public ScaleLabels {
    public:
	VScaleLabels(Gtk::PositionType p);
	virtual ~VScaleLabels();

    protected:
    virtual void on_realize();
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);

	virtual void update_(const Cairo::RefPtr<Cairo::Context> &);
	virtual void requestsize_();
    };

    /*! @brief Class to draw horizintal scale labels
     *
     */
class HScaleLabels : public ScaleLabels {
    public:
	HScaleLabels(Gtk::PositionType p);
	virtual ~HScaleLabels();

    protected:
	virtual void on_realize();
    	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);
	virtual void update_(const Cairo::RefPtr<Cairo::Context> &);
	virtual void requestsize_();
    };



    /*! @brief Class to draw a scale.
     *
     */
    class Scale : public Gtk::DrawingArea
    {
    public:
	Scale(Gtk::PositionType p, ScaleLabels *labels= 0);
	virtual ~Scale();

	ScaleDiv* get_scaleDiv(){ return &scaleDiv_; }

	void set_position(Gtk::PositionType p);
	//! Return where the scale is positioned
	Gtk::PositionType position() const { return position_; }

	void set_ticklengths(int major, int minor);
	//! Return the major ticklength
	int major_ticklength() const { return majorTL_; }
	//! Return the minor ticklength
	int minor_ticklength() const { return minorTL_; }

	void set_enabled(bool b);
	//! Query if the scale is enabled (i.e. visible)
	bool enabled() const { return enabled_; }

	//! Query the ScaleMap used by this scale
	DoubleIntMap &scale_map() { return scaleMap_; }

	void set_range(double l, double r);
	void set_range(double l, double r, bool lg);

	void set_autoscale(bool b);
	//! Query if autoscaling is enabled
	bool autoscale() const { return autoscale_; }
	void begin_autoscale();
	void autoscale(double min, double max);
	void end_autoscale();

	/*! This signal is thrown whenever the scale is enabled or disabled
	 *  \sa set_enabled, enabled
	 */
	sigc::signal1<void,bool> signal_enabled;

    protected:
	virtual void on_realize();

	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);

	virtual void redraw(Cairo::RefPtr<Cairo::Context> const &cr) = 0;  //  Be careful. This makes this an abstract class.

	/*! implement this to react on changes of tick lengths */
	// note that virtual methods are those that can be redeclared in subclasses.
	// virtual methods can be called from the base class.
	virtual void on_tick_change() {}

	Glib::RefPtr<Gdk::Window> window_;
	Glib::RefPtr<Gdk::Window> labwin_;
	ScaleLabels *labels_;

	ScaleDiv scaleDiv_;
	DoubleIntMap scaleMap_;

    private:
	Gtk::PositionType position_;
	bool autoscale_;
	bool enabled_;
	int majorTL_, minorTL_;
	double asMin_, asMax_;
    };

    /*! @brief Class to draw a vertical scale.
     *
     */
    class VScale : public Scale
    {
    public:
	VScale(Gtk::PositionType p, ScaleLabels *l= 0);
	virtual ~VScale();

    protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);
	virtual void redraw(const Cairo::RefPtr<Cairo::Context> &cr);
	virtual void on_tick_change();
    };

    /*! @brief Class to draw a horizontal scale.
     *
     */
class HScale : public Scale
    {
    public:
	HScale(Gtk::PositionType p, ScaleLabels *l= 0);
	virtual ~HScale();

    protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);
	virtual void redraw(const Cairo::RefPtr<Cairo::Context> &cr);
	virtual void on_tick_change();
    };


}

#endif
