/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#include <glibmm/refptr.h>

//#include <gdkmm/drawable.h>

#include "doubleintmap.h"
#include "errorcurve.h"
#include "paint.h"
#include "supplemental.h"

using namespace PlotMM;


//! Initialize data members
void ErrorCurve::init(const Glib::ustring &title)
{
    Curve::init(title);
    epaint_= Glib::RefPtr<Paint> (new Paint);
}

//! Copy the contents of a curve into another curve
void ErrorCurve::copy(const Curve &c)
{
    dx_.clear();
    dy_.clear();
    Curve::copy(c);
}

//! Copy the contents of a curve into another curve
void ErrorCurve::copy(const ErrorCurve &c)
{
    dx_= c.dx_;
    dy_= c.dy_;
    epaint_ = c.epaint_;
    Curve::copy(c);
}

//! Destructor
ErrorCurve::~ErrorCurve()
{
}

/*!
  \brief Ctor
  \param title title of the curve
*/
ErrorCurve::ErrorCurve(const Glib::ustring &title)
{
    init(title);
}

//! Copy Constructor
ErrorCurve::ErrorCurve(const ErrorCurve &c)
{
    init(c.title());
    copy(c);
}

//! Copy Assignment
const ErrorCurve& ErrorCurve::operator=(const ErrorCurve &c)
{
    if (this != &c)
    {
        copy(c);
        curve_changed();
    }

    return *this;
}

/*! Return a reference to the Paint the curve is drawn with
 */
Glib::RefPtr<Paint> ErrorCurve::error_paint() const
{
    return epaint_;
}

/*!
  \brief Initialize data with x- and y-arrays (explicitly shared)

  \param xData x data
  \param yData y data

  @todo handle unevenly sized arrays correctly
*/
void ErrorCurve::set_data(const std::vector<double> &xData,
			 const std::vector<double> &yData)
{
    dx_.clear();
    dy_.clear();
    Curve::set_data(xData,yData);
}

/*!
  \brief Set data by copying x- and y-values from specified memory blocks
  This function makes a 'deep copy' of the data.

  \param xData pointer to x values
  \param yData pointer to y values
  \param size size of xData and yData

  \sa QwData::set_data.
  @todo handle null pointers correctly
*/
void ErrorCurve::set_data(const double *xData, const double *yData, int size)
{
    dx_.clear();
    dy_.clear();
    Curve::set_data(xData,yData,size);
}

/*!
  \brief Set data by copying x- and y-values from specified memory blocks
  This function makes a 'deep copy' of the data.

  \sa QwData::set_data.
  @todo handle null pointers correctly
*/
void ErrorCurve::set_data(const double *xData, const double *yData,
			  const double *xErr,  const double *yErr,  int size)
{
    if (xErr) vector_from_c(dx_,xErr,size); else dx_.clear();
    if (yErr) vector_from_c(dy_,yErr,size); else dy_.clear();
    Curve::set_data(xData,yData,size);
}

/*!
  \brief Initialize data with x- and y-arrays (explicitly shared)

  @todo handle unevenly sized arrays correctly
*/
void ErrorCurve::set_data(const std::vector<double> &xData,
			 const std::vector<double> &yData,
			 const std::vector<double> &xErr,
			 const std::vector<double> &yErr)
{
    dx_= xErr;
    dy_= yErr;
    Curve::set_data(xData,yData);
}

/*!
  Initialize data with an array of points (explicitly shared).

  \param data Data

  @todo test if this method really works
*/
void ErrorCurve::set_data(const Glib::ArrayHandle<DoublePoint> &data)
{
    dx_.clear();
    dy_.clear();
    Curve::set_data(data);
}


/*!
  \brief Draw error bars
*/
void ErrorCurve::draw_errors_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
			    const DoubleIntMap &xMap,
			    const DoubleIntMap &yMap,
			    int from, int to)
{
  if (!have_dx_() && !have_dy_()) return;
  if (!symbol()->size()) return;
  for (int i = from; i <= to; i++) {
    if (have_dx_()) {
      int xl = xMap.transform(x(i)-dx(i));
      int y0 = yMap.transform(y(i));
      int xu = xMap.transform(x(i)+dx(i));
      draw_x_error_(cr, painter, xl,y0,xu,y0);
    }
    if (have_dy_()) {
      int yl = yMap.transform(y(i)-dy(i));
      int x0 = xMap.transform(x(i));
      int yu = yMap.transform(y(i)+dy(i));
      draw_y_error_(cr, painter, x0,yl,x0,yu);
    }
  }
}

void ErrorCurve::draw_x_error_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
			    int x1, int y1, int x2, int y2)
{

  int w = (symbol()->size_aux() + 1) / 2;

    error_paint()->set_cr_to_pen(cr);

  //  cr->set_source_rgb(0.0, 0.0, 1.0);
 //   cr->paint();

    cr->move_to(x1,y1);
    cr->line_to(x2,y2);
    cr->stroke();

    cr->move_to(x1,y1-w);
    cr->line_to(x1,y1+w);
    cr->stroke();

    cr->move_to(x2,y2-w);
    cr->line_to(x2,y2+w);
    cr->stroke();

//  painter->draw_line(error_paint()->pen(painter),x1,y1,x2,y2);
//  painter->draw_line(error_paint()->pen(painter),x1,y1-w,x1,y1+w);
//  painter->draw_line(error_paint()->pen(painter),x2,y2-w,x2,y2+w);
}

void ErrorCurve::draw_y_error_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
			    int x1, int y1, int x2, int y2)
{
  int w = (symbol()->size() + 1) / 2;

    error_paint()->set_cr_to_pen(cr);

    cr->move_to(x1,y1);
    cr->line_to(x2,y2);
    cr->stroke();

    cr->move_to(x1-w,y1);
    cr->line_to(x1+w,y1);
    cr->stroke();

    cr->move_to(x2-w,y2);
    cr->line_to(x2+w,y2);
    cr->stroke();

}


/*!
  \brief Draw an intervall of the curve
  \param painter Painter
  \param xMap maps x-values into pixel coordinates.
  \param yMap maps y-values into pixel coordinates.
  \param from index of the first point to be painted
  \param to index of the last point to be painted. If to < 0 the
         curve will be painted to its last point.

  \sa Curve::draw,
      ErrorCurve::draw_x_error_,
      ErrorCurve::draw_x_error_,
      ErrorCurve::draw_errors_
*/
void ErrorCurve::draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
		      const DoubleIntMap &xMap,
		      const DoubleIntMap &yMap,
		      int from, int to)
{
    if ( data_size() <= 0 )
        return;

    if (to < 0)
        to = data_size() - 1;

    if ( verify_range(from, to) > 0 ) {
	draw_errors_(cr, painter,xMap,yMap,from,to);
    }
    Curve::draw(cr, painter,xMap,yMap,from,to);
}


