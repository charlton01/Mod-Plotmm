/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#ifndef PLOTMM_RECTANGLE_H
#define PLOTMM_RECTANGLE_H

#include <gdkmm/rectangle.h>

namespace PlotMM {

    /*! @brief Extends Gdk::Rectangle with some useful methods
     */
    class Rectangle : public Gdk::Rectangle
    {
    public:
	Rectangle() 
	    : Gdk::Rectangle() {}
	Rectangle(int x, int y, int width, int height)
	    : Gdk::Rectangle(x,y,width,height) {}

	inline int get_x_min() const;
	inline int get_x_max() const;
	inline int get_y_min() const;
	inline int get_y_max() const;
	inline int get_abs_width() const;
	inline int get_abs_height() const;

	inline void set_rect(int x, int y, int width, int height);
	inline void set_points(int x1, int y1, int x2, int y2);
	inline void set_origin(int x, int y);
	inline void set_destination(int x, int y);
    };

    //! Returns rectangle's lowest x
    inline int Rectangle::get_x_min() const
    { 
	int w= get_width(); 
	return w<0 ? get_x()+w : get_x(); 
    }

    //! Returns rectangle's highest x
    inline int Rectangle::get_x_max() const
    { 
	int w= get_width(); 
	return w<0 ? get_x() : get_x()+w; 
    }

    //! Returns rectangle's lowest y
    inline int Rectangle::get_y_min() const
    { 
	int h= get_height(); 
	return h<0 ? get_y()+h : get_y(); 
    }

    //! Returns rectangle's highest y
    inline int Rectangle::get_y_max() const
    { 
	int h= get_height(); 
	return h<0 ? get_y() : get_y()+h; 
    }

    //! Returns rectangle's absolute width
    inline int Rectangle::get_abs_width() const
    { 
	int w= get_width(); 
	return w<0 ? -w : w; 
    }

    //! Returns rectangle's absolute height
    inline int Rectangle::get_abs_height() const
    { 
	int h= get_height(); 
	return h<0 ? -h : h; 
    }

    //! Set rectangle by specifying one corner and with/height
    inline void Rectangle::set_rect(int x, int y, int width, int height)
    {
	set_x(x); set_y(y); set_width(width); set_height(height);
    }

    //! Set two opposing corners
    inline void Rectangle::set_points(int x1, int y1, int x2, int y2)
    {
	set_x(x1); set_y(y1); set_width(x2-x1); set_height(y2-y1);
    }

    //! Set one of two opposing corners
    inline void Rectangle::set_origin(int x, int y)
    { 
	set_x(x);
	set_y(y);
    }
    
    //! Set the other of two opposing corners
    inline void Rectangle::set_destination(int x, int y)
    {
	set_width(x-get_x());
	set_height(y-get_y());
    }

}

#endif // PLOT_RECTANGLE_H
