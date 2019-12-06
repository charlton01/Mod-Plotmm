/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
#include <gtkmm/window.h>
#include <gdkmm/general.h>
#include <gtkmm/widget.h>
//#include <gdkmm/gc.h>
#include <gdkmm/color.h>

#include "paint.h"

using namespace PlotMM;



    Paint::Paint() :
    size_(1),
    sizeB_(-1),
    filled_(false)
{
}

Paint::Paint(const Paint &s)
{
    size_= s.size_;
    sizeB_= s.sizeB_;
    filled_= s.filled_;
}

Paint::~Paint()
{
}

bool Paint::operator!=(const Paint &) const
{
    return true;
}

bool Paint::operator==(const Paint &) const
{
    return true;
}

/*! Set the color of the pen
 *
 *  The pen is used to draw lines or outlines of objects
 */
void Paint::set_pen_color(const Gdk::RGBA &c)
{
    pColor_= c;
}

/*! Set the color of the brush
 *
 *  The pen is used to fill objects
 */
void Paint::set_brush_color(const Gdk::RGBA &c)
{
    bColor_= c;
    filled_= true;
}

void Paint::set_cr_to_pen(const Cairo::RefPtr<Cairo::Context> &gc)
{
    gc->set_source_rgb(pColor_.get_red(), pColor_.get_green(), pColor_.get_blue());
}

void Paint::set_cr_to_brush(const Cairo::RefPtr<Cairo::Context> &gc)
{
    gc->set_source_rgb(bColor_.get_red(), bColor_.get_green(), bColor_.get_blue());
}

/*
//! Return Pen (only outline)
//Glib::RefPtr<Gdk::GC> Paint::pen(const Glib::RefPtr<Gdk::Drawable> &w) const
Cairo::RefPtr<Cairo::Context> Paint::pen(const Glib::RefPtr<Gdk::Window> &w) const
{
    //Glib::RefPtr<Gdk::GC> gc= Gdk::GC::create(w);
    Cairo::RefPtr<Cairo::Context> gc = w->create_cairo_context();
    //gc->set_rgb_fg_color(pColor_);
//    gc->set_source_rgba(1, pColor_.get_red(), pColor_.get_green(), pColor_.get_blue());
    gc->set_source_rgb(pColor_.get_red(), pColor_.get_green(), pColor_.get_blue());

//    printf("The color of the pen is %f  %f %f\n", pColor_.get_red(), pColor_.get_green(), pColor_.get_blue());
    return gc;
}

//! Return Brush (no fill)
//Glib::RefPtr<Gdk::GC> Paint::brush(const Glib::RefPtr<Gdk::Drawable> &w) const
Cairo::RefPtr<Cairo::Context> Paint::brush(const Glib::RefPtr<Gdk::Window> &w) const
{
    //Glib::RefPtr<Gdk::GC> gc= Gdk::GC::create(w);
    Cairo::RefPtr<Cairo::Context> gc = w->create_cairo_context();
    //gc->set_rgb_fg_color(bColor_);
//    gc->set_source_rgba(1, bColor_.get_red(), bColor_.get_green(), bColor_.get_blue());

    gc->set_source_rgb(bColor_.get_red(), bColor_.get_green(), bColor_.get_blue());
    return gc;
}
*/
