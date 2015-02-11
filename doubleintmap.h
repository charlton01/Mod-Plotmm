/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#ifndef PLOTMM_DBLINTMAP_H
#define PLOTMM_DBLINTMAP_H

#include "supplemental.h"

namespace PlotMM {

    /*! @brief Map a double interval into an integer interval
     *
     *  The DoubleIntMap class maps an interval of type double into an
     *  interval of type int. It consists of two intervals D = [d1,
     *  d2] (double) and I = [i1, i2] (int), which are specified with
     *  the DoubleIntMap::set_dbl_range and DoubleIntMap::set_int_range
     *  members. The point d1 is mapped to the point i1, and d2 is
     *  mapped to i2. Any point inside or outside D can be mapped to a
     *  point inside or outside I using DoubleIntMap::transform or
     *  DoubleIntMap::lim_transform or vice versa using
     *  Plot::inv_transform. D can be scaled linearly or
     *  logarithmically, as specified with DoubleIntMap::set_dbl_range.
     *
     *  <b>Usage</b>
     *  \verbatim
     *  #include <plotmm/doubleintmap.h>
     *
     *  DoubleIntMap map;
     *  int ival;
     *  double dval;
     *
     *  // Assign an interval of type double with linear mapping
     *  map.set_dbl_range(0.0, 3.1415);
     *
     *  // Assign an integer interval
     *  map.set_int_range(0,100);
     *
     *  // obtain integer value corresponding to 1.0
     *  ival = map.transform(1.0);
     *
     *  // obtain double value corresponding to 77
     *  dval = map.inv_transform(77);
     *  \endverbatim
     */

    class DoubleIntMap
    {
    public:
	DoubleIntMap();
	DoubleIntMap(int i1, int i2, double d1, double d2, bool lg = false);
	virtual ~DoubleIntMap();

	bool contains(double x) const;
	bool contains(int x) const;

	void set_int_range(int i1, int i2);
	void set_dbl_range(double d1, double d2, bool lg = false);

	int transform(double x) const;
	double inv_transform(int i) const;

	int lim_transform(double x) const;
	double x_transform(double x) const;

	inline double d1() const;
	inline double d2() const;
	inline int i1() const;
	inline int i2() const;
	inline bool logarithmic() const;

	static const double LogMin;
	static const double LogMax;

    private:
	void newFactor();

	double d_x1, d_x2;  // double interval boundaries
	int d_y1, d_y2;     // integer interval boundaries
	double d_cnv;       // conversion factor
	bool d_log;     // logarithmic scale?
    };

    /*!
     *  \return the first border of the double interval
     */
    inline double DoubleIntMap::d1() const
    {
	return d_x1;
    }

    /*!
     *  \return the second border of the double interval
     */
    inline double DoubleIntMap::d2() const
    {
	return d_x2;
    }

    /*!
     *  \return the second border of the integer interval
     */
    inline int DoubleIntMap::i1() const
    {
	return d_y1;
    }

    /*!
     *  \return the second border of the integer interval
     */
    inline int DoubleIntMap::i2() const
    {
	return d_y2;
    }

    /*!
     *  \return TRUE if the double interval is scaled logarithmically
     */
    inline bool DoubleIntMap::logarithmic() const
    {
	return d_log;
    }

    /*! Transform a point in double interval into an point in the
     *  integer interval
     *
     *  \param x value
     *  \return
     *  <dl>
     *  <dt>linear mapping:
     *      <dd>rint(i1 + (i2 - i1) / (d2 - d1) * (x - d1))
     *  <dt>logarithmic mapping:
     *      <dd>rint(i1 + (i2 - i1) / log(d2 / d1) * log(x / d1))
     *  </dl>
     *  \warning The specified point is allowed to lie outside the
     *  intervals. If you want to limit the returned value, use
     *  DoubleIntMap::lim_transform.
     */
    inline int DoubleIntMap::transform(double x) const
    {
	if (d_log)
	    return d_y1 + ROUND((log(x) - d_x1) * d_cnv);
	else
	    return d_y1 + ROUND((x - d_x1) * d_cnv);
    }

}

#endif
