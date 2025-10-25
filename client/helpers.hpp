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

/**
 * @brief Helper to create a dial needle
 *
 * @param dial The dial to create the needle for
 */
void create_dial_needle(QwtDial *dial);

/**
 * @brief Set the widget color object
 *
 * @param widget The widget to set the color for
 * @param color The color to set
 * @param bg The background color to set
 */
void set_widget_color(QWidget *widget, QColor color, QColor bg);

/**
 * @brief Set the widget color object
 *
 * @param widget The widget to set the color for
 * @param color The color to set
 * @param bg The background color to set
 */
void set_widget_color(QWidget *widget, QString color, QString bg);

/**
 * @brief Get the widget background color
 *
 * @param widget The widget to get the background color for
 * @return QColor The background color
 */
QColor widget_bg_color(QWidget *widget);

/**
 * @brief Get the widget color
 *
 * @param widget The widget to get the color for
 * @return QColor The color
 */
QColor widget_color(QWidget *widget);

/**
 * @brief Reset the widget color to default
 *
 * @param widget The widget to reset the color for
 */
void reset_widget_color(QWidget *widget);
