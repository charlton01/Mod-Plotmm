/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#ifndef PLOTMM_DOUBLE_RECT_H
#define PLOTMM_DOUBLE_RECT_H

namespace Gdk {
    class Point;
}

namespace PlotMM {

    /*! @brief Class defining a point in double coordinates
     */

    class DoublePoint
    {
    public:
	DoublePoint();
	DoublePoint(double x, double y);
	DoublePoint(const Gdk::Point &);

	inline bool is_null()    const;

	inline double get_x() const;
	inline double get_y() const;
	inline void set_x(double x);
	inline void set_y(double y);

	bool operator==(const DoublePoint &) const;
	bool operator!=(const DoublePoint &) const;

	const DoublePoint operator-() const;
	const DoublePoint operator+(const DoublePoint &) const;
	const DoublePoint operator-(const DoublePoint &) const;
	const DoublePoint operator*(double) const;
	const DoublePoint operator/(double) const;

	DoublePoint &operator+=(const DoublePoint &);
	DoublePoint &operator-=(const DoublePoint &);
	DoublePoint &operator*=(double);
	DoublePoint &operator/=(double);

    private:
	double d_x;
	double d_y;
    };

    /*! @brief Class defining a size in double coordinates
     */

    class DoubleSize
    {
    public:
	DoubleSize();
	DoubleSize(double width, double height);
	// DoubleSize(const QSize &);

	inline bool is_null() const;
	inline bool is_empty() const;
	inline bool is_valid() const;

	inline double get_width() const;
	inline double get_height() const;
	inline void set_width( double w );
	inline void set_height( double h );
	void transpose();

	DoubleSize expanded_to(const DoubleSize &) const;
	DoubleSize bounded_to(const DoubleSize &) const;

	bool operator==(const DoubleSize &) const;
	bool operator!=(const DoubleSize &) const;

	const DoubleSize operator-() const;
	const DoubleSize operator+(const DoubleSize &) const;
	const DoubleSize operator-(const DoubleSize &) const;
	const DoubleSize operator*(double) const;
	const DoubleSize operator/(double) const;

	DoubleSize &operator+=(const DoubleSize &);
	DoubleSize &operator-=(const DoubleSize &);
	DoubleSize &operator*=(double c);
	DoubleSize &operator/=(double c);

    private:
	double d_width;
	double d_height;
    };

    /*! @brief Class defining a size in double coordinates.
     */

    class DoubleRect  
    {
    public:
	DoubleRect();
	DoubleRect(double x1, double x2, double y1, double y2);
	DoubleRect(double x, double y, const DoubleSize &);

	inline bool is_null()    const;
	inline bool is_empty()   const;
	inline bool is_valid()   const;

	DoubleRect normalize() const;

	inline double get_x1()  const;
	inline double get_x2()  const;
	inline double get_y1()  const;
	inline double get_y2()  const;

	inline void set_x1(double);
	inline void set_x2(double);
	inline void set_y1(double);
	inline void set_y2(double);

	DoublePoint center()  const;

	void move_x(double x);
	void move_y(double y );
	void move(double x, double y);
	void move_by(double dx, double dy);
	void move_center(const DoublePoint &);
	void move_center(double dx, double dy);

	void set_rect(double x1, double x2, double y1, double y2);

	inline double get_width()   const;
	inline double get_height()  const;
	inline DoubleSize size() const;

	inline void set_width(double w );
	inline void set_height(double h );
	void set_size(const DoubleSize &);

	DoubleRect  operator|(const DoubleRect &r) const;
	DoubleRect  operator&(const DoubleRect &r) const;
	DoubleRect &operator|=(const DoubleRect &r);
	DoubleRect &operator&=(const DoubleRect &r);
	bool operator==( const DoubleRect &) const;
	bool operator!=( const DoubleRect &) const;

	bool contains(const DoublePoint &p, bool proper = false) const;
	bool contains(double x, double y, bool proper = false) const; 
	bool contains(const DoubleRect &r, bool proper=false) const;

	DoubleRect unite(const DoubleRect &) const;
	DoubleRect intersect(const DoubleRect &) const;
	bool intersects(const DoubleRect &) const;

    private:
	double d_x1;
	double d_x2;
	double d_y1;
	double d_y2;
    };

    /*! 
     *  Returns TRUE if both the x value and the y value are 0; 
     *  otherwise returns false. 
     */
    inline bool DoublePoint::is_null() const
    { 
	return d_x == 0.0 && d_y == 0.0; 
    }

    //! Returns the x coordinate of the point. 
    inline double DoublePoint::get_x() const
    { 
	return d_x; 
    }

    //! Returns the y coordinate of the point. 
    inline double DoublePoint::get_y() const
    {   
	return d_y; 
    }

    //! Sets the x coordinate of the point to x. 
    inline void DoublePoint::set_x(double x)
    { 
	d_x = x; 
    }

    //! Sets the y coordinate of the point to x. 
    inline void DoublePoint::set_y(double y)
    { 
	d_y = y; 
    }

    /*!
     *  Returns TRUE if the width is 0 and the height is 0; 
     *  otherwise returns false.
     */
    inline bool DoubleSize::is_null() const
    { 
	return d_width == 0.0 && d_height == 0.0; 
    }

    /*! 
     *  Returns TRUE if the width is <= 0 or the height is <= 0, 
     *  otherwise false. 
     */
    inline bool DoubleSize::is_empty() const
    { 
	return d_width <= 0.0 || d_height <= 0.0; 
    }

    /*
     *  Returns TRUE if the width is equal to or greater than 0 and the height 
     *  is equal to or greater than 0; otherwise returns false.
     */
    inline bool DoubleSize::is_valid() const
    { 
	return d_width >= 0.0 && d_height >= 0.0; 
    }

    //! Returns the width. 
    inline double DoubleSize::get_width() const
    { 
	return d_width; 
    }

    //! Returns the height. 
    inline double DoubleSize::get_height() const
    { 
	return d_height; 
    }

    //! Sets the width to width. 
    inline void DoubleSize::set_width(double width)
    { 
	d_width = width; 
    }

    //! Sets the height to height. 
    inline void DoubleSize::set_height(double height)
    { 
	d_height = height; 
    }

    /*!
     *  Returns TRUE if the rectangle is a null rectangle; 
     *  otherwise returns false. 
     *
     *  A rect is null when x1() == x2() && y1() == y2().
     */
    inline bool DoubleRect::is_null() const
    { 
	return d_x2 == d_x1 && d_y2 == d_y1;
    }

    /*!
     *  Returns TRUE if the rectangle is a empty rectangle; 
     *  otherwise returns false. 
     *
     *  A rect is null when x1() >= x2() && y1() >= y2().
     */
    inline bool DoubleRect::is_empty() const
    { 
	return d_x1 >= d_x2 || d_y1 >= d_y2; 
    }

    /*!
     *  Returns TRUE if the rectangle is a valid rectangle; 
     *  otherwise returns false. 
     *
     *  A rect is valid when x1() <= x2() && y1() <= y2().
     */
    inline bool DoubleRect::is_valid() const
    { 
	return d_x1 <= d_x2 && d_y1 <= d_y2; 
    }

    //! Returns x1
    inline double DoubleRect::get_x1() const
    { 
	return d_x1; 
    }

    //! Returns x2
    inline double DoubleRect::get_x2() const
    { 
	return d_x2; 
    }

    //! Returns y1
    inline double DoubleRect::get_y1() const
    { 
	return d_y1; 
    }

    //! Returns y2
    inline double DoubleRect::get_y2() const
    { 
	return d_y2; 
    }

    //! Set x1  
    inline void DoubleRect::set_x1(double x1)
    { 
	d_x1 = x1;
    }

    //! Set x2  
    inline void DoubleRect::set_x2(double x2)
    { 
	d_x2 = x2;
    }

    //! Set y1  
    inline void DoubleRect::set_y1(double y1)
    { 
	d_y1 = y1;
    }

    //! Set y2  
    inline void DoubleRect::set_y2(double y2)
    { 
	d_y2 = y2;
    }

    //! Returns the width
    inline double DoubleRect::get_width() const
    { 
	return  d_x2 - d_x1; 
    }

    //! Returns the height
    inline double DoubleRect::get_height() const
    { 
	return  d_y2 - d_y1; 
    }

    //! Returns the size
    inline DoubleSize DoubleRect::size() const
    { 
	return DoubleSize(get_width(), get_height());
    }

    //! Set the width, by x2 = x1 + w;
    inline void DoubleRect::set_width(double w)
    {
	d_x2 = d_x1 + w;
    }

    //! Set the height, by y2 = y1 + h;
    inline void DoubleRect::set_height(double h)
    {
	d_y2 = d_y1 + h;
    }

}

#endif // PLOT_DOUBLE_RECT_H
