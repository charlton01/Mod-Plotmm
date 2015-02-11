/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#ifndef PLOTMM_MATH_H
#define PLOTMM_MATH_H

#include <vector>
#include <math.h>

namespace PlotMM {

#ifndef LOG10_2
#define LOG10_2     0.30102999566398119802  /* log10(2) */
#endif

#ifndef LOG10_3
#define LOG10_3     0.47712125471966243540  /* log10(3) */
#endif

#ifndef LOG10_5
#define LOG10_5     0.69897000433601885749  /* log10(5) */
#endif

#ifndef M_2PI
#define M_2PI       6.28318530717958623200  /* 2 pi */
#endif

#ifndef LOG_MIN
#define LOG_MIN 1.0e-100
#endif

#ifndef LOG_MAX
#define LOG_MAX 1.0e100
#endif

#ifndef M_E
#define M_E            2.7182818284590452354   /* e */
#endif

#ifndef M_LOG2E
#define M_LOG2E 1.4426950408889634074   /* log_2 e */
#endif

#ifndef M_LOG2E
#define M_LOG10E    0.43429448190325182765  /* log_10 e */
#endif

#ifndef M_LN2
#define M_LN2       0.69314718055994530942  /* log_e 2 */
#endif

#ifndef M_LN10
#define M_LN10         2.30258509299404568402  /* log_e 10 */
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846  /* pi */
#endif

#ifndef M_PI_2
#define M_PI_2      1.57079632679489661923  /* pi/2 */
#endif

#ifndef M_PI_4
#define M_PI_4      0.78539816339744830962  /* pi/4 */
#endif

#ifndef M_1_PI
#define M_1_PI      0.31830988618379067154  /* 1/pi */
#endif

#ifndef M_2_PI
#define M_2_PI      0.63661977236758134308  /* 2/pi */
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI  1.12837916709551257390  /* 2/sqrt(pi) */
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880  /* sqrt(2) */
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2   0.70710678118654752440  /* 1/sqrt(2) */
#endif

    double ceil_125(double x);
    double floor_125(double x);
    double array_min (double *array, int size);
    double array_max( double *array, int size);
    void twist_array(double *array, int size);
    void twist_array(std::vector<double> &);
    int check_mono(double *array, int size);
    void lin_space(double *array, int size, double xmin, double xmax);
    void log_space(double *array, int size, double xmin, double xmax);
    void lin_space(std::vector<double>&,int size,double xmin,double xmax);
    void log_space(std::vector<double>&,int size,double xmin,double xmax);
    void vector_from_c(std::vector<double> &array,const double *c, int size);

#ifndef MAX
#define MAX(a,b) ((b) < (a) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#endif

    inline int ROUND( double d )
    {
	return d >= 0.0 ? 
	    int(d + 0.5) : 
	    int( d - ((int)d-1) + 0.5 ) + ((int)d-1);
    }



    //! Return the sign 
    template <class T>
    inline int SIGN(const T& x)
    {
	if (x > T(0))
	    return 1;
	else if (x < T(0))
	    return (-1);
	else
	    return 0;
    }            

    /*! Copy an array into another
     *
     *  \param dest Destination
     *  \param src Source
     *  \param n Number of elements  
     */
    template <class T>
    void copy_array(T *dest, T *src, int n) 
    {
	int i;
	for (i=0; i<n;i++ )
	    dest[i] = src[i];
    }

    /*! Sort two values in ascending order
     *
     *  \param x1 First input value
     *  \param x2 Second input value
     *  \param xmax Greater value
     *  \param xmin Smaller value
     */
    template <class T>
    void sort_values(const T& x1, const T& x2, T& xmin, T& xmax)
    {
	T buffer;
    
	if (x2 < x1)
	{
	    buffer = x1;
	    xmin = x2;
	    xmax = buffer;
	}
	else
	{
	    xmin = x1;
	    xmax = x2;
	}
    }

    //! Sort two values in ascending order 
    template <class T>
    void sort_values(T& x1, T& x2)
    {
	T buffer;
    
	if (x2 < x1)
	{
	    buffer = x1;
	    x1 = x2;
	    x2 = buffer;
	}
    }

    /*! Limit a value to fit into a specified interval
     *
     *  \param x Input value
     *  \param x1 First interval boundary
     *  \param x2 Second interval boundary  
     */
    template <class T>
    T value_limes(const T& x, const T& x1, const T& x2)
    {
	T rv;
	T xmin, xmax;
    
	xmin = MIN(x1, x2);
	xmax = MAX(x1, x2);

	if ( x < xmin )
	    rv = xmin;
	else if ( x > xmax )
	    rv = xmax;
	else
	    rv = x;

	return rv;
    }
    /*
      inline Gdk::Point polar_to_position(const Gdk::Point &center,
      double radius, double angle)
      {
      const double x = center.get_x() + radius * cos(angle);
      const double y = center.get_y() - radius * sin(angle);

      return Gdk::Point(ROUND(x), ROUND(y));
      }

      inline Gdk::Point degree_to_position(const Gdk::Point &center,
      double radius, double angle)
      {
      return polar_to_position(center, radius, angle / 180.0 * M_PI);
      }
    */
} // namespace PlotMM

#endif
