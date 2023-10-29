#include "helpers.h"

void create_dial_needle(QwtDial* dial)
{
    QwtDialSimpleNeedle* needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, Qt::yellow,
        QColor( Qt::gray ).lighter( 130 ) );
    needle->setWidth(2);
    dial->setNeedle( needle );
}

void set_thermo_color(QwtSlider* slider, QColor color, QColor bg)
{
    QString style = QString("#%1 { background-color: %2; color: %3;}").arg(slider->objectName(), bg.name(), color.name());

    slider->setStyleSheet(style);
}

void set_button_color(QPushButton* button, QColor color, QColor bg)
{
    QString style = QString("#%1 { background-color: %2; color: %3;}").arg(button->objectName(), bg.name(), color.name());

    button->setStyleSheet(style);
}

void set_button_color(QPushButton* button, QString color, QString bg)
{
    QString style = QString("#%1 { background-color: %2; color: %3;}").arg(button->objectName(), bg, color);

    button->setStyleSheet(style);
}

QColor button_bg_color(QPushButton* button)
{
    std::string input = button->styleSheet().toStdString();
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

QColor button_color(QPushButton* button)
{
    std::string input = button->styleSheet().toStdString();
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

void reset_button_color(QPushButton* button)
{
    button->setStyleSheet("");
}

bool is_read_only(QWidget* widget)
{
    return widget->testAttribute(Qt::WA_TransparentForMouseEvents) || (widget->focusPolicy() == Qt::NoFocus);
}

void set_read_only(QWidget* widget, bool read_only)
{
    widget->setAttribute(Qt::WA_TransparentForMouseEvents, read_only);
    widget->setFocusPolicy(read_only ? Qt::NoFocus : Qt::StrongFocus);
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
