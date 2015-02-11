

/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL
 *****************************************************************************/

#include "plot.h"
#include "scalediv.h"
#include "curve.h"
#include "errorcurve.h"
#include "symbol.h"
#include "paint.h"
#include "rectangle.h"

#include <gtkmm/checkbutton.h>
#include <gtkmm/button.h>

#include <gtkmm/statusbar.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/table.h>


#include <gtkmm/window.h>
#include <gtkmm/main.h>

#include <iostream>

class PlotTest : public Gtk::Window
{
public:
  PlotTest();
  virtual ~PlotTest();
  void do_the_plot();
  void replot();
  
    bool on_m_press(GdkEventButton*);
    bool on_m_release(GdkEventButton*);
    bool on_m_move(GdkEventMotion*); 
  //void on_plot_mouse_press(int x,int y, GdkEventButton *ev);

  //void on_plot_mouse_release(int x,int y, GdkEventButton *ev);

  //void on_plot_mouse_move(int x,int y, GdkEventMotion *ev);

  void print_coords(int x, int y);
  Glib::RefPtr<PlotMM::Curve> myCurve1;
  PlotMM::Plot m_plot;

    void set_x1_range( double, double, bool);

    void set_x2_range( double, double, bool);

    void set_y2_range( double, double, bool);

    void set_y1_range( double, double, bool);

    void set_x2_label(char*);
    void set_y2_label(char*);
    void set_x1_label(char*);
    void set_y1_label(char*);




protected:

    double x1_range_begin;
    double x1_range_end;
    bool x1_autoscale;
    double x2_range_begin;
    double x2_range_end;
    bool x2_autoscale;
    double y1_range_begin;
    double y1_range_end;
    bool y1_autoscale;
    double y2_range_begin;
    double y2_range_end;
    bool y2_autoscale;

  //virtual bool 	on_event (GdkEvent* event);
  void on_draw_clicked();

  PlotMM::Rectangle zoomRect_;
  bool button_[3];
  int mx_,my_;
  // Child widgets
  Gtk::Button m_Button_Draw, m_Button_Replot;
  Gtk::VBox m_box_top, m_box0, m_box1;
  //Gtk::Frame frame1;
  Gtk::Table table;

  Gtk::Statusbar m_sb;

  Glib::RefPtr<PlotMM::ErrorCurve> myCurve0;

  Glib::RefPtr<PlotMM::Curve> myCurve2;
//  Glib::RefPtr<PlotMM::Curve> myCurve3;
//  Glib::RefPtr<PlotMM::Curve> myCurve4;

  int loop;
  int cs;
};

PlotTest::PlotTest() :
  mx_(-1000),my_(-1000),
    x1_range_begin(0),
    x1_range_end(1200),
    x1_autoscale(false),
    x2_range_begin(0),
    x2_range_end(20),
    x2_autoscale(false),
    y1_range_begin(0),
    y1_range_end(600),
    y1_autoscale(false),
    y2_range_begin(0),
    y2_range_end(600),
    y2_autoscale(false),
  m_sb(),
  m_plot(),
  loop(0),
  m_Button_Draw("Draw Cairo Image"),
  m_Button_Replot("Call Replot")
 
{

    add(m_box_top);

//  m_box1.pack_start(m_Button_Draw);
//  m_box1.pack_start(m_Button_Replot);

  m_box1.pack_start(m_plot, Gtk::PACK_EXPAND_WIDGET, 5);
  m_plot.set_size_request(300, 300);

  // box0
  m_box0.pack_start(m_box1, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box0.pack_start(m_sb, Gtk::PACK_SHRINK, 5);
  m_box0.pack_start(m_Button_Draw, Gtk::PACK_SHRINK, 5);
  
  m_box0.pack_start(m_Button_Replot, Gtk::PACK_SHRINK, 5);
  
  m_box_top.pack_start(m_box0);
  

  set_border_width(0);
//  show_all();
    m_plot.canvas()->signal_button_press_event().connect(sigc::mem_fun(*this,&PlotTest::on_m_press));
    m_plot.canvas()->signal_button_release_event().connect(sigc::mem_fun(*this,&PlotTest::on_m_release));
    m_plot.canvas()->signal_motion_notify_event().connect(sigc::mem_fun(*this,&PlotTest::on_m_move));
  //m_plot.signal_plot_mouse_press().
  //  connect(sigc::mem_fun(*this,&PlotTest::on_plot_mouse_press));
  //m_plot.signal_plot_mouse_release().
  //  connect(sigc::mem_fun(*this,&PlotTest::on_plot_mouse_release));
  //m_plot.signal_plot_mouse_move().
  //  connect(sigc::mem_fun(*this,&PlotTest::on_plot_mouse_move));

    // set some axes to linear and others to logarithmic scale
    m_plot.scale(PlotMM::AXIS_BOTTOM)->set_range(x1_range_begin,x1_range_end,x1_autoscale);
    m_plot.scale(PlotMM::AXIS_TOP)->set_range(x2_range_begin,x2_range_end,x2_autoscale);
    m_plot.scale(PlotMM::AXIS_LEFT)->set_range(y1_range_begin,y1_range_end,y1_autoscale);
    m_plot.scale(PlotMM::AXIS_RIGHT)->set_range(y2_range_begin,y2_range_end,y2_autoscale);
    // allow for autoscaling on all axes
    m_plot.scale(PlotMM::AXIS_TOP)->set_autoscale(false);
//  m_plot.scale(PlotMM::AXIS_RIGHT)->set_autoscale(true);
    m_plot.scale(PlotMM::AXIS_BOTTOM)->set_autoscale(false);
    m_plot.scale(PlotMM::AXIS_LEFT)->set_autoscale(false);
    // set a plot title and some axis labels
    m_plot.title()->set_text("Plot");
    m_plot.title()->set_enabled(true);
    m_plot.label(PlotMM::AXIS_TOP)->set_text("top axis");
    m_plot.label(PlotMM::AXIS_RIGHT)->set_text("right axis");
    m_plot.label(PlotMM::AXIS_BOTTOM)->set_text("bottom axis");
    m_plot.label(PlotMM::AXIS_LEFT)->set_text("left axis");
    m_plot.scale(PlotMM::AXIS_RIGHT)->set_enabled(true);
    m_plot.scale(PlotMM::AXIS_LEFT)->set_enabled(true);
    m_plot.scale(PlotMM::AXIS_TOP)->set_enabled(true);
    m_plot.scale(PlotMM::AXIS_BOTTOM)->set_enabled(true);
    m_plot.label(PlotMM::AXIS_RIGHT)->set_enabled(true);
    m_plot.label(PlotMM::AXIS_LEFT)->set_enabled(true);
    m_plot.label(PlotMM::AXIS_TOP)->set_enabled(true);
    m_plot.label(PlotMM::AXIS_BOTTOM)->set_enabled(true);


  // create some named curves with different colors and symbols //
  myCurve0 = Glib::RefPtr<PlotMM::ErrorCurve>(new PlotMM::ErrorCurve("c1"));
 // myCurve0= Glib::RefPtr<PlotMM::Curve>(new PlotMM::Curve("c1"));

    myCurve1= Glib::RefPtr<PlotMM::Curve>(new PlotMM::Curve("c2"));
    myCurve2= Glib::RefPtr<PlotMM::Curve>(new PlotMM::Curve("c3"));
  //myCurve3= Glib::RefPtr<PlotMM::Curve>(new PlotMM::Curve("c4"));
  //myCurve4= Glib::RefPtr<PlotMM::Curve>(new PlotMM::Curve("c5"));
    myCurve1->set_curve_style(PlotMM::CURVE_LINES);
    myCurve1->paint()->set_pen_color(Gdk::Color("blue"));

    //m_plot.add_curve(myCurve0);
    //m_plot.add_curve(myCurve2);


//  m_plot.add_curve(myCurve3,PlotMM::AXIS_BOTTOM,PlotMM::AXIS_RIGHT);
//  m_plot.add_curve(myCurve4,PlotMM::AXIS_TOP);
  m_plot.add_curve(myCurve1);

    GdkColor g_red;
    myCurve1->paint()->set_pen_color(Gdk::Color("blue"));
  //myCurve3->paint()->set_pen_color(Gdk::Color("darkgreen"));
  //myCurve4->paint()->set_pen_color(Gdk::Color("darkred"));

    myCurve2->set_curve_style(PlotMM::CURVE_LINES);
    myCurve2->paint()->set_pen_color(Gdk::Color("green"));
    myCurve2->paint()->set_brush_color(Gdk::Color("blue"));

    myCurve2->symbol()->set_style(PlotMM::SYMBOL_R_TRIANGLE);
    myCurve2->symbol()->set_size(20);
    myCurve2->symbol()->paint()->set_pen_color(Gdk::Color("red"));
    myCurve2->symbol()->paint()->set_brush_color(Gdk::Color("yellow"));


  // some special settings for our error curve
  myCurve0->symbol()->paint()->set_pen_color(Gdk::Color("red"));
  myCurve0->error_paint()->set_pen_color(Gdk::Color("orange"));
    myCurve0->error_paint()->set_brush_color(Gdk::Color("blue"));
  myCurve0->symbol()->set_style(PlotMM::SYMBOL_ELLIPSE);
  myCurve0->symbol()->set_size(10);
  myCurve0->set_curve_style(PlotMM::CURVE_NONE);

  // of cource we also need data for the curves to show
  double x1[] = {0,25,1200,100,0,200,290}; double x2[] = {0,40,60};
  double y1[] = {0,10,80,600, 170, 0,290}; double y2[] = {10,15,20}; double y3[] = {4,5,4};
  double X1[] = {30.0,40.0,50.0,30.0,40.0,50.0,50.0};
  double Y1[] = {50.0,60.0,10.0,100.0,30.0,40.0,60.0};

  myCurve0->set_data(x1,y1,X1,Y1,7);
 myCurve1->set_data(x1,y1,7);
  myCurve2->set_data(x1,y1,7);
  //myCurve3->set_data(x2,y2,3);
  //myCurve4->set_data(x1,y3,3);

  // Connect the clicked signal of the Draw button
    m_Button_Draw.signal_clicked().connect(sigc::mem_fun(*this,
              &PlotTest::do_the_plot ));

    m_Button_Replot.signal_clicked().connect(sigc::mem_fun(*this,
              &PlotTest::replot ));

  show_all();

}

void PlotTest::set_x1_range( double x1, double x2, bool lin_log)
{
    x1_range_begin = x1;
    x1_range_end = x2;
    x1_autoscale = lin_log;
    m_plot.scale(PlotMM::AXIS_BOTTOM)->set_range(x1, x2,lin_log);
}

void PlotTest::set_x2_range( double x1, double x2, bool lin_log)
{
    x2_range_begin = x1;
    x2_range_end = x2;
    x2_autoscale = lin_log;
    m_plot.scale(PlotMM::AXIS_TOP)->set_range(x1, x2,lin_log);
}

void PlotTest::set_y2_range( double y1, double y2, bool lin_log)
{
    y2_range_begin = y1;
    y2_range_end = y2;
    y2_autoscale = lin_log;
    m_plot.scale(PlotMM::AXIS_RIGHT)->set_range(y1, y2,lin_log);
}

void PlotTest::set_y1_range( double y1, double y2, bool lin_log)
{
    y1_range_begin = y1;
    y1_range_end = y2;
    y1_autoscale = lin_log;
    m_plot.scale(PlotMM::AXIS_LEFT)->set_range(y1, y2,lin_log);
    // set the RIGHT axis to the same scale as the LEFT
    set_y2_range(y1, y2, lin_log);

}

void PlotTest::set_x2_label(char* str)
{
    m_plot.label(PlotMM::AXIS_TOP)->set_text(str);
}
void PlotTest::set_y2_label(char* str)
{
    m_plot.label(PlotMM::AXIS_RIGHT)->set_text(str);
}
void PlotTest::set_x1_label(char* str)
{
    m_plot.label(PlotMM::AXIS_BOTTOM)->set_text(str);
}
void PlotTest::set_y1_label(char* str)
{
    m_plot.label(PlotMM::AXIS_LEFT)->set_text(str);
}


//void PlotTest::on_plot_mouse_press(int x,int y, GdkEventButton *ev)
bool PlotTest::on_m_press(GdkEventButton *ev)
{	
	int x, y;
	m_plot.canvas()->get_pointer(x, y);
    if (ev->button>0 && ev->button<4) button_[ev->button-1]= true;
    print_coords(mx_=x,my_=y);

    if ((ev->button==1)) {  // zoom

        m_plot.scale(PlotMM::AXIS_BOTTOM)->set_autoscale(false);
        m_plot.scale(PlotMM::AXIS_LEFT)->set_autoscale(false);
        zoomRect_.set_rect(x,y,0,0);
        m_plot.set_selection(zoomRect_);
        m_plot.enable_selection();
    } else if ((ev->button==3)) {  // unzoom
//        m_plot.scale(PlotMM::AXIS_BOTTOM)->set_autoscale(true);
        set_x1_range(x1_range_begin, x1_range_end, x1_autoscale);
//        m_plot.scale(PlotMM::AXIS_LEFT)->set_autoscale(true);
        set_y1_range(y1_range_begin, y1_range_end, y1_autoscale);
        set_y2_range(y2_range_begin, y2_range_end, y2_autoscale);
        set_x2_range(x2_range_begin, x2_range_end, x2_autoscale);
        m_plot.replot();
    }
    return true;
}

//void PlotTest::on_plot_mouse_release(int x,int y, GdkEventButton *ev)
bool PlotTest::on_m_release(GdkEventButton *ev)
{
	int x, y;
	m_plot.canvas()->get_pointer(x, y);
    if (ev->button>0 && ev->button<4) button_[ev->button-1]= false;
    print_coords(mx_=x,my_=y);

    if ((ev->button==1)) {
        double x0,y0,x1,y1;
        int ix0,iy0,ix1,iy1;
        zoomRect_= m_plot.get_selection();
        ix0= zoomRect_.get_x_min();
        ix1= zoomRect_.get_x_max();
        iy0= zoomRect_.get_y_min();
        iy1= zoomRect_.get_y_max();
        x0= m_plot.scale(PlotMM::AXIS_BOTTOM)->scale_map().inv_transform(ix0);
        x1= m_plot.scale(PlotMM::AXIS_BOTTOM)->scale_map().inv_transform(ix1);
        y0= m_plot.scale(PlotMM::AXIS_LEFT)->scale_map().inv_transform(iy1);
        y1= m_plot.scale(PlotMM::AXIS_LEFT)->scale_map().inv_transform(iy0);
        m_plot.disable_selection();
        if (zoomRect_.get_width()==0 && zoomRect_.get_height()==0)
            return true;
        m_plot.scale(PlotMM::AXIS_TOP)->set_range(x0/60,x1/60);
        m_plot.scale(PlotMM::AXIS_BOTTOM)->set_range(x0,x1);
        m_plot.scale(PlotMM::AXIS_LEFT)->set_range(y0,y1);
        m_plot.scale(PlotMM::AXIS_RIGHT)->set_range(y0,y1);
        m_plot.replot();
        return true;
    }
}




  //void PlotTest::on_plot_mouse_move(int x,int y, GdkEventMotion *ev)
  bool PlotTest::on_m_move(GdkEventMotion *ev)
  {
    int x, y;
	m_plot.canvas()->get_pointer(x, y);
    print_coords(mx_=x,my_=y);
    zoomRect_.set_destination(x,y);
    m_plot.set_selection(zoomRect_);
    return true;

  }

  void PlotTest::print_coords(int x, int y)
  {
    char tmp[1000];
    sprintf(tmp,"(%d,%d): b:%8g; t:%8g; l:%8g; r:%8g; %c%c%c",x,y,
	    m_plot.scale(PlotMM::AXIS_BOTTOM)->scale_map().inv_transform(x),
	    m_plot.scale(PlotMM::AXIS_TOP)->scale_map().inv_transform(x),
	    m_plot.scale(PlotMM::AXIS_LEFT)->scale_map().inv_transform(y),
	    m_plot.scale(PlotMM::AXIS_RIGHT)->scale_map().inv_transform(y),
	    button_[0]?'*':'O',button_[1]?'*':'O',button_[2]?'*':'O');
    m_sb.pop();
    if (x>-1000&&y>-1000)
      m_sb.push(tmp);
  }


void PlotTest::replot()
{
	m_plot.replot();
}

void PlotTest::do_the_plot()
{
//    myCurve1->set_enabled(true);
 //   m_plot.replot();
    //canvas_.get_window()->show();
/*
double x[500], y[500];
for(int i=0; i<300; i++)
    {
        x[i] = i;
        y[i] = (i/30)*(i/15);
        myCurve1->set_data(x,y,i);
        m_plot.replot();
    }
 */
//    m_plot.replot();
//    return;
      int winx, winy, winw, winh;
    double xx, yy;

    m_plot.canvas()->get_window()->get_geometry(winx, winy, winw, winh);
//    printf("PlotCanvas window_ x, y, width and height are %d   %d   %d   %d\n", winx, winy, winw, winh);
    Cairo::RefPtr<Cairo::Context> gc_local;
    gc_local = m_plot.canvas()->get_window()->create_cairo_context();
// Glib::RefPtr<Gdk::Pixbuf> pixbuf_ = Gdk::Pixbuf::create (m_plot.canvas()->get_window(), 0, 0, winw, winh);

  gc_local->set_source_rgb(0.0, 0.0, 1.0);
  gc_local->set_line_width(1.0);
    gc_local->paint();

  Gtk::Allocation allocation = m_plot.canvas()->get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  // coordinates for the center of the window
  int xc, yc;
  xc = width / 2;
  yc = height / 2;
//  gc_local->set_line_width(10.0);

  // draw red lines out from the center of the window
  gc_local->set_source_rgb(0.8, 0.0, 0.0);
  gc_local->move_to(0, 0);
        gc_local->get_current_point(xx, yy);

  gc_local->line_to(xc, yc);
      gc_local->get_current_point(xx, yy);
//    printf(" Current points are xx = %f yy = %f", xx, yy);

  gc_local->line_to(0, height);
  gc_local->move_to(xc, yc);
  gc_local->line_to(width, yc);
  gc_local->stroke();

 //   gc_local->show_page();

}

PlotTest::~PlotTest()
{
}


int main(int argc, char** argv)
{
  Gtk::Main kit (argc,argv);
  PlotTest My_PlotTest;
 

  Gtk::Main::run(My_PlotTest);

  return 0;
}

