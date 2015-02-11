/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#include <vector>

//#include <gdkmm/drawable.h>

#include "symbol.h"
#include "paint.h"

using namespace PlotMM;

/*!
  \brief Constructor
  \param style Symbol Style
  \param size horizontal size
  \param sizeb optional vertical size (defaults to horizontal size)
*/
Symbol::Symbol(SymbolStyleID style, int size, int sizeb):
    size_(size),
    sizeB_(sizeb),
    style_(style)
{
    paint_= Glib::RefPtr<Paint> (new Paint);
}

//! Destructor
Symbol::~Symbol()
{
}

/*!
  \brief Specify the symbol's size

  In most cases, the vertical size is the same as the horizontal size.
  It is perfectly sufficient to set the horizontal size.  size() and
  size_aux() will return the same value, then.

  \param w horizontal size
  \param h optional vertical size (defaults to horizontal size)
*/
void Symbol::set_size(int w, int h)
{
    size_ = w; sizeB_= h;
}

/*!
  \brief Draw the symbol at a point (x,y).
*/
void Symbol::draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &painter,
		  const Gdk::Rectangle& r) const
{
    draw_vfunc_(cr, painter, r.get_x(), r.get_y(), r.get_width(), r.get_height());
}

/*!  \brief Draw the symbol at the specified position with optionally
  specified size
*/
void Symbol::draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &painter,
		  int x1, int y1, int w, int h) const
{
    draw_vfunc_(cr, painter, x1, y1, w, h);
}

/*!
  \brief Draw the symbol at a specified point

  \param painter Painter
  \param pos Center of the symbol
*/
void Symbol::draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &painter,
		  const Gdk::Point &pos) const
{
    draw_vfunc_(cr, painter, pos.get_x(), pos.get_y(), size_, sizeB_);
}


/*! \brief Implementation of the drawing function
 *
 *  To draw the symbol this function makes use of paint() - the pen is
 *  used to draw outlines whilst the brush is used to fill the symbol.
 */
void Symbol::draw_vfunc_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &painter,
			 int x1, int y1, int w, int h) const
{
    if (w<0) w= size_;
    if (h<0) h= sizeB_;
    if (h<0) h= size_;

    const int x0 = x1-w/2;
    const int y0 = y1-h/2;
    const int x2 = x0+w;
    const int y2 = y0+h;

    bool filled= paint()->filled();
//    if(filled)printf("Filled...\n");

    switch(style_)
    {
        case SYMBOL_ELLIPSE:
	    if (filled)
	    {
            paint()->set_cr_to_brush(cr);
            cr->save();
            cr->translate (x1, y1);
            cr->scale(w/2, h/2);
            cr->arc(0, 0, 1, 0, 2*M_PI);
            cr->fill();
            cr->restore();
	    }
		paint()->set_cr_to_pen(cr);
		cr->save();
		cr->set_line_width(0.5);
		cr->translate (x1, y1);
		cr->scale(w/2, h/2);
		cr->arc(0, 0, 1, 0, 2*M_PI);
		cr->stroke();
		cr->restore();
            break;
        case SYMBOL_RECTANGLE:
            //painter->draw_rectangle(pen,filled,x0,y0,w,h);
            if (filled)
            {
		paint()->set_cr_to_brush(cr);
                cr->move_to(x0,y0);
                cr->line_to(x0+w,y0);
                cr->line_to(x0+w,y0+h);
                cr->line_to(x0,y0+h);
                cr->close_path();
                cr->fill();
            }
 //           else{
		paint()->set_cr_to_pen(cr);
                cr->move_to(x0,y0);
                cr->line_to(x0+w,y0);
                cr->line_to(x0+w,y0+h);
                cr->line_to(x0,y0+h);
                cr->close_path();
                cr->stroke();
//                }
            break;
        case SYMBOL_DIAMOND:
        {
            if (filled)
            {
		paint()->set_cr_to_pen(cr);
                cr->move_to(x1,y0);
                cr->line_to(x2,y1);
                cr->line_to(x1,y2);
                cr->line_to(x0,y1);
                cr->close_path();
                cr->stroke();

		paint()->set_cr_to_brush(cr);
                cr->move_to(x1,y0);
                cr->line_to(x2,y1);
                cr->line_to(x1,y2);
                cr->line_to(x0,y1);
                cr->close_path();
                cr->fill();

            }


            break;
        }
        case SYMBOL_CROSS:
	    paint()->set_cr_to_pen(cr);
            cr->move_to(x1,y0);
            cr->line_to(x1,y2);
	    cr->move_to(x0,y1);
            cr->line_to(x2,y1);
            cr->stroke();

            //painter->draw_line(pen,x1,y0,x1,y2);
            //painter->draw_line(pen,x0,y1,x2,y1);
            break;
        case SYMBOL_XCROSS:
	    paint()->set_cr_to_pen(cr);
            cr->move_to(x0,y0);
            cr->line_to(x2,y2);
            cr->stroke();
            cr->move_to(x0,y2);
            cr->line_to(x2,y0);
            cr->stroke();

            break;
        case SYMBOL_TRIANGLE:
        case SYMBOL_U_TRIANGLE:
	        paint()->set_cr_to_brush(cr);
                cr->move_to(x1,y0);
                cr->line_to(x2,y2);
                cr->line_to(x0,y2);
                cr->close_path();
                cr->fill();

            break;

        case SYMBOL_D_TRIANGLE:
        {
	        paint()->set_cr_to_brush(cr);
                cr->move_to(x0,y0);
                cr->line_to(x2,y0);
                cr->line_to(x1,y2);
                cr->close_path();
                cr->fill();

        }
            break;
        case SYMBOL_L_TRIANGLE:
        {
	        paint()->set_cr_to_brush(cr);
                cr->move_to(x0,y0);
                cr->line_to(x2,y1);
                cr->line_to(x0,y2);
                cr->close_path();
                cr->fill();

            break;
        }
        case SYMBOL_R_TRIANGLE:
        {
                paint()->set_cr_to_brush(cr);
		cr->move_to(x2,y0);
                cr->line_to(x0,y1);
                cr->line_to(x2,y2);
                cr->close_path();
                cr->fill();

            break;
        }
        default:;
    }
}

/*!
  \brief Specify the symbol style

  The following styles are defined:<dl>
  <dt>SYMBOL_NONE<dd>No Style. The symbol cannot be drawn.
  <dt>SYMBOL_ELLIPSE<dd>Ellipse or circle
  <dt>SYMBOL_RECTANGLE<dd>Rectangle
  <dt>SYMBOL_DIAMOND<dd>Diamond
  <dt>SYMBOL_TRIANGLE<dd>Triangle pointing upwards
  <dt>SYMBOL_D_TRIANGLE<dd>Triangle pointing downwards
  <dt>SYMBOL_U_TRIANGLE<dd>Triangle pointing upwards
  <dt>SYMBOL_L_TRIANGLE<dd>Triangle pointing left
  <dt>SYMBOL_R_TRIANGLE<dd>Triangle pointing right
  <dt>SYMBOL_CROSS<dd>Cross
  <dt>SYMBOL_XCROSS<dd>Diagonal cross</dl>
  \param s style
*/
void Symbol::set_style(SymbolStyleID s)
{
    style_ = s;
}

//! == operator
bool Symbol::operator==(const Symbol &other) const
{
    return
	paint() == other.paint() &&
	style() == other.style() &&
	size() == other.size();
}

//! != operator
bool Symbol::operator!=(const Symbol &other) const
{
    return !(*this == other);
}

/*! Return a reference to the Paint the symbol is drawn with
 */
Glib::RefPtr<Paint> Symbol::paint() const
{
    return paint_;
}

