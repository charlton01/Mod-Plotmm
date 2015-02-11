/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#ifndef PLOTMM_SCALEDIV_H
#define PLOTMM_SCALEDIV_H

#include <vector>

namespace PlotMM {

    /*!
     *  \brief A class for building scale divisions
     *
     *  The ScaleDiv class can build
     *  linear and logarithmic scale divisions for specified
     *  intervals. It uses an adjustable algorithm to
     *  generate the major and minor step widths automatically.
     *  A scale division has a minimum value, a maximum value,
     *  a vector of major marks, and a vector of minor marks.
     *
     *  \warning ScaleDiv uses implicit sharing for the mark vectors.
     *  \par Example
     *  To build a logarithmic scale division from 0.01 to 1000
     *  and print out the major and minor marks.
     *  \verbatim
#include "scalediv.h"
#include <iostream.h>

main()
{
  int i,k;
  PlotMM::ScaleDiv sd;

  sd.rebuild(0.01, 1000, 10, 10, TRUE, 0.0);

  k=0;
  for (i=0;i<sd.maj_count();i++)
  {
    while(k < sd.min_count())
    {
      if (sd.min_mark(k) < sd.maj_mark(i))
      {
        cout << " - " << sd.min_mark(i) << "\n";
    k++;
      }
      else
        break;
    }
    cout << "-- " << sd.maj_mark(i) << "\n";
  }
  while(k < sd.min_count())
  {
    cout << " - " << sd.min_mark(i) << "\n";
    k++;
  }
}\endverbatim
    */

    class ScaleDiv
    {
    public:
	ScaleDiv ();

	int operator== (const ScaleDiv &s) const;
	int operator!= (const ScaleDiv &s) const;

	/*!
	  \return left bound
	  \sa ScaleDiv::rebuild
	*/
	double low_bound() const { return d_lBound; }
	/*!
	  \return right bound
	  \sa ScaleDiv::rebuild
	*/
	double high_bound() const { return d_hBound; }
	/*!
	  \return minor mark count
	  \sa ScaleDiv::rebuild
	*/
	unsigned int min_count() const { return minMarks_.size(); }
	/*!
	  \return major mark count
	  \sa ScaleDiv::rebuild
	*/
	unsigned int maj_count() const { return majMarks_.size(); }

	/*!
	  \return TRUE id ths scale div is logarithmic
	  \sa ScaleDiv::rebuild
	*/
	bool log_scale() const { return d_log; }

	//! Return major mark at position i
	double maj_mark(int i) const { return majMarks_[i]; }
	//! Return minor mark at position i
	double min_mark(int i) const { return minMarks_[i]; }

	/*!
	  \return major step size
	  \sa ScaleDiv::rebuild
	*/
	double maj_step() const { return d_majStep; }
	void reset();

	bool rebuild(double lBound, double hBound, int maxMaj, int maxMin,
		     bool log, double step = 0.0, bool ascend = true);

    private:
	bool build_lin_div_(int maxMajMark, int maxMinMark, double step = 0.0);
	bool build_log_div_(int maxMajMark, int maxMinMark, double step = 0.0);

	double d_lBound;
	double d_hBound;
	double d_majStep;
	bool d_log;

	std::vector<double> majMarks_;
	std::vector<double> minMarks_;
    };

}

#endif
