/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#ifndef PLOTMM_ERRORCURVE_H
#define PLOTMM_ERRORCURVE_H

#include "curve.h"

namespace PlotMM {

    class Paint;
    class DoubleIntMap;

    /*! @brief A class which draws curves with errors
     *
     *  This class can be used to display data as a curve in the  x-y plane.
     *  It supports different display styles and symbols.
     *
     *  \sa Curve, Symbol, DoubleIntMap
     */
    class ErrorCurve : public Curve
    {
    public:
	ErrorCurve(const Glib::ustring &title = "");
	ErrorCurve(const ErrorCurve &c);
	virtual ~ErrorCurve();

	const ErrorCurve& operator= (const ErrorCurve &c);

	// void set_rawData(const double *x, const double *y, int size);
	virtual void set_data(const double *xData, const double *yData,
			     int size);
	virtual void set_data(const double *xData, const double *yData,
			     const double *xErr,  const double *yErr,
			     int size);
	virtual void set_data(const std::vector<double> &xData,
			     const std::vector<double> &yData);
	virtual void set_data(const std::vector<double> &xData,
			     const std::vector<double> &yData,
			     const std::vector<double> &xErr,
			     const std::vector<double> &yErr);
	virtual void set_data(const Glib::ArrayHandle<DoublePoint> &data);

	inline double dx(int i) const;
	inline double dy(int i) const;

	virtual Glib::RefPtr<Paint> error_paint() const;

	virtual void draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> painter,
			  const DoubleIntMap &xMap, const DoubleIntMap &yMap,
			  int from = 0, int to = -1);

    protected:
	virtual void init(const Glib::ustring &title);
	virtual void copy(const Curve &c);
	virtual void copy(const ErrorCurve &c);

	virtual void draw_errors_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				const DoubleIntMap &xMap,
				const DoubleIntMap &yMap,
				int from = 0, int to = -1);

	virtual void draw_x_error_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				int x1, int y1, int x2, int y2);
	virtual void draw_y_error_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> p,
				int x1, int y1, int x2, int y2);

	bool have_dx_() const {
	    return data_size() && ((int)dx_.size()==(int)data_size());
	}
	bool have_dy_() const {
	    return data_size() && ((int)dy_.size()==(int)data_size());
	}
    private:
	Glib::RefPtr<Paint> epaint_;
	std::vector<double> dx_;
	std::vector<double> dy_;
    };

    /*!
      \param i index
      \return x-value at position i
    */
    inline double ErrorCurve::dx(int i) const
    {
	return dx_[i];
    }

    /*!
      \param i index
      \return y-value at position i
    */
    inline double ErrorCurve::dy(int i) const
    {
	return dy_[i];
    }

}

#endif
