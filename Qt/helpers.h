#ifndef HELPERS_H
#define HELPERS_H

#include <QtWidgets>
#include <qwt_color_map.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_slider.h>
#include <qwt_series_data.h>
#include <QNetworkReply>

void create_dial_needle(QwtDial* dial);

void set_thermo_color(QwtSlider* slider, QColor color, QColor bg);
void set_button_color(QPushButton* button, QColor color, QColor bg);
void set_button_color(QPushButton* button, QString color, QString bg);
QColor button_bg_color(QPushButton* button);
QColor button_color(QPushButton* button);
void reset_button_color(QPushButton* button);
void set_read_only(QWidget* widget, bool readOnly = true);
bool is_read_only(QWidget* widget);
QString network_reply_to_fix(QNetworkReply::NetworkError error);

#endif // HELPERS_H
