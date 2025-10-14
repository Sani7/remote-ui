#pragma once
#include <QtWidgets>
#include <qwt_color_map.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_slider.h>
#include <qwt_thermo.h>

void create_dial_needle(QwtDial *dial);

void set_widget_color(QWidget *widget, QColor color, QColor bg);
void set_widget_color(QWidget *widget, QString color, QString bg);
QColor widget_bg_color(QWidget *widget);
QColor widget_color(QWidget *widget);
void reset_widget_color(QWidget *widget);
