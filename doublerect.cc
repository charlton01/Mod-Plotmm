/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#include <gdkmm/types.h>

#include "doublerect.h"
#include "supplemental.h"

using namespace PlotMM;

//! Constructs a point with coordinates (0.0, 0.0) (is_null() returns TRUE).

DoublePoint::DoublePoint():
    d_x(0.0),
    d_y(0.0)
{
}

//! Constructs a point with x value xpos and y value ypos.

DoublePoint::DoublePoint(double x, double y ):
    d_x(x),
    d_y(y)
{
}

//! Copy constructor
DoublePoint::DoublePoint(const Gdk::Point &p):
    d_x(double(p.get_x())),
    d_y(double(p.get_y()))
{
}

//! Returns TRUE if point and other are equal; otherwise returns FALSE.
bool DoublePoint::operator==(const DoublePoint &other) const
{
    return (d_x == other.d_x) && (d_y == other.d_y);
}

//! Returns TRUE if point and other are not equal; otherwise returns FALSE.
bool DoublePoint::operator!=(const DoublePoint &other) const
{
    return !operator==(other);
}

/*!
  Returns the point formed by changing the sign of both components,
  equivalent to QDoublePoint(-x(), -y());
*/

const DoublePoint DoublePoint::operator-() const
{
    return DoublePoint(-d_x, -d_y);
}

/*!
  Returns the point formed by adding both components by
  the components of other. Each component is added separately.
*/

const DoublePoint DoublePoint::operator+(
    const DoublePoint &other) const
{
    return DoublePoint(d_x + other.d_x, d_y + other.d_y);
}

/*!
  Returns the point formed by subtracting both components by
  the components of other. Each component is subtracted separately.
*/

const DoublePoint DoublePoint::operator-(
    const DoublePoint &other) const
{
    return DoublePoint(d_x - other.d_x, d_y - other.d_y);
}

//! Returns the point formed by multiplying both components by c.

const DoublePoint DoublePoint::operator*(double c) const
{
    return DoublePoint(d_x * c, d_y * c);
}

//! Returns the point formed by dividing both components by c.

const DoublePoint DoublePoint::operator/(double c) const
{
    return DoublePoint(d_x / c, d_y / c);
}

//! Adds point other to this point and returns a reference to this point.

DoublePoint &DoublePoint::operator+=(const DoublePoint &other)
{
    d_x += other.d_x;
    d_y += other.d_y;
    return *this;
}

//! Subtracts point other from this point and returns a reference to this point.

DoublePoint &DoublePoint::operator-=(const DoublePoint &other)
{
    d_x -= other.d_x;
    d_y -= other.d_y;
    return *this;
}

//! Multiplies this point's x and y by c, and returns a reference to this point.

DoublePoint &DoublePoint::operator*=(double c)
{
    d_x *= c;
    d_y *= c;
    return *this;
}

//! Divides both x and y by c, and returns a reference to this point.

DoublePoint &DoublePoint::operator/=(double c)
{
    d_x /= c;
    d_y /= c;
    return *this;
}

//! Constructs a size with a width and height of 0.0.

DoubleSize::DoubleSize():
    d_width(0.0),
    d_height(0.0)
{
}

//! Constructs a size with a width w and height h.

DoubleSize::DoubleSize( double w, double h ):
    d_width(w),
    d_height(h)
{
}

//! Copy constructor
/*
DoubleSize::DoubleSize(const QSize &sz):
    d_width(double(sz.width())),
    d_height(double(sz.height()))
{
}
*/

//! Swaps the values of width and height.

void DoubleSize::transpose()
{
    double tmp = d_width;
    d_width = d_height;
    d_height = tmp;
}

/*!
  Returns a size with the maximum width and height
  of this size and other.
*/

DoubleSize DoubleSize::expanded_to(
    const DoubleSize &other) const
{
    return DoubleSize(
        MAX(d_width, other.d_width),
        MAX(d_height, other.d_height)
    );
}

/*!
  Returns a size with the minimum width and height of
  this size and other.
*/

DoubleSize DoubleSize::bounded_to(
    const DoubleSize &other) const
{
    return DoubleSize(
        MIN(d_width, other.d_width),
        MIN(d_height, other.d_height)
    );
}

//! Returns TRUE if the size is equal to other; otherwise returns FALSE.

bool DoubleSize::operator==(const DoubleSize &other) const
{
    return d_width == other.d_width && d_height == other.d_height;
}

//! Returns TRUE if the size is not equal to other; otherwise returns FALSE.

bool DoubleSize::operator!=(const DoubleSize &other) const
{
    return !operator==(other);
}

/*!
  Returns the size formed by changing the sign of both components,
  equivalent to DoubleSize(-width(), -height());
*/

const DoubleSize DoubleSize::operator-() const
{
    return DoubleSize(-d_width, -d_height);
}

/*!
  Returns the size formed by adding both components by
  the components of other. Each component is added separately.
*/

const DoubleSize DoubleSize::operator+(
    const DoubleSize &other) const
{
    return DoubleSize(d_width + other.d_width,
        d_height + other.d_height);
}

/*!
  Returns the size formed by subtracting both components by
  the components of other. Each component is subtracted separately.
*/

const DoubleSize DoubleSize::operator-(
    const DoubleSize &other) const
{
    return DoubleSize(d_width - other.d_width,
        d_height - other.d_height);
}

//! Returns the size formed by multiplying both components by c.

const DoubleSize DoubleSize::operator*(double c) const
{
    return DoubleSize(d_width * c, d_height * c);
}

//! Returns the size formed by dividing both components by c.

const DoubleSize DoubleSize::operator/(double c) const
{
    return DoubleSize(d_width / c, d_height / c);
}

//! Adds size other to this size and returns a reference to this size.

DoubleSize &DoubleSize::operator+=(const DoubleSize &other)
{
    d_width += other.d_width;
    d_height += other.d_height;
    return *this;
}

//! Subtracts size other from this size and returns a reference to this size.

DoubleSize &DoubleSize::operator-=(const DoubleSize &other)
{
    d_width -= other.d_width;
    d_height -= other.d_height;
    return *this;
}

/*
  Multiplies this size's width and height by c,
  and returns a reference to this size.
*/

DoubleSize &DoubleSize::operator*=(double c)
{
    d_width *= c;
    d_height *= c;
    return *this;
}

/*
  Devides this size's width and height by c,
  and returns a reference to this size.
*/

DoubleSize &DoubleSize::operator/=(double c)
{
    d_width /= c;
    d_height /= c;
    return *this;
}

//! Constructs an rectangle with all components set to 0.0
DoubleRect::DoubleRect():
    d_x1(0.0),
    d_x2(0.0),
    d_y1(0.0),
    d_y2(0.0)
{
}

/*!
  Constructs an rectangle with x1 to x2 as x-range and,
  y1 to y2 as y-range.
*/

DoubleRect::DoubleRect(double x1, double x2,
        double y1, double y2):
    d_x1(x1),
    d_x2(x2),
    d_y1(y1),
    d_y2(y2)
{
}

/*!
  Constructs an rectangle with x1 to x1 + size.width() as x-range and,
  y1 to y1 + size.height() as y-range.
*/

DoubleRect::DoubleRect(double x, double y, const DoubleSize &size):
    d_x1(x),
    d_x2(x + size.get_width()),
    d_y1(y),
    d_y2(y + size.get_height())
{
}

/*!
  Set the x-range from x1 to x2 and the y-range from y1 to y2.
*/
void DoubleRect::set_rect(double x1, double x2, double y1, double y2)
{
    d_x1 = x1;
    d_x2 = x2;
    d_y1 = y1;
    d_y2 = y2;
}

/*!
  Sets the size of the rectangle to size.
  Changes x2 and y2 only.
*/

void DoubleRect::set_size(const DoubleSize &size)
{
    set_width(size.get_width());
    set_height(size.get_height());
}

/*!
  Returns a normalized rectangle, i.e. a rectangle that has a non-negative
  width and height.

  It swaps x1 and x2 if x1() > x2(), and swaps y1 and y2 if y1() > y2().
*/

DoubleRect DoubleRect::normalize() const
{
    DoubleRect r;
    if ( d_x2 < d_x1 )
    {
        r.d_x1 = d_x2;
        r.d_x2 = d_x1;
    }
    else
    {
        r.d_x1 = d_x1;
        r.d_x2 = d_x2;
    }
    if ( d_y2 < d_y1 )
    {
        r.d_y1 = d_y2;
        r.d_y2 = d_y1;
    }
    else
    {
        r.d_y1 = d_y1;
        r.d_y2 = d_y2;
    }
    return r;
}

/*!
  Returns the bounding rectangle of this rectangle and rectangle other.
  r.unite(s) is equivalent to r|s.
*/

DoubleRect DoubleRect::unite(const DoubleRect &other) const
{
    return *this | other;
}

/*!
  Returns the intersection of this rectangle and rectangle other.
  r.intersect(s) is equivalent to r&s.
*/

DoubleRect DoubleRect::intersect(const DoubleRect &other) const
{
    return *this & other;
}

/*!
  Returns TRUE if this rectangle intersects with rectangle other;
  otherwise returns FALSE.
*/

bool DoubleRect::intersects(const DoubleRect &other) const
{
    return ( MAX(d_x1, other.d_x1) <= MIN(d_x2, other.d_x2) ) &&
         ( MAX(d_y1, other.d_y1 ) <= MIN(d_y2, other.d_y2) );
}

//! Returns TRUE if this rect and other are equal; otherwise returns FALSE.

bool DoubleRect::operator==(const DoubleRect &other) const
{
    return d_x1 == other.d_x1 && d_x2 == other.d_x2 &&
        d_y1 == other.d_y1 && d_y2 == other.d_y2;
}

//! Returns TRUE if this rect and other are different; otherwise returns FALSE.

bool DoubleRect::operator!=(const DoubleRect &other) const
{
    return !operator==(other);
}

/*!
  Returns the bounding rectangle of this rectangle and rectangle other.
  The bounding rectangle of a nonempty rectangle and an empty or
  invalid rectangle is defined to be the nonempty rectangle.
*/

DoubleRect DoubleRect::operator|(const DoubleRect &other) const
{
    if ( !is_valid() )
        return other;

    if ( !other.is_valid() )
        return *this;

    return DoubleRect(MIN(d_x1, other.d_x1), MAX(d_x2, other.d_x2),
        MIN(d_y1, other.d_y1), MAX(d_y2, other.d_y2) );
}

/*!
  Returns the intersection of this rectangle and rectangle other.
  Returns an empty rectangle if there is no intersection.
*/

DoubleRect DoubleRect::operator&(const DoubleRect &other) const
{
    return DoubleRect(MAX(d_x1, other.d_x1), MIN(d_x2, other.d_x2),
        MAX(d_y1, other.d_y1), MIN(d_y2, other.d_y2));
}

//! Unites this rectangle with rectangle other.

DoubleRect &DoubleRect::operator|=(const DoubleRect &other)
{
    *this = *this | other;
    return *this;
}

//! Intersects this rectangle with rectangle other.

DoubleRect &DoubleRect::operator&=(const DoubleRect &other)
{
    *this = *this & other;
    return *this;
}

//! Returns the center point of the rectangle.

DoublePoint DoubleRect::center() const
{
    return DoublePoint(d_x1 + (d_x2 - d_x1) / 2.0,
        d_y1 + (d_y2 - d_y1) / 2.0);
}

/*!
  Returns TRUE if the point (x, y) is inside or on the edge of the rectangle;
  otherwise returns FALSE.

  If proper is TRUE, this function returns TRUE only if p is inside
  (not on the edge).
*/

bool DoubleRect::contains(double x, double y, bool proper) const
{
    if ( proper )
        return x > d_x1 && x < d_x2 && y > d_y1 && y < d_y2;
    else
        return x >= d_x1 && x <= d_x2 && y >= d_y1 && y <= d_y2;
}

/*!
  Returns TRUE if the point p is inside or on the edge of the rectangle;
  otherwise returns FALSE.

  If proper is TRUE, this function returns TRUE only if p is inside
  (not on the edge).
*/

bool DoubleRect::contains(const DoublePoint &p, bool proper) const
{
    return contains(p.get_x(), p.get_y(), proper);
}

/*!
  Returns TRUE if the rectangle other is inside this rectangle;
  otherwise returns FALSE.

  If proper is TRUE, this function returns TRUE only if other is entirely
  inside (not on the edge).
*/

bool DoubleRect::contains(const DoubleRect &other, bool proper) const
{
    return contains(other.d_x1, other.d_y1, proper) &&
        contains(other.d_x2, other.d_y2, proper);
}

//! moves x1() to x, leaving the size unchanged

void DoubleRect::move_x(double x)
{
    const double w = get_width();
    d_x1 = x;
    d_x2 = d_x1 + w;
}

//! moves y1() to y, leaving the size unchanged

void DoubleRect::move_y(double y)
{
    const double h = get_height();
    d_y1 = y;
    d_y2 = d_y1 + h;
}

//! moves x1() to x and y1() to y, leaving the size unchanged

void DoubleRect::move(double x, double y)
{
    move_x(x);
    move_y(y);
}

//! moves x1() by dx and y1() by dy. leaving the size unchanged

void DoubleRect::move_by(double dx, double dy)
{
    d_x1 += dx;
    d_x2 += dx;
    d_y1 += dy;
    d_y2 += dy;
}

//! moves the center to pos, leaving the size unchanged

void DoubleRect::move_center(const DoublePoint &pos)
{
    move_center(pos.get_x(), pos.get_y());
}

//! moves the center to (x, y), leaving the size unchanged

void DoubleRect::move_center(double x, double y)
{
    move(x - get_width() / 2.0, y - get_height() / 2.0);
}
