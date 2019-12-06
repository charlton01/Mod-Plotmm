/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#ifndef PLOTMM_CURVE_H
#define PLOTMM_CURVE_H

#include <vector>

#include "compat.h"
#if (GTKMM_MAJOR_VERSION == 2 && GTKMM_MINOR_VERSION > 4)
#include <glibmm/objectbase.h>
#else
//#include <sigc++/object.h>
#include <sigc++/sigc++.h>
#endif

#include <glibmm/arrayhandle.h>
#include <glibmm/ustring.h>

#include "symbol.h"
#include "doublerect.h"

namespace Gtk {
  //  class Drawable;
}

namespace PlotMM {

    class Paint;
    class DoubleIntMap;

    /*!
      Curve styles.
      \sa Curve::set_style
    */
    enum CurveStyleID
    {
	CURVE_NONE,
	CURVE_LINES,
	CURVE_STICKS,
	CURVE_L_STEPS,
	CURVE_C_STEPS,
	CURVE_R_STEPS,
	CURVE_DOTS,
	CurveStyleCnt
    };

    /*!
      Curve options.
      \sa Curve::set_options
    */
    enum CurveOptions
    {
	CURVE_AUTO = 0,
	CURVE_Y_FX = 1,
	CURVE_X_FY = 2,
	CURVE_INVERTED = 4
    };

    /*! @brief A class which draws curves
     *
     *  This class can be used to display data as a curve in the  x-y plane.
     *  It supports different display styles and symbols.
     *
     *  \par Usage
     *  <dl><dt>A. Assign curve properties</dt>
     *      <dd>When a curve is created, it is configured to draw black solid
     *          lines with CURVE_LINES and no symbols. You can change this by
     *          calling Curve::set_pen(), Curve::set_style() and
     *          Curve::set_symbol().</dd>
     *      <dt>B. Assign or change data.</dt>
     *      <dd>use one of the provided Curve::set_data() functions. The
     *          curve's x and y data are assigned by copying from different
     *          data structures.</dd>
     *      <dt>C. Draw</dt>
     *      <dd>Curve::draw() maps the data into pixel coordinates and paints
     *          them.  </dd>
     *  </dl>
     *
     *  \par Example:
     *  see examples/curvdemo
     *
     *  \sa Symbol, DoubleIntMap
     */
    class Curve : public PlotMM::ObjectBase
    {
    public:
	Curve(const Glib::ustring &title = "");
	Curve(const Curve &c);
	virtual ~Curve();

	//! Enable or disable this curve for drawing
	virtual void set_enabled(bool b) { enabled_= b; }
	//! Query if this curve is enabled for drawing
	virtual bool enabled() { return enabled_; }

	const Curve& operator= (const Curve &c);

	// void set_rawData(const double *x, const double *y, int size);
	virtual void set_data(const double *xData, const double *yData,
			      int size);
	virtual void set_data(const std::vector<double> &xData,
			      const std::vector<double> &yData);
	virtual void set_data(const Glib::ArrayHandle<DoublePoint> &data);

	virtual int data_size() const;
	inline double x(int i) const;
	inline double y(int i) const;

	virtual DoubleRect bounding_rect() const;

	inline double min_x_value() const { return bounding_rect().get_x1(); }
	inline double max_x_value() const { return bounding_rect().get_x2(); }
	inline double min_y_value() const { return bounding_rect().get_y1(); }
	inline double max_y_value() const { return bounding_rect().get_y2(); }

	virtual void set_options(CurveOptions t);
	virtual CurveOptions options() const;

	virtual void set_title(const Glib::ustring &title);
	virtual const Glib::ustring &title() const;

	virtual void set_baseline(double ref);
	virtual double baseline() const;

	virtual void set_curve_style(CurveStyleID style,
				     CurveOptions options = CURVE_AUTO);
	virtual CurveStyleID curve_style() const;
	virtual Glib::RefPtr<Paint> paint() const;

	virtual void set_symbol(const Glib::RefPtr<Symbol> &s);
        virtual Glib::RefPtr<Symbol> symbol() const;
    //first parameter below is changed from const Glib::RefPtr<Gdk::Drawable> &p
	virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
			  const DoubleIntMap &xMap, const DoubleIntMap &yMap,
			  int from = 0, int to = -1);

	//! Signals that this curve has changed in some way or the other
	sigc::signal0<void> signal_curve_changed;

    protected:
	virtual void init(const Glib::ustring &title);
	virtual void copy(const Curve &c);

	virtual void draw_curve_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				 int style,
				 const DoubleIntMap &xMap,
				 const DoubleIntMap &yMap,
				 int from, int to);

	virtual void draw_symbols_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				   const Glib::RefPtr<Symbol> &,
				   const DoubleIntMap &xMap,
				   const DoubleIntMap &yMap,
				   int from, int to);

	virtual void draw_lines_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				 const DoubleIntMap &xMap,
				 const DoubleIntMap &yMap,
				 int from, int to);
	virtual void draw_sticks_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				  const DoubleIntMap &xMap,
				  const DoubleIntMap &yMap,
				  int from, int to);
	virtual void draw_dots_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				const DoubleIntMap &xMap,
				const DoubleIntMap &yMap,
				int from, int to);
	virtual void draw_lsteps_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				  const DoubleIntMap &xMap,
				  const DoubleIntMap &yMap,
				  int from, int to);
	virtual void draw_csteps_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				  const DoubleIntMap &xMap,
				  const DoubleIntMap &yMap,
				  int from, int to);
	virtual void draw_rsteps_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				  const DoubleIntMap &xMap,
				  const DoubleIntMap &yMap,
				  int from, int to);
	virtual void close_polyline_(const DoubleIntMap &,
				     const DoubleIntMap &,
				     std::vector<Gdk::Point> &) const;

	virtual void curve_changed();

	virtual int verify_range(int &i1, int &i2);

    private:
	bool enabled_;
	std::vector<double> x_;
	std::vector<double> y_;

	CurveStyleID cStyle_;
	double baseline_;
	bool fill_;

	Glib::RefPtr<Symbol> symbol_;

	Glib::RefPtr<Paint> paint_;
	Glib::ustring title_;

	CurveOptions options_;
    };

    /*!
      \param i index
      \return x-value at position i
    */
    inline double Curve::x(int i) const
    {
	return x_[i];
    }

    /*!
      \param i index
      \return y-value at position i
    */
    inline double Curve::y(int i) const
    {
	return y_[i];
    }

}

#endif
