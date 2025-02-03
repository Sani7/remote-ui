#include "helpers.h"

void create_dial_needle(QwtDial* dial)
{
    QwtDialSimpleNeedle* needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, Qt::yellow,
        QColor( Qt::gray ).lighter( 130 ) );
    needle->setWidth(2);
    dial->setNeedle( needle );
}

void set_widget_color(QWidget* widget, QColor color, QColor bg)
{
    QString style = QString("#%1 { background-color: %2; color: %3;}").arg(widget->objectName(), bg.name(), color.name());

    widget->setStyleSheet(style);
}

void set_widget_color(QWidget* widget, QString color, QString bg)
{
    QString style = QString("#%1 { background-color: %2; color: %3;}").arg(widget->objectName(), bg, color);

    widget->setStyleSheet(style);
}

QColor widget_bg_color(QWidget* widget)
{
    std::string input = widget->styleSheet().toStdString();
    QColor background_color;

           // Find the position of "background-color:"
    size_t bg_pos = input.find("background-color:");
    if (bg_pos != std::string::npos) {
        // Find the end position of the background color value
        size_t bg_value_start = bg_pos + 17; // Length of "background-color:" is 17
        size_t bg_value_end = input.find(";", bg_value_start);
        if (bg_value_end != std::string::npos) {
            background_color = QColor(input.substr(bg_value_start, bg_value_end - bg_value_start).c_str());
        }
    }

    return background_color;
}

QColor widget_color(QWidget* widget)
{
    std::string input = widget->styleSheet().toStdString();
    QColor color;

           // Find the position of "color:"
    size_t color_pos = input.find(" color:");
    if (color_pos != std::string::npos) {
        // Find the end position of the text color value
        size_t color_value_start = color_pos + 7; // Length of "color:" is 7
        size_t color_value_end = input.find(";", color_value_start);
        if (color_value_end != std::string::npos) {
            color = QColor(input.substr(color_value_start, color_value_end - color_value_start).c_str());
        }
    }
    return color;
}

void reset_widget_color(QWidget* widget)
{
    widget->setStyleSheet("");
}

QString network_reply_to_fix(QNetworkReply::NetworkError error)
{
    switch (error) {
        case QNetworkReply::NetworkError::ConnectionRefusedError:
            return "Make sure the server is started ($ systemctl status unocan_server)";
            break;
        case QNetworkReply::NetworkError::HostNotFoundError:
            return "Make sure the simulator is turned on";
            break;
        case QNetworkReply::NetworkError::InternalServerError:
            return "Server had an oopsie, restart the client and if it fails again restart the server";
            break;
        default:
            return "";
            break;
    }
}
