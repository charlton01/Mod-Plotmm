/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#include <pangomm/layout.h>

#include "scale.h"

#include <iostream>
#include <sstream>
#include <sys/time.h>

using namespace PlotMM;



/*! Constructor
 */
ScaleLabels::ScaleLabels(Gtk::PositionType p) :
    position_(p),  // p is POS_LEFT, POS_RIGHT, ... etc. gtk constants.
    enabled_(true)
{
    set_app_paintable();
    layout_= create_pango_layout("");

}

/*! Destructor */
ScaleLabels::~ScaleLabels()
{
}

/*! Set the labels to be drawn
 *
 *  The given map holds double values associated with int positions on
 *  a scale.  The given offs can be taken as a hint to calculate the
 *  right positions for the labels.  Normally, the leftmost or upmost
 *  border of the scale's Gdk::Window is taken for this offset.
 *
 *  \sa newsize_ (called automatically)
 */
void ScaleLabels::set_labels(int offs, const std::map<int,double> &labels)
{
    offset_= offs;
    labels_= labels;
    newsize_();
    update_();
}

/*! Enable drawing of scalelabels
 *
 *  \sa newsize_ (called automatically)
 */
void ScaleLabels::set_enabled(bool b)
{
    if (b==enabled_) return;
    enabled_= b;
    newsize_();
}

/*! Format a string from a double value
 *
 *  @todo find a smart way to user-define scale label formatting
 */
Glib::ustring ScaleLabels::format(double d) const
{
    char tmp[100];
    sprintf(tmp,"%g",d);  // prints a double to a string tmp
    return tmp;
}

/*! Query the pixel width of the given text when rendered with the
 *  current font() settings
 */
int ScaleLabels::text_width(const Glib::ustring &str) const
{
    int layw,layh;
    layout_->set_font_description(font_);
    layout_->set_text(str);
    layout_->context_changed();
    layout_->get_pixel_size(layw,layh);
    return layw;
}

/*! Query the pixel height of the given text when rendered with the
 *  current font() settings
 */
int ScaleLabels::text_height(const Glib::ustring &str) const
{
    int layw,layh;
    layout_->set_font_description(font_);
    layout_->set_text(str);
    layout_->context_changed();
    layout_->get_pixel_size(layw,layh);
    return layh;
}

/* ****************************************************************** */

/*! Constructor
 */
VScaleLabels::VScaleLabels(Gtk::PositionType p) :
    ScaleLabels(p)
{

}

/*! Destructor */
VScaleLabels::~VScaleLabels()
{
}

void VScaleLabels::on_realize()
{
    Gtk::DrawingArea::on_realize();
    window_= get_window();

}



bool VScaleLabels::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{

 //   newsize_();
    update_(cr);
    return false;
}

/* ****************************************************************** */

/*! Constructor
 */
HScaleLabels::HScaleLabels(Gtk::PositionType p) :
    ScaleLabels(p)
{
}

/*! Destructor */
HScaleLabels::~HScaleLabels()
{
}

void HScaleLabels::on_realize()
{
    Gtk::DrawingArea::on_realize();
    window_= get_window();
}



bool HScaleLabels::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    update_(cr);
    return false;
}

/* ****************************************************************** */

void ScaleLabels::newsize_()
{
    if (!labels_.size()) return;  // if there are no labels, return
    if (!enabled()) {
	set_size_request(0,0);  // make as small as possible if not drawn
	return;
    } else {
	layh_= text_height("8,8");
	layw_= 0;
	int min, max;
	min= max= labels_.begin()->first;

	std::map<int,double>::iterator daPos;
	for (daPos= labels_.begin(); daPos!=labels_.end(); ++daPos) {
	    layw_= MAX(layw_,text_width(format(daPos->second)));
	    min= MIN(min,daPos->first);
	    max= MAX(max,daPos->first);
	}
	range_= max-min;
    }
    requestsize_();
}

void VScaleLabels::requestsize_()
{
    Glib::RefPtr<Gdk::Window> window= get_window();
    if (!window) return;

//    int winx,winy,winw,winh,wind;
//    window->get_geometry(winx, winy, winw, winh);
    set_size_request(layw_,-1);
}

void HScaleLabels::requestsize_()
{
    Glib::RefPtr<Gdk::Window> window= get_window();
    if (!window) return;

//    int winx,winy,winw,winh,wind;
//    window->get_geometry(winx, winy, winw, winh);
    set_size_request(-1,layh_);
}

/* ****************************************************************** */

void VScaleLabels::update_(const Cairo::RefPtr<Cairo::Context> &cr)
{

    if (!enabled()) return;
    if (!labels_.size()) return;
    Glib::RefPtr<Gdk::Window> window= get_window();
    if (!window) return;

    int layw,layh;
    int winx,winy,winw,winh;
    window->get_geometry(winx, winy, winw, winh);

    // blank the scalelabel area to white and then reset cairo color to black
//    cr->set_source_rgb(1.0, 1.0, 1.0);
//    cr->paint();
    cr->set_source_rgb(0.0, 0.0, 0.0);


    std::map<int,double>::iterator daPos;
    layout_->set_font_description(font_);
    for (daPos= labels_.begin(); daPos!=labels_.end(); ++daPos) {
	layout_->set_text(format(daPos->second));
	layout_->context_changed();
	layout_->get_pixel_size(layw,layh);
	int lx=0,ly=offset_-winy-layh/2+daPos->first;
	if (position()==Gtk::POS_LEFT) {
	    lx= winw-layw;
	}
	if (ly<0) ly=0;
	else if (ly+layh>winh) ly=winh-layh;

    cr->move_to(lx, ly);

    layout_->show_in_cairo_context (cr);

    cr->stroke();
    }

}

void HScaleLabels::update_(const Cairo::RefPtr<Cairo::Context> &cr)
{
    if (!enabled()) return;
    if (!labels_.size()) return;
    Glib::RefPtr<Gdk::Window> window= get_window();
    if (!window) return;

    // blank the scalelabel area to white and then reset cairo color to black
//    cr->set_source_rgb(1.0, 1.0, 1.0);
//    cr->paint();
    cr->set_source_rgb(0.0, 0.0, 0.0);

    int layw,layh;
    int winx,winy,winw,winh;
    window->get_geometry(winx, winy, winw, winh);
    //window->clear();

    std::map<int,double>::iterator daPos;
    layout_->set_font_description(font_);
    for (daPos= labels_.begin(); daPos!=labels_.end(); ++daPos)
     {
        layout_->set_text(format(daPos->second));
        layout_->context_changed();
        layout_->get_pixel_size(layw,layh);
        int lx=offset_-winx-layw/2+daPos->first, ly=0;
        if (position()==Gtk::POS_TOP)
            ly= winh-layh;
        if (lx<0)
            lx=0;
        else if (lx+layw>winw)
            lx=winw-layw;

        cr->move_to(lx, ly);

        layout_->show_in_cairo_context(cr);
    }
        cr->stroke();

}


/* ****************************************************************** */


/*! Constructor
 *
 *  Upon construction, the position for the Scale must be declared.
 *  Optionally, a reference to a ScaleLabels instance can be provided
 *  to allow for automatic label generation.  If the labels are actually
 *  shown can be set with ScaleLabels::set_enabled()
 */
Scale::Scale(Gtk::PositionType p, ScaleLabels *l) :
    labels_(l),   //  Note this is the letter el not the number one.
    position_(p),
    enabled_(false),
    majorTL_(10),
    minorTL_(5)
{
    set_app_paintable();
    set_range(-1,1,false);
}

/*! Destructor */
Scale::~Scale()
{
}

bool Scale::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    return false;
}
/*! Change the position of the Scale (may not work)
 *
 * @todo check this out
 */
void Scale::set_position(Gtk::PositionType p)
{
    position_= p;
}

/*! Enable drawing of scales
 *
 *  If scale drawing is disabled, no space will be occupied and
 *  nothing will be displayed.  If a ScaleLabels instance was
 *  specified, the ScaleLabels will also be disabled.
 *
 *  To re-enable Scale and ScaleLabels drawing, call this function
 *  with true.
 */
void Scale::set_enabled(bool b)
{
    if (labels_) labels_->set_enabled(b);
    if (b==enabled_) return;
    enabled_= b;
    on_tick_change();
    signal_enabled(enabled_);
}

/*! Set the length of major and minor ticks */
void Scale::set_ticklengths(int major, int minor)
{
    majorTL_= major;
    minorTL_= minor;
    on_tick_change();
}

/*! Do the necessary initializations when window is realized.
 */
void Scale::on_realize()
{
    Gtk::DrawingArea::on_realize();
#if 0
    if (labels_) {
	labwin_= labels_->get_window();  // never used!
    }
#endif
    //gc_ = Gdk::GC::create(window_);
    window_= get_window();

    on_tick_change();
}

/*! Set the range of double values the scale represents.
 *
 *  To make the scale logarithmic, true can be given as the third
 *  argument.  After setting the new range, the scale is redrawn.
 */
void Scale::set_range(double l, double r)
{
    set_range(l,r,scaleMap_.logarithmic());
}

/*! Set the range of double values the scale represents.
 *
 *  To make the scale logarithmic, true can be given as the third
 *  argument.  After setting the new range, the scale is redrawn.
 */
void Scale::set_range(double l, double r, bool lg)
{

    scaleMap_.set_dbl_range(l,r,false);

    if (scaleMap_.logarithmic())
	scaleDiv_.rebuild(l, r, 10, 10, true, 0.0);
    else
	scaleDiv_.rebuild(l, r, 10, 10, false, 0.0);
//    if (window_)
//    {
//        window_->hide(); window_->show();
//    }

}

/*! Turn on autoscaling
 */
void Scale::set_autoscale(bool b)
{
    autoscale_= b;
}

/*! Reset autoscale information
 *
 *  Before updating autoscale information with autoscale(min,max),
 *  begin_autoscale must be called to reset the old autoscale
 *  information.  Call end_autoscale() when finished.
 */
void Scale::begin_autoscale()
{
    if (!autoscale_) return;
    asMin_= 1; asMax_= -1;
}

/*! Add autoscale information
 *
 *  The given minimum and maximum value extend the current autoscale
 *  information.  Call end_autoscale() when finished.
 */
void Scale::autoscale(double min, double max)
{
    if (!autoscale_) return;
    if (asMin_ > asMax_) {
	asMin_= MIN(min,max);
	asMax_= MAX(min,max);
    } else {
	asMin_= MIN(asMin_,MIN(min,max));
	asMax_= MAX(asMax_,MAX(min,max));
    }
}

/*! Finish update of autoscale information
 *
 *  After updating autoscale information with autoscale(min,max)
 *  this function sets the new range of values.
 *
 *  \sa set_range
 */
void Scale::end_autoscale()
{
    if (!autoscale_) return;
    if (asMin_ != asMax_) {
	set_range(asMin_,asMax_,scaleMap_.logarithmic());
    }
}

/*! Handles an expose event */

//bool Scale::on_expose_event(GdkEventExpose* event)
//bool 	Scale::on_event (GdkEvent* event)



/*! Constructor
 *
 *  \see Scale::Scale()
 */
VScale::VScale(Gtk::PositionType p, ScaleLabels *l) : Scale(p,l)
{
}

/*! Destructor */
VScale::~VScale()
{
}

bool VScale::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    redraw(cr);
    return false;
}

/*! Requests a size for the drawingarea that can hold the major tick */
void VScale::on_tick_change()
{
    if (enabled())
	set_size_request(major_ticklength(),-1);
    else
	set_size_request(1,-1);
}

/*! Draws the scale  (line and tickmarks) */
void VScale::redraw(const Cairo::RefPtr<Cairo::Context> &VS_cr)
{

    if (!window_) return;

    int winx, winy, winw, winh;
    get_window()->get_geometry(winx, winy, winw, winh);


//    VS_cr = window_->create_cairo_context();


	VS_cr->set_line_width(1.0);
    VS_cr->set_source_rgb(0.0,0.0,0.0);

    scaleMap_.set_int_range(winh-1,0);
    int i, iy;
    double y;
    int l0= 0;
    int l1= minor_ticklength();
    int l2= major_ticklength();
    if (position()==Gtk::POS_LEFT) {
	l0= winw;
	l1= l0-l1;
	l2= l0-l2;
    }
    std::map<int,double> lblpos;
    for (i=0;i<(int)scaleDiv_.maj_count();i++) {
	y= scaleDiv_.maj_mark(i);
	iy= scaleMap_.transform(y);
	if (labels_) lblpos[iy]= y;
	//window_->draw_line(gc_,l0,iy,l2,iy);
	VS_cr->move_to(l0, iy);
	VS_cr->line_to(l2, iy);
	VS_cr->stroke();
    }
    for (i=0;i<(int)scaleDiv_.min_count();i++) {
	y= scaleDiv_.min_mark(i);
	iy= scaleMap_.transform(y);
	//window_->draw_line(gc_,l0,iy,l1,iy);
	VS_cr->move_to(l0, iy);
	VS_cr->line_to(l1, iy);
	VS_cr->stroke();
    }
    //window_->draw_line(gc_,l0,0,l0,winh-1);
    VS_cr->move_to(l0, 0);
	VS_cr->line_to(l0, winh-1);
	VS_cr->stroke();

    if (labels_) labels_->set_labels(winy,lblpos);

//    show_all();
}

/*! Constructor
 *
 *  \see Scale::Scale()
 */
HScale::HScale(Gtk::PositionType p, ScaleLabels *l) : Scale(p,l)
{
}

/*! Destructor */
HScale::~HScale()
{
}

bool HScale::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    redraw(cr);
    return false;
}

/*! Requests a size for the drawingarea that can hold the major tick */
void HScale::on_tick_change()
{
    if (enabled())
	set_size_request(-1,major_ticklength());
    else
	set_size_request(-1,1);
}

/*! Draws the scale */
void HScale::redraw(const Cairo::RefPtr<Cairo::Context> &HS_cr)
{

    if (!window_) return;
    int winx, winy, winw, winh;
    window_->get_geometry(winx, winy, winw, winh);

//    HS_cr = window_->create_cairo_context();

    HS_cr->set_line_width(1.0);
    HS_cr->set_source_rgb(0.0,0.0,0.0);
    //window_->clear();
    scaleMap_.set_int_range(0,winw-1);
    int i, ix;
    double x;
    int l0= 0;
    int l1= minor_ticklength();
    int l2= major_ticklength();
    if (position()==Gtk::POS_TOP) {
	l0= winh;
	l1= l0-l1;
	l2= l0-l2;
    }

    std::map<int,double> lblpos;
    for (i=0;i<(int)scaleDiv_.maj_count();i++) {
	x= scaleDiv_.maj_mark(i);
	ix= scaleMap_.transform(x);
	if (labels_) lblpos[ix]= x;
	//window_->draw_line(gc_,ix,l0,ix,l2);
	HS_cr->move_to(ix, l0);
	HS_cr->line_to(ix, l2);
	HS_cr->stroke();
    }
    for (i=0;i<(int)scaleDiv_.min_count();i++) {
	x= scaleDiv_.min_mark(i);
	ix= scaleMap_.transform(x);
	//window_->draw_line(gc_,ix,l0,ix,l1);
	HS_cr->move_to(ix, l0);
	HS_cr->line_to(ix, l1);
	HS_cr->stroke();
    }
    //window_->draw_line(gc_,0,l0,winw-1,l0);
    HS_cr->move_to(0, l0);
	HS_cr->line_to(winw-1, l0);
	HS_cr->stroke();
    if (labels_) labels_->set_labels(winx,lblpos);
}

