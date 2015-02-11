/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#include <glibmm/refptr.h>

//#include <gdkmm/drawable.h>

#include "doubleintmap.h"
#include  "curve.h"
#include "paint.h"
#include "supplemental.h"

using namespace PlotMM;

//! Initialize data members
void Curve::init(const Glib::ustring &title)
{
    enabled_= true;
    baseline_ = 0.0;
    options_ = CURVE_AUTO;
    title_ = title;
    cStyle_= CURVE_LINES;
    paint_= Glib::RefPtr<Paint> (new Paint);
    symbol_= Glib::RefPtr<Symbol> (new Symbol);
}

//! Copy the contents of a curve into another curve
void Curve::copy(const Curve &c)
{
    enabled_= c.enabled_;
    baseline_ = c.baseline_;
    symbol_ = c.symbol_;
    title_ = c.title_;
    paint_ = c.paint_;
    cStyle_ = c.cStyle_;

    options_ = c.options_;
    x_= c.x_;
    y_= c.y_;
}

//! Destructor
Curve::~Curve()
{
}

/*!
  \brief Ctor
  \param title title of the curve
*/
Curve::Curve(const Glib::ustring &title)
{
    init(title);
}

//! Copy Constructor
Curve::Curve(const Curve &c)
{
    init(c.title_);
    copy(c);
}

//! Copy Assignment
const Curve& Curve::operator=(const Curve &c)
{
    if (this != &c)
    {
        copy(c);
        curve_changed();
    }

    return *this;
}

/*!
  \brief Set the curve's drawing style

  Valid styles are:
  <dl>
  <dt>CURVE_NONE</dt>
  <dd>Don't draw a curve. Note: This doesn't affect the symbol. </dd>
  <dt>CURVE_LINES</dt>
  <dd>Connect the points with straight lines.</dd>
  <dt>CURVE_STICKS</dt>
  <dd>Draw vertical sticks from a baseline which is defined by set_baseline().</dd>
  <dt>Curve::Steps</dt>
  <dd>Connect the points with a step function. The step function
      is drawn from the left to the right or vice versa,
      depending on the 'CURVE_INVERTED' option.</dd>
  <dt>Curves::CURVE_DOTS</dt>
  <dd>Draw dots at the locations of the data points. Note:
      This is different from a dotted line (see set_pen()).</dd>
  <dt>Curve::UserCurve ...</dt>
  <dd>Styles >= Curve::UserCurve are reserved for derived
      classes of Curve that overload Curve::draw() with
      additional application specific curve types.</dd>
  </dl>
  \sa Curve::paint()
*/
void Curve::set_curve_style(CurveStyleID cStyle, CurveOptions options)
{

    options_ = options;
    cStyle_ = cStyle;

    curve_changed();
}

/*!
    \brief Return the current style
    \sa Curve::set_style
*/
CurveStyleID Curve::curve_style() const
{
    return cStyle_;
}

/*! Return a reference to the Paint the curve is drawn with
 */
Glib::RefPtr<Paint> Curve::paint() const
{
    return paint_;
}

/*!
  \brief Assign a symbol
  \param s symbol
  \sa Symbol
*/
void Curve::set_symbol(const Glib::RefPtr<Symbol> &s )
{
    symbol_ = s;
    curve_changed();
}

/*!
    \brief Return the current symbol
    \sa Curve::set_symbol
*/
Glib::RefPtr<Symbol> Curve::symbol() const
{
    return symbol_;
}

/*!
  \brief Set data by copying x- and y-values from specified memory blocks
  Contrary to \b Plot::set_curveRawData, this function makes a 'deep copy' of
  the data.

  \param xData pointer to x values
  \param yData pointer to y values
  \param size size of xData and yData

  \sa QwData::set_data.
  @todo handle null pointers correctly
*/
void Curve::set_data(const double *xData, const double *yData, int size)
{
    vector_from_c(x_,xData,size);
    vector_from_c(y_,yData,size);
    curve_changed();
}

/*!
  \brief Initialize data with x- and y-arrays (explicitly shared)

  \param xData x data
  \param yData y data

  @todo handle unevenly sized arrays correctly
*/
void Curve::set_data(const std::vector<double> &xData,
			const std::vector<double> &yData)
{

    x_= xData;
    y_= yData;
    curve_changed();
}

/*!
  Initialize data with an array of points (explicitly shared).

  \param data Data

  @todo test if this method really works
*/
void Curve::set_data(const Glib::ArrayHandle<DoublePoint> &data)
{
    x_.clear();
    y_.clear();
    Glib::ArrayHandle<DoublePoint>::const_iterator daPnt(data.begin());
    for (daPnt= data.begin(); daPnt!=data.end(); ++daPnt) {
	x_.push_back((*daPnt).get_x());
	y_.push_back((*daPnt).get_y());
    }
    curve_changed();
}

/*!
  \brief Assign a title to a curve
  \param title new title
*/
void Curve::set_title(const Glib::ustring &title)
{
    title_ = title;
    curve_changed();
}

/*!
    \brief Return the title.
    \sa Curve::set_title
*/
const Glib::ustring &Curve::title() const
{
    return title_;
}

/*!
  Returns the bounding rectangle of the curve data. If there is
  no bounding rect, like for empty data the rectangle is invalid:
  DoubleRect.is_valid() == FALSE
*/

DoubleRect Curve::bounding_rect() const
{
    if ( (x_.size()==0) || (x_.size()!=y_.size()) )
        return DoubleRect(1.0, -1.0, 1.0, -1.0); // invalid

    const size_t sz = x_.size();

    double minX, maxX, minY, maxY;
    std::vector<double>::const_iterator xIt = x_.begin();
    std::vector<double>::const_iterator yIt = y_.begin();
    std::vector<double>::const_iterator end = x_.begin() + sz;
    minX = maxX = *xIt++;
    minY = maxY = *yIt++;

    while ( xIt < end )
    {
        const double xv = *xIt++;
        if ( xv < minX )
            minX = xv;
        if ( xv > maxX )
            maxX = xv;

        const double yv = *yIt++;
        if ( yv < minY )
            minY = yv;
        if ( yv > maxY )
            maxY = yv;
    }
    return DoubleRect(minX, maxX, minY, maxY);
}

/*!
  \brief Checks if a range of indices is valid and corrects it if necessary
  \param i1 Index 1
  \param i2 Index 2
*/
int Curve::verify_range(int &i1, int &i2)
{
    int size = data_size();

    if (size < 1) return 0;

    i1 = value_limes(i1, 0, size-1);
    i2 = value_limes(i2, 0, size-1);
    sort_values(i1, i2, i1, i2);

    return (i2 - i1 + 1);
}

/*!
  \brief Draw an intervall of the curve
  \param painter Painter
  \param xMap maps x-values into pixel coordinates.
  \param yMap maps y-values into pixel coordinates.
  \param from index of the first point to be painted
  \param to index of the last point to be painted. If to < 0 the
         curve will be painted to its last point.

  \sa Curve::draw_curve_, Curve::draw_dots_,
      Curve::draw_lines_, Curve::draw_symbols_,
      Curve::draw_lsteps_, Curve::draw_csteps_,
      Curve::draw_rsteps_, Curve::draw_sticks_
*/
void Curve::draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{
    if ( data_size() <= 0 )
        return;

    int winx, winy, winw, winh;
    painter->get_geometry(winx, winy, winw, winh);

    if (to < 0)
        to = data_size() - 1;
    if ( verify_range(from, to) > 0 ) {
        draw_curve_(cr, painter, cStyle_, xMap, yMap, from, to);

        if (symbol_->style() != SYMBOL_NONE) {
            draw_symbols_(cr, painter, symbol_, xMap, yMap, from, to);
        }
    }
}

/*!
  \brief Draw the line part (without symbols) of a curve interval.
  \param painter Painter
  \param style curve style, see CurveStyleID
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw
*/

void Curve::draw_curve_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter, int style,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{	
    switch (style)
    {
        case CURVE_NONE:
            break;
        case CURVE_LINES:
            draw_lines_(cr, painter, xMap, yMap, from, to);
            break;
        case CURVE_STICKS:
            draw_sticks_(cr, painter, xMap, yMap, from, to);
            break;
        case CURVE_L_STEPS:
            draw_lsteps_(cr, painter, xMap, yMap, from, to);
            break;
        case CURVE_C_STEPS:
            draw_csteps_(cr, painter, xMap, yMap, from, to);
            break;
        case CURVE_R_STEPS:
            draw_rsteps_(cr, painter, xMap, yMap, from, to);
            break;
        case CURVE_DOTS:
            draw_dots_(cr, painter, xMap, yMap, from, to);
            break;
        default:
            break;
    }
}

/*
  \brief Draw lines
  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw
*/

void Curve::draw_lines_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{
    int xi, yi;
    //std::vector<Gdk::Point> polyline;

    paint()->set_cr_to_pen(cr);
    cr->set_line_width(1.0);

    xi = xMap.transform(x(from));
    yi = xMap.transform(y(from));

    cr->move_to(xMap.transform(x(from)),yMap.transform(y(from)));
    for (int i = from; i <= to; i++)
    {

// transform converts a double to int or a log conversion to int.
        xi = xMap.transform(x(i));
        yi = yMap.transform(y(i));
        //polyline.push_back(Gdk::Point(xi, yi));
        cr->line_to(xi,yi);

    }

    cr->stroke();

    //painter->draw_lines(paint()->pen(painter), polyline);


//  Not sure why you would ever want the following
    if ( paint()->filled() )

    {
        paint()->set_cr_to_brush(cr);
        cr->move_to(xMap.transform(x(from)),yMap.transform(y(from)));

        for (int i = from+1; i <= to; i++)
        {
    // transform converts a double to int or a log conversion to int.
            int xi = xMap.transform(x(i));
            int yi = yMap.transform(y(i));

            //polyline.push_back(Gdk::Point(xi, yi));
            cr->line_to(xi,yi);
        }
        // what is desired here is to fill everything below the curve so...
        cr->line_to(xMap.transform(x(to)),yMap.transform(y(0)));
        cr->line_to(xMap.transform(y(from)),yMap.transform(y(0)));
        cr->close_path();
        cr->fill();


    }

}

/*!
  \brief Draw sticks
  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw
*/
void Curve::draw_sticks_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{
    paint()->set_cr_to_pen(cr);
    cr->set_line_width(1.0);

    int x0 = xMap.transform(baseline_);
    int y0 = yMap.transform(baseline_);

    for (int i = from; i <= to; i++)
    {
        int xi = xMap.transform(x(i));
        int yi = yMap.transform(y(i));

        if (options_ & CURVE_X_FY)
        {
            cr->move_to(x0, yi);
            cr->line_to(xi, yi);
            cr->stroke();

        }
        else
         {
            cr->move_to(xi, y0);
            cr->line_to(xi, yi);
            cr->stroke();

         }
    }
}

/*!
  \brief Draw dots
  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw
*/
void Curve::draw_dots_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{

    // const bool doFill = painter->brush().paint() != Qt::NoBrush;
//    bool doFill= paint()->filled();

//    cr->save();
    paint()->set_cr_to_pen(cr);
    cr->set_line_width(8);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    for (int i = from; i <= to; i++)
    {
        int xi = xMap.transform(x(i));
        int yi = yMap.transform(y(i));


        cr->move_to(xi, yi);
        cr->line_to(xi, yi);
        //painter->draw_point(paint()->pen(painter), xi, yi);

    }

    cr->stroke();
//    cr->restore();


}

/*!
  \brief Draw step function
  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw
*/
void Curve::draw_lsteps_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{

    std::vector<Gdk::Point> polyline;

    cr->set_line_width(1.0);
    paint()->set_cr_to_pen(cr);
    bool inverted = options_ & CURVE_X_FY;
    if ( options_ & CURVE_INVERTED )
        inverted = !inverted;

    int i,ip;
    int xi = xMap.transform(x(from));
    int yi = yMap.transform(y(from));

    polyline.push_back(Gdk::Point(xi, yi));
    cr->move_to(xMap.transform(x(from)),yMap.transform(y(from)));

    for (i = from+1, ip = 1; i <= to; i++, ip += 2)
    {
        xi = xMap.transform(x(i));
        yi = yMap.transform(y(i));
        if (inverted)
        {
            polyline.push_back(Gdk::Point(xi, polyline[ip-1].get_y()));
            cr->line_to(xi,polyline[ip-1].get_y());
        }
        else
        {
            polyline.push_back(Gdk::Point(polyline[ip-1].get_x(), yi));
            cr->line_to(polyline[ip-1].get_x(),yi);
        }

        polyline.push_back(Gdk::Point(xi, yi));
        cr->line_to(xi,yi);

    }
    cr->stroke();
    if ( paint()->filled() )
    {
        paint()->set_cr_to_brush(cr);
        // what is desired here is to fill everything below the curve so...
        cr->line_to(xMap.transform(x(to)),yMap.transform(y(0)));
        cr->line_to(xMap.transform(y(from)),yMap.transform(y(0)));
        cr->close_path();
        cr->fill();
    }

}

/*!
  \brief Draw step function
  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw, Curve::draw_curve_, Curve::draw_dots_,
      Curve::draw_lines_, Curve::draw_sticks_
*/
void Curve::draw_csteps_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{
        std::vector<Gdk::Point> polyline;

   paint()->set_cr_to_pen(cr);

    cr->set_line_width(1.0);

    bool inverted = options_ & CURVE_X_FY;
    if ( options_ & CURVE_INVERTED )
        inverted = !inverted;

    int i,ip;

    int xi = xMap.transform(x(from));
    int yi = yMap.transform(y(from));
    polyline.push_back(Gdk::Point(xi, yi));
    cr->move_to(xMap.transform(x(from)),yMap.transform(y(from)));

    for (i = from+1, ip = 0; i <= to; i++, ip += 2)
    {
	int xn,yn;
	if (inverted)
        {
            xn= xMap.transform(x(i));
            yn= yMap.transform((y(i)+y(i-1))*.5);
            polyline.push_back(Gdk::Point(xi, yn));
            cr->line_to(xi,yn);
        }
	else
        {
	    xn= xMap.transform((x(i)+x(i-1))*.5);
	    yn= yMap.transform(y(i));
	    polyline.push_back(Gdk::Point(xn, yi));
	    cr->line_to(xn,yi);
        }
	xi=xn; yi=yn;
        polyline.push_back(Gdk::Point(xi, yi));
        cr->line_to(xi,yi);
    }
    xi = xMap.transform(x(to));
    yi = yMap.transform(y(to));
    polyline.push_back(Gdk::Point(xi, yi));
    cr->line_to(xi,yi);
    cr->stroke();

    if ( paint()->filled() )
    {
        paint()->set_cr_to_brush(cr);

        // what is desired here is to fill everything below the curve so...
        cr->line_to(xMap.transform(x(to)),yMap.transform(y(0)));
        cr->line_to(xMap.transform(y(from)),yMap.transform(y(0)));
        cr->close_path();
        cr->fill();

    }

}

/*!
  \brief Draw step function
  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
  \sa Curve::draw, Curve::draw_curve_, Curve::draw_dots_,
      Curve::draw_lines_, Curve::draw_sticks_
*/
void Curve::draw_rsteps_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
    const DoubleIntMap &xMap, const DoubleIntMap &yMap, int from, int to)
{
    std::vector<Gdk::Point> polyline;

    paint()->set_cr_to_pen(cr);

    cr->set_line_width(1.0);

    bool inverted = options_ & CURVE_X_FY;
    if ( options_ & CURVE_INVERTED )
        inverted = !inverted;

    int i,ip;

        int xi = xMap.transform(x(from));
        int yi = yMap.transform(y(from));

        cr->move_to(xi,yi);

    for (i = from, ip = 0; i <= to; i++, ip += 2)
    {
        int xi = xMap.transform(x(i));
        int yi = yMap.transform(y(i));

        if ( ip > 0 )
        {
            if (inverted)
            {
                polyline.push_back(Gdk::Point(polyline[ip-2].get_x(), yi));
                cr->line_to(polyline[ip-2].get_x(), yi);
            }
            else
             {
                polyline.push_back(Gdk::Point(xi, polyline[ip-2].get_y()));
                cr->line_to(xi, polyline[ip-2].get_y());
             }
        }

        polyline.push_back(Gdk::Point(xi, yi));
        cr->line_to(xi,yi);
    }
    cr->stroke();
//    if ( paint()->filled() )
    {
        paint()->set_cr_to_brush(cr);

        // what is desired here is to fill everything below the curve so...
        cr->line_to(xMap.transform(x(to)),yMap.transform(y(0)));
        cr->line_to(xMap.transform(y(from)),yMap.transform(y(0)));
        cr->close_path();
        cr->fill();
    }

}

/*!
  \brief Specify options for the drawing style

  The options can be used to modify the drawing style.
  Options can be or-combined.
  The following options are defined:<dl>
  <dt>CURVE_AUTO</dt>
  <dd>The default setting. This means that y is
      regarded as a function of x.</dd>
  <dt>CURVE_Y_FX</dt>
  <dd>Draws y as a function of x (the default). The
      baseline is interpreted as a horizontal line
      with y = baseline().</dd>
  <dt>CURVE_X_FY</dt>
  <dd>Draws x as a function of y. The baseline is
      interpreted as a vertical line with x = baseline().</dd>
  <dt>CURVE_INVERTED</dt>
  <dd>For Curve::Steps only. Draws a step function
      from the right to the left.</dd></dl>

  \param opt new options
  /sa Curve::options()
*/
void Curve::set_options(CurveOptions opt)
{
    options_ = opt;
    curve_changed();
}

/*!
    \brief Return the current style options
    \sa Curve::set_options
*/
CurveOptions Curve::options() const
{
    return options_;
}

/*!
  \brief Complete a polygon to be a closed polygon
         including the area between the original polygon
         and the baseline.
  \param xMap X map
  \param yMap Y map
  \param pa Polygon to be completed
*/
void Curve::close_polyline_(const DoubleIntMap &xMap,
			      const DoubleIntMap &yMap,
			      std::vector<Gdk::Point> &pa) const
{
    const int sz = pa.size();
    if ( sz < 2 )
        return;

    if ( options_ & CURVE_X_FY )
    {
        pa.push_back(Gdk::Point(xMap.transform(baseline_),pa[sz - 1].get_y()));
        pa.push_back(Gdk::Point(xMap.transform(baseline_),pa[0].get_y()));
    }
    else
    {
        pa.push_back(Gdk::Point(pa[sz-1].get_x(), yMap.transform(baseline_)));
        pa.push_back(Gdk::Point(pa[0].get_x(), yMap.transform(baseline_)));
    }
}

/*!
  \brief Draw symbols
  \param painter Painter
  \param symbol Curve symbol
  \param xMap x map
  \param yMap y map
  \param from index of the first point to be painted
  \param to index of the last point to be painted
*/
void Curve::draw_symbols_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
			const Glib::RefPtr<Symbol> &symbol,
			const DoubleIntMap &xMap,
			const DoubleIntMap &yMap, int from, int to)
{
//    painter->set_brush(symbol.brush());
//    painter->set_pen(symbol.pen());

    for (int i = from; i <= to; i++)
    {
        const int xi = xMap.transform(x(i));
        const int yi = yMap.transform(y(i));

        symbol->draw(cr, painter, xi, yi);
    }
}

/*!
  \brief Set the value of the baseline

  The baseline is needed for filling the curve with a brush or
  the CURVE_STICKS drawing style.
  The default value is 0.0. The interpretation
  of the baseline depends on the style options. With CURVE_Y_FX,
  the baseline is interpreted as a horizontal line at y = baseline(),
  with Curve::Yfy, it is interpreted as a vertical line at
  x = baseline().
  \param ref baseline
  \sa Curve::set_brush(), Curve::set_style(), Curve::set_options()
*/
void Curve::set_baseline(double ref)
{
    baseline_ = ref;
    curve_changed();
}

/*!
    \brief Return the value of the baseline
    \sa Curve::set_baseline
*/
double Curve::baseline() const
{
    return baseline_;
}

/*!
  Return the size of the data arrays
*/
int Curve::data_size() const
{
    return x_.size();
}

/*!
    \brief Notify a change of attributes.
    This virtual function is called when an attribute of the curve
    has changed. It can be redefined by derived classes.
    The default implementation does nothing.
*/

void Curve::curve_changed()
{
    signal_curve_changed();
}
