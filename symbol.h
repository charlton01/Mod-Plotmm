/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/
/* ported from qwt */

#ifndef PLOTMM_PLOTSYMBOL_H
#define PLOTMM_PLOTSYMBOL_H

#include <gtkmm/window.h>
#include "compat.h"
#if (GTKMM_MAJOR_VERSION == 2 && GTKMM_MINOR_VERSION > 4)
#include <glibmm/objectbase.h>
#else
#include <sigc++/object.h>
#endif

#include <glibmm/refptr.h>

namespace Gdk {
    class GC;
    class Point;
    class Drawable;
    class Rectangle;
}

namespace PlotMM {

    class Paint;

    /*! Style
     *
     *  \sa Symbol::set_style, Symbol::style
     */
    enum SymbolStyleID
    {
	SYMBOL_NONE,
	SYMBOL_ELLIPSE,
	SYMBOL_RECTANGLE,
	SYMBOL_DIAMOND,
	SYMBOL_TRIANGLE,
	SYMBOL_D_TRIANGLE,
	SYMBOL_U_TRIANGLE,
	SYMBOL_L_TRIANGLE,
	SYMBOL_R_TRIANGLE,
	SYMBOL_CROSS,
	SYMBOL_XCROSS,
	SymbolStyleCnt
    };

    /*! @brief A class for drawing symbols
     *
     *  Symbols are used by Curve to draw individual data points.
     *  There are various kinds of symbols that can be used like
     *  ellipses, rectangles, triangles (with different orientations)
     *  and crosses.  Additional symbols can be implemented by
     *  overloading draw_vfunc_().
     *
     *  \sa Curve::set_symbol()
     */
    class Symbol : public PlotMM::ObjectBase
    {
    public:
	Symbol(SymbolStyleID st=SYMBOL_NONE, int size=1, int sizeb=-1);
	virtual ~Symbol();

	bool operator!=(const Symbol &) const;
	bool operator==(const Symbol &) const;

	// void set_size(const QSize &s);
	void set_size(int a, int b = -1);
	void set_style (SymbolStyleID s);
	Glib::RefPtr<Paint> paint() const;

	//! Return Size
	int size() const { return size_; }
	//! Return auxiliary size or, if unset, size()
	int size_aux() const { return (sizeB_<0)?size_:sizeB_; }
	//! Return SymbolStyle
	SymbolStyleID style() const { return style_; }

	void draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &p,
		  const Gdk::Point &pt) const;
	void draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &p,
		  const Gdk::Rectangle &r) const;
	void draw(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &p,
			  int x, int y, int w=-1, int h=-1) const;

    protected:
	//! overload this function to provide additional symbol styles
	virtual void draw_vfunc_(const Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<Gdk::Window> &p,
				 int x, int y, int w, int h) const;


    private:
	Glib::RefPtr<Paint> paint_;
	int size_, sizeB_;
	SymbolStyleID style_;
	bool filled_;
    };

}

#endif
