/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
#include "paint.h"
//#include <gtkmm/drawingarea.h>

#include <gdkmm/rectangle.h>

#include <pangomm/layout.h>

#include <gtkmm/window.h>
#include <gtkmm/button.h>

#include "curve.h"
//#include "paint.h"
#include "plot.h"

#include <iostream>
#include <sigc++/sigc++.h>
#include <gdkmm/general.h> // set_source_pixbuf()

#include <sys/time.h>

using namespace PlotMM;


bool TestArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
return false;

    Glib::RefPtr<Gdk::Window> win_;
    win_ = get_window();
//    PL_cr = win_->create_cairo_context();
//    PL_cr->set_line_width(12);


    cr->set_source_rgb(0.0, 0.5, 0.);
    cr->paint();
    cr->set_source_rgb(0.5, 0.0, 0.5);
    cr->move_to(2,2);
//    cr->line_to(10,10);
    cr->show_text("jim");
    int winx, winy, winw, winh;
 //   window_->get_geometry(winx, winy, winw, winh, wind);
    win_->get_geometry(winx, winy, winw, winh);
//    printf("TestArea::on_draw window_ x, y, width and height are %d   %d   %d   %d\n", winx, winy, winw, winh);



    cr->stroke();
    //window_->show();

    return false;
}


PlotLabel::PlotLabel(Gtk::Widget *w,Gtk::Orientation o,
		     const Glib::ustring &text) :
    widget_(w),
    orientation_(o),
    tainted_(true),
    enabled_(false)
{
	set_app_paintable();
    layout_= create_pango_layout("");
    font_= w->get_pango_context()->get_font_description();
    set_text(text);
}

void PlotLabel::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  // Now we can allocate any additional resources we need
  window_= get_window();
	update_();
}

/*! Set the text for the label
 *
 *  If the given text is an empty string the displayed label will
 *  show nothing but will still require some space.
 */
void PlotLabel::set_text(const Glib::ustring &str)
{
    if (str=="")
	layout_->set_text(" ");
    else
	layout_->set_text(str);
    layout_->set_font_description(font_);
    layout_->context_changed();
    update_();

}

/*! Query the label's text
 */
Glib::ustring PlotLabel::get_text()
{
    Glib::ustring str= layout_->get_text();
    return str==" " ? "" : str;
}

/*! Enable or disable displaying of the label's text
 *
 *  When disabled, the label will require no space
 */
void PlotLabel::set_enabled(bool b)
{
    if (b==enabled_) return;
    enabled_= b;
    update_();
}


bool PlotLabel::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{

    if (tainted_) update_();
    if (!enabled()) return true;

    Glib::RefPtr<Gdk::Window> win_;
    win_ = get_window();

    cr->move_to(100,1);
    int winx, winy, winw, winh;
    win_->get_geometry(winx, winy, winw, winh);

    int layw,layh;

    if (orientation_==Gtk::ORIENTATION_HORIZONTAL) {
	layout_->get_pixel_size(layw,layh);
    cr->move_to((winw-layw)/2,0);

    } else {
	layout_->get_pixel_size(layh,layw);
	cr->move_to(0,(winh+layh)/2);
	cr->rotate_degrees(-90);

    }

    layout_->show_in_cairo_context(cr);

    cr->stroke();


    return true;
}

void PlotLabel::update_()
{

    tainted_= true;
    if (!window_) return;
    //if (!buffer_) return;
    int layw,layh;
    layout_->get_pixel_size(layw,layh);
    int winx, winy, winw, winh;
    window_->get_geometry(winx, winy, winw, winh);

    Gdk::Rectangle area(0, 0, layw, layh);

    if (orientation_==Gtk::ORIENTATION_HORIZONTAL) {
	set_size_request(-1,enabled()?layh:0);
    } else {
	set_size_request(enabled()?layh:0,-1);
    }
    tainted_= false;

    
}

/*! Rotate the text label by 90 degree to the right
    I don't see where this is ever called.  But replacing Pixmap with Pixbuf
      Also changed in Plot.h
 */
 // not used now
Glib::RefPtr<Gdk::Pixbuf> PlotLabel::rotate_right_(Glib::RefPtr<Gdk::Pixbuf> p)
{
    // Pixbuf has a rotation method
    Glib::RefPtr<Gdk::Pixbuf> o = p->rotate_simple ((Gdk::PixbufRotation) 90);
    return o;
/*
    int w,h;
    p->get_size(w,h);
    Glib::RefPtr<Gdk::Pixbuf> o= Gdk::Pixbuf::create(window_,h,w,-1,-1);
    Glib::RefPtr<Gdk::Image> pi= p->get_image(0,0,w,h);
    Glib::RefPtr<Gdk::Image> oi=
	Gdk::Image::create(Gdk::IMAGE_NORMAL,get_visual(),h,w);
    for (int x= 0; x<w; ++x)
	for (int y= 0; y<h; ++y)
	    oi->put_pixel(h-y-1,x,pi->get_pixel(x,y));
    o->draw_image(gc_, oi, 0, 0, 0, 0,-1,-1);
    return o;
*/
}

/*! Rotate the text label by 90 degree to the left
 */
 // Not used now
Glib::RefPtr<Gdk::Pixbuf> PlotLabel::rotate_left_(Glib::RefPtr<Gdk::Pixbuf> p)
{
    // Pixbuf has a rotation method
    Glib::RefPtr<Gdk::Pixbuf> o = p->rotate_simple ((Gdk::PixbufRotation) -90);
    return o;

/*
    int w,h;
    p->get_size(w,h);
    Glib::RefPtr<Gdk::Pixmap> o= Gdk::Pixmap::create(window_,h,w,-1);
    Glib::RefPtr<Gdk::Image> pi= p->get_image(0,0,w,h);
    Glib::RefPtr<Gdk::Image> oi=
	Gdk::Image::create(Gdk::IMAGE_NORMAL,get_visual(),h,w);
    for (int x= 0; x<w; ++x)
	for (int y= 0; y<h; ++y)
	    oi->put_pixel(y,w-x-1,pi->get_pixel(x,y));
    o->draw_image(gc_, oi, 0, 0, 0, 0,-1,-1);
    return o;
*/
}


PlotCanvas::PlotCanvas(Plot* pp) :
    window_(0),
    replotting_(false)
{
  set_app_paintable();
  Gdk::RGBA m_Color;

  m_Color.set_red(0.0);
  m_Color.set_green(1.0);
  m_Color.set_blue(0.0);
  m_Color.set_alpha(1.0); //opaque

  //override_background_color(m_Color);
  //colormap->alloc_color(black_);
  //colormap->alloc_color(white_);
  //colormap->alloc_color(grey_);

  plot_ptr = pp;

  add_events(Gdk::EXPOSURE_MASK|
	     Gdk::BUTTON_PRESS_MASK|
	     Gdk::BUTTON_RELEASE_MASK|
	     Gdk::POINTER_MOTION_MASK|
	     Gdk::POINTER_MOTION_HINT_MASK|
	     Gdk::ENTER_NOTIFY_MASK|
	     Gdk::LEAVE_NOTIFY_MASK);

}


PlotCanvas::~PlotCanvas()
{
}

/*
bool PlotCanvas::on_button_press_event(GdkEventButton* event)
{
    int x,y;
    get_pointer(x,y);
    signal_plot_mouse_press(x,y,event);
    return true;
}

bool PlotCanvas::on_button_release_event(GdkEventButton* event)
{
    int x,y;
    get_pointer(x,y);
    signal_plot_mouse_release(x,y,event);
    return true;
}

bool PlotCanvas::on_motion_notify_event (GdkEventMotion* event)
{
    int x,y;
    get_pointer(x,y);
    signal_plot_mouse_move(x,y,event);
    return true;
}
*/
void PlotCanvas::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  // Now we can allocate any additional resources we need
  window_= get_window();

  // gc_ = Gdk::GC::create(window_);
  //  We are using a cairo context, not a GC
  //window_->set_background(grey_);
  //  I don't think we need to clear the window; done by cairo when needed
 // window_->clear();
 //  I could set the color and paint the background with Cairo
}

//! Clear the plot canvas (where the curves are drawn
void PlotCanvas::clear()
{
  //  I don't think we need to clear the window; done automatically??
 // window_->clear();
 // Again... I could set the color and paint the cairo::context if need be
}

/*! Start replot of the plot canvas' contents
 *
 *  When the replot takes place outside on_expose_event this method
 *  must be called to avoid flickering
 *
 *  \sa end_replot()
 */
bool PlotCanvas::begin_replot()
{
    if (replotting_ || !window_) return false;
    //window_->freeze_updates();
    // The following code is not needed.  It causes the window area to be place on a
    // backing-store which is cleared to the background color.  Drawing operations
    // to the window are made to the backing-store.  A subsequent end_paint() call returns
    // the image to the window.  But Cairo does not draw to the backing store.  So this is
    // not needed when using Cairo.  In fact it will wipe out the cairo image.
/*
    Gdk::Rectangle frame;
    window_->get_frame_extents(frame);
    frame.set_x(0); frame.set_y(0);
    window_->begin_paint_rect(frame);
 */
    return replotting_= true;
}

/*! End replot of the plot canvas' contents
 *
 *  If begin_replot() was called, this method must be called after
 *  changing the plot canvas' contents
 */
void PlotCanvas::end_replot()
{
    if (!replotting_) return;
//    window_->end_paint();
    //window_->thaw_updates();
    replotting_= false;
}


Plot::Plot() :
    layout_(7,9),
    title_(this,Gtk::ORIENTATION_HORIZONTAL),
    canvas_(this),
    curveidx_(-1),
    draw_select_(false)
{
//    add(m_box1);

    axisLabel_.push_back(new PlotLabel(this,Gtk::ORIENTATION_HORIZONTAL));
    axisLabel_.push_back(new PlotLabel(this,Gtk::ORIENTATION_HORIZONTAL));
    axisLabel_.push_back(new PlotLabel(this,Gtk::ORIENTATION_VERTICAL));
    axisLabel_.push_back(new PlotLabel(this,Gtk::ORIENTATION_VERTICAL));
    tickLabel_.push_back(new HScaleLabels(Gtk::POS_TOP));
    tickLabel_.push_back(new HScaleLabels(Gtk::POS_BOTTOM));
    tickLabel_.push_back(new VScaleLabels(Gtk::POS_LEFT));
    tickLabel_.push_back(new VScaleLabels(Gtk::POS_RIGHT));
    tickMark_.push_back(new HScale(Gtk::POS_TOP,tickLabel_[0]));
    tickMark_.push_back(new HScale(Gtk::POS_BOTTOM,tickLabel_[1]));
    tickMark_.push_back(new VScale(Gtk::POS_LEFT,tickLabel_[2]));
    tickMark_.push_back(new VScale(Gtk::POS_RIGHT,tickLabel_[3]));




    layout_.attach(canvas_,3,4,4,5);
    layout_.attach(title_, 0,7,0,1,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
//    layout_.attach(myarea_, 0,7,0,1,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);


    layout_.attach(*tickLabel_[0],1,6,2,3,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
    layout_.attach(*tickLabel_[1],1,6,6,7,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
    layout_.attach(*tickLabel_[2],1,2,3,6,Gtk::SHRINK,Gtk::FILL|Gtk::EXPAND);
    layout_.attach(*tickLabel_[3],5,6,3,6,Gtk::SHRINK,Gtk::FILL|Gtk::EXPAND);
    layout_.attach(*tickMark_[0], 3,4,3,4,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
    layout_.attach(*tickMark_[1], 3,4,5,6,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
    layout_.attach(*tickMark_[2], 2,3,4,5,Gtk::SHRINK,Gtk::FILL|Gtk::EXPAND);
    layout_.attach(*tickMark_[3], 4,5,4,5,Gtk::SHRINK,Gtk::FILL|Gtk::EXPAND);
    layout_.attach(*axisLabel_[0],1,6,1,2,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
    layout_.attach(*axisLabel_[1],1,6,7,8,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);
    layout_.attach(*axisLabel_[2],0,1,1,8,Gtk::SHRINK,Gtk::FILL|Gtk::EXPAND);
    layout_.attach(*axisLabel_[3],6,7,1,8,Gtk::SHRINK,Gtk::FILL|Gtk::EXPAND);

//
//    layout_.attach(myarea_,3,4,4,5,Gtk::FILL|Gtk::EXPAND,Gtk::SHRINK);

//    m_box1.pack_start(layout_);

    add(layout_);

     int fontsize= axisLabel_[0]->font()->get_size();

    // std::cerr << "fontsize is "<<fontsize<<"\n";

    axisLabel_[0]->font()->set_size(int(fontsize*.9));
    axisLabel_[1]->font()->set_size(int(fontsize*.9));
    axisLabel_[2]->font()->set_size(int(fontsize*.9));
    axisLabel_[3]->font()->set_size(int(fontsize*.9));
/*
    tickLabel_[0]->set_font(this->get_pango_context()->get_font_description());
    tickLabel_[1]->set_font(this->get_pango_context()->get_font_description());
    tickLabel_[2]->set_font(this->get_pango_context()->get_font_description());
    tickLabel_[3]->set_font(this->get_pango_context()->get_font_description());
*/

    tickLabel_[0]->font()->set_size(int(fontsize*.8));
    tickLabel_[1]->font()->set_size(int(fontsize*.8));
    tickLabel_[2]->font()->set_size(int(fontsize*.8));
    tickLabel_[3]->font()->set_size(int(fontsize*.8));

}


Plot::~Plot()
{
}

/*! @brief Add a new curve to the plot
 *
 *  The plot will not be updated - call replot() manually after adding
 *  new curves.
 */
int Plot::add_curve(const Glib::RefPtr<Curve> &cv,
		   PlotAxisID xaxis,
		   PlotAxisID yaxis,bool enable)
{
    curveidx_++;
    plotDict_[curveidx_].curve= cv;
    plotDict_[curveidx_].xaxis= xaxis;
    plotDict_[curveidx_].yaxis= yaxis;
    return curveidx_;
}
//  Don't want an on_draw() here as it will obscure the plot window after drawing.
//  PlotCanvas::on_draw() is sufficient... confirmed... on_draw() for DrawingArea objects only
/*
//bool Plot::on_canvas_expose_event(GdkEventExpose* event)

{

    replot();
    return false;  //allow signal propagation
}
*/

bool PlotCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>&cr)
{   

    plot_ptr->replot2(cr);
    return true;  //allow signal propagation
}

bool Plot::reset_autoscale()
{
        std::map<int,CurveInfo>::iterator cv;
    int axis;
    bool autoscale= false;

//   int winx, winy, winw, winh;
//    Glib::RefPtr<Gdk::Window> win_;
//    win_ = tickMark_[2]->get_window();
//    win_->get_geometry(winx, winy, winw, winh);

// Note below that tickMark is a vector of *Scale and axis 0-3 are TOP, BOTTOM, LEFTand RIGHT.
// so if any axis is marked for autoscale, begin_autoscale() is called on that axis
// and the local variable autoscale is set to true.

    for (axis=0; axis<4; ++axis) {
	if (tickMark_[axis]->autoscale()) {
	    tickMark_[axis]->begin_autoscale();
	    autoscale= true;
	}
    }

// if any axis is marked for autoscale then the following is carried out.
    if (autoscale) {
	for (cv= plotDict_.begin(); cv!=plotDict_.end(); ++cv) {
	    if (!cv->second.curve->enabled()) continue;
	    PlotAxisID xaxis= cv->second.xaxis;
	    PlotAxisID yaxis= cv->second.yaxis;
	    DoubleRect br= cv->second.curve->bounding_rect();
	    tickMark_[xaxis]->autoscale(br.get_x1(),br.get_x2());
	    tickMark_[yaxis]->autoscale(br.get_y1(),br.get_y2());
	}

// This ends the autoscaling for all axes marked for autoscale.
	for (axis=0; axis<4; ++axis) {
	    if (tickMark_[axis]->autoscale()) {
		tickMark_[axis]->end_autoscale();
	    }
	}

    } // end if (autoscale...
    return true;
}

/*! Replot all curves
 *
 *  After adding curves to the plot or changing the curves' data with
 *  Curve::setData() this method must be called to update the plot.
 *
 *  If autoscaling is activated for an axis used by any of the
 *  Curve::enabled() curves that axis will be scaled to a new range.
 */
bool Plot::replot()
{
    reset_autoscale();

    if(!draw_select_)
    {
    // hide and show_all on the Plot will redraw everything and erase the
    //  drag rectangle, if present.  Causes on_draw() for all widgets.

        hide();
        show_all();


    }

    return true;
}


bool Plot::replot2(const Cairo::RefPtr<Cairo::Context> &cr)  // This is now only called by the on_draw() signal
{
    std::map<int,CurveInfo>::iterator cv;


 if (!canvas_.begin_replot()) return true;



//    reset_autoscale();
//  draw to a backing store
cr->push_group();


    for (cv= plotDict_.begin(); cv!=plotDict_.end(); ++cv) {
	if (!cv->second.curve->enabled()) continue;
	PlotAxisID xaxis= cv->second.xaxis;
	PlotAxisID yaxis= cv->second.yaxis;
//     int cst = cv->second.curve->curve_style();
	// tickMark[xaxis]->scale_map() returns the DoubleIntMap for that axis.
	//  What sets the DoubleIntMap parameters?  They are set
	// by its constructor, redraw() and other things.
//    cv->second.curve->paint()->set_pen_GC(canvas_.gc());
//    cv->second.curve->paint()->set_brush_GC(canvas_.gc());
//    Make sure that the transform matrix is set up properly
    int winx, winy, winw, winh;

    tickMark_[xaxis]->get_window()->get_geometry(winx, winy, winw, winh);
    tickMark_[xaxis]->scale_map().set_int_range(0, winw-1);
    tickMark_[yaxis]->get_window()->get_geometry(winx, winy, winw, winh);
    tickMark_[yaxis]->scale_map().set_int_range(winh-1, 0);



//Bitter experience has shown that it is best not to store the Cairo::Context.  Pass it as argument

    cv->second.curve->draw(cr, canvas_.get_window(), tickMark_[xaxis]->scale_map(), tickMark_[yaxis]->scale_map());

    }

//  restore the image from the backing store after saving a copy to pattern_
pattern_.clear();
cr->pop_group_to_source();
pattern_ = cr->get_source();
//cr->set_source(pattern_);
cr->paint();
canvas_.end_replot();

return true;


}

/*
//! Return the signal owned by plot canvas
sigc::signal3<void,int,int,GdkEventButton*> Plot::signal_plot_mouse_press()
{
    return canvas_.signal_plot_mouse_press;
}

//! Return the signal owned by plot canvas
sigc::signal3<void,int,int,GdkEventButton*> Plot::signal_plot_mouse_release()
{
    return canvas_.signal_plot_mouse_release;
}

//! Return the signal owned by plot canvas
sigc::signal3<void,int,int,GdkEventMotion*> Plot::signal_plot_mouse_move()
{
    return canvas_.signal_plot_mouse_move;
}
*/
/*! Set the selection to the given rectangle.  If selection is
 *  enabled, the old rectangle is erased and the new is drawn.  Note
 *  that replot() does not have to be called explicitly.  No other
 *  canvas contents but the rectangle are redrawn calling this method.
 */
void Plot::set_selection(const PlotMM::Rectangle &r)
{
//    if (draw_select_) draw_selection_();

    select_= r;
    if (draw_select_) draw_selection_();
}

/*! Enable drawing of the selection rectangle.  The geometry can be
 *  changed using set_selection.  To actually draw the rectangle,
 *  replot() must be called.
    This is changed when using gtkmm and cairomm.  The rectangle is drawn
    and before another rectangle is drawn the original image is restored.
 */
void Plot::enable_selection()
{
    if (draw_select_) return;
    draw_select_= true;
    set_selection(select_);
}

/*! Disable drawing of the selection rectangle.  To actually erase the
 *  rectangle, replot() must be called.
 */
void Plot::disable_selection()
{
    if (!draw_select_) return;
    draw_select_= false;

}

//! draws the current selection rectangle on the plot canvas.
void Plot::draw_selection_()
{

 if ( select_.get_abs_width() <=0 || select_.get_abs_height() <=0) return;


    Paint paint;
    Cairo::RefPtr<Cairo::Context> gc_ = canvas()->get_window()->create_cairo_context();

    int winx, winy, winw, winh;
    canvas_.get_window()->get_geometry(winx, winy, winw, winh);


//  clear the screen to a background color and redraw the last canvas image if exists.
        gc_->set_source_rgb(0.9,0.9,0.9);
        
        gc_->paint();
        
        if(pattern_)
        {	
            gc_->set_source(pattern_);
            gc_->paint();
            
        }
// draw a rectangle on the image
    gc_->save();
    gc_->set_source_rgb(0.7,0.4,0.0);

    gc_->rectangle 	(select_.get_x_min(),
					 select_.get_y_min(),
					 select_.get_abs_width(),
					 select_.get_abs_height());

    gc_->stroke();
	
    gc_->restore();

}

