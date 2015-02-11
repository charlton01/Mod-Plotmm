/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#include "supplemental.h"

using namespace PlotMM;

namespace PlotMM {
/*!
  \brief Find the smallest value in an array
  \param array Pointer to an array
  \param size Array size
*/
double array_min(double *array, int size)
{
    if (size <= 0)
       return 0.0;

    double rv = array[0];
    for (int i = 1; i < size; i++)
       rv = MIN(rv, array[i]);

    return rv;
}


/*!
  \brief Find the largest value in an array
  \param array Pointer to an array
  \param size Array size
*/
double array_max(double *array, int size)
{
    if (size <= 0)
       return 0.0;

    double rv = array[0];
    for (int i = 1; i < size; i++)
       rv = MAX(rv, array[i]);

    return rv;
}


/*!
  \brief Find the smallest value out of {1,2,5}*10^n with an integer number n
  which is greater than or equal to x

  \param x Input value
*/
double ceil_125( double x)
{
    if (x == 0.0)
        return 0.0;

    const double sign = (x > 0) ? 1.0 : -1.0;
    const double lx = log10(fabs(x));
    const double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr <=1.0)
       fr = 1.0;
    else if (fr <= 2.0)
       fr = 2.0;
    else if (fr <= 5.0)
       fr = 5.0;
    else
       fr = 10.0;

    return sign * fr * pow(10.0, p10);
}


/*!
  \brief Find the largest value out of {1,2,5}*10^n with an integer number n
  which is smaller than or equal to x

  \param x Input value
*/
double floor_125( double x)
{
    if (x == 0.0)
        return 0.0;

    double sign = (x > 0) ? 1.0 : -1.0;
    const double lx = log10(fabs(x));
    const double p10 = floor(lx);

    double fr = pow(10.0, lx - p10);
    if (fr >= 10.0)
       fr = 10.0;
    else if (fr >= 5.0)
       fr = 5.0;
    else if (fr >= 2.0)
       fr = 2.0;
    else
       fr = 1.0;

    return sign * fr * pow(10.0, p10);
}


/*!
  \brief  Checks if an array is a strictly monotonic sequence
  \param array Pointer to an array
  \param size Size of the array
  \return
  <dl>
  <dt>0<dd>sequence is not strictly monotonic
  <dt>1<dd>sequence is strictly monotonically increasing
  <dt>-1<dd>sequence is strictly monotonically decreasing
  </dl>
*/
int check_mono(double *array, int size)
{
    if (size < 2)
        return 0;

    int rv = SIGN(array[1] - array[0]);
    for (int i = 1; i < size - 1; i++)
    {
        if ( SIGN(array[i+1] - array[i]) != rv )
        {
            rv = 0;
            break;
        }
    }
    return rv;
}

/*!
  \brief Invert the order of array elements
  \param array Pointer to an array
  \param size Size of the array
*/
void twist_array(double *array, int size)
{
    const int s2 = size / 2;

    for (int i=0; i < s2; i++)
    {
        const int itmp = size - 1 - i;
        const double dtmp = array[i];
        array[i] = array[itmp];
        array[itmp] = dtmp;
    }
}

/*!
  \brief Invert the order of array elements
  \param array Pointer to an array
  \param size Size of the array
*/
void twist_array(std::vector<double> &array)
{
    int size= array.size();
    const int s2 = size/ 2;

    for (int i=0; i < s2; i++)
    {
        const int itmp = size - 1 - i;
        const double dtmp = array[i];
        array[i] = array[itmp];
        array[itmp] = dtmp;
    }
}

/*!
  \brief Create an array of equally spaced values
  \param array Where to put the values
  \param size Size of the array
  \param xmin Value associated with index 0
  \param xmax Value associated with index (size-1)
*/
void lin_space(double *array, int size, double xmin, double xmax)
{
    if (size <= 0)
        return;

    const int imax = size -1;

    array[0] = xmin;
    array[imax] = xmax;

    const double step = (xmax - xmin) / double(imax);
    const double tiny = 1e-6;

    for (int i = 1; i < imax; i++)
    {
       array[i] = xmin + double(i) * step;
       if (fabs(array[i]) < tiny*fabs(step))
          array[i] = step*floor(array[i]/step + tiny/2);
    }
}

/*!
  \brief Create an array of logarithmically equally spaced values
  \param array  Where to put the values
  \param size   Size of the array
  \param xmin Value associated with index 0
  \param xmax   Value associated with index (size-1)
*/
void log_space(double *array, int size, double xmin, double xmax)
{
    if ((xmin <= 0.0) || (xmax <= 0.0) || (size <= 0))
       return;

    const int imax = size -1;

    array[0] = xmin;
    array[imax] = xmax;

    const double lxmin = log(xmin);
    const double lxmax = log(xmax);
    const double lstep = (lxmax - lxmin) / double(imax);

    for (int i = 1; i < imax; i++)
       array[i] = exp(lxmin + double(i) * lstep);
}


/*!
  \brief Create an array of equally spaced values
  \param array Where to put the values
  \param size Size of the array
  \param xmin Value associated with index 0
  \param xmax Value associated with index (size-1)
*/
void lin_space(std::vector<double> &array_, int size,
			 double xmin, double xmax)
{
    if (size<1) return;
    double array[size];
    lin_space(array,size,xmin,xmax);
    vector_from_c(array_,array,size);
}

/*!
  \brief Create an array of logarithmically equally spaced values
  \param array  Where to put the values
  \param size   Size of the array
  \param xmin Value associated with index 0
  \param xmax   Value associated with index (size-1)
*/
void log_space(std::vector<double> &array_, int size,
			 double xmin, double xmax)
{
    if (size<1) return;
    double array[size];
    log_space(array,size,xmin,xmax);
    vector_from_c(array_,array,size);
}

/*!
  \brief Create an array of logarithmically equally spaced values
  \param array  Where to put the values
  \param size   Size of the array
  \param xmin Value associated with index 0
  \param xmax   Value associated with index (size-1)
*/
void vector_from_c(std::vector<double> &array,const double *c, int size)
{
    array.clear();
    if (size<1) return;
    for (int i=0; i<size; ++i)
	array.push_back(c[i]);
}



} // namespace PlotMM
