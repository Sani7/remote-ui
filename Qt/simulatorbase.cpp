#include "simulatorbase.h"

SimulatorBase::SimulatorBase(Web_socket_wrapper* web_socket, QWidget* parent) :
      QMainWindow{parent},
      m_error_dialog(new NetworkError(this))
{
    this->m_web_socket = web_socket;
}

void SimulatorBase::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    QD << "Connecting callbacks";
    setup_cb();
    this->timer_update->start(refresh_rate);
    closed = false;
}

void SimulatorBase::closeEvent(QCloseEvent* event)
{
    if (!closed)
    {
        QD << "Disconnecting callbacks";
        QWidget::closeEvent(event);
        this->timer_update->stop();
        disconnect(api, nullptr, nullptr, nullptr);
        this->parentWidget()->show();
    }
    closed = true;
}

void SimulatorBase::setup_cb(void)
{
    connect(m_web_socket, &Web_socket_wrapper::on_command_cb, this, [=](json& j){on_cmd_cb(j);});
    connect(m_web_socket, &Web_socket_wrapper::on_event_cb, this, [=](json& j){on_event_cb(j);});
}

void SimulatorBase::on_cmd_cb(json& j)
{
    auto response = magic_enum::enum_cast<Web_socket_wrapper::Command>(std::string(j.at("type"))).value_or(Web_socket_wrapper::Command::end);
    switch (response)
    {
        case Web_socket_wrapper::Command::get_UI_elements:
        case Web_socket_wrapper::Command::get_UI_element:
            UI_item_parser(j);
            break;
        default:
            break;
    }
}

void SimulatorBase::on_event_cb(json& j)
{
    auto response = magic_enum::enum_cast<Web_socket_wrapper::Event>(std::string(j.at("type"))).value_or(Web_socket_wrapper::Event::end);
    switch (response)
    {
        case Web_socket_wrapper::Event::UI_changed:
            UI_item_parser(j);
            break;
        default:
            break;
    }
}

void SimulatorBase::UI_item_parser(json& input)
{
    for (auto& uiItem : input["UI_items"])
    {
        if (uiItem["type"] == "UI_button")
        {
            process_ui_button(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_led")
        {
            process_ui_led(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_slider")
        {
            process_ui_slider(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_dial")
        {
            process_ui_dial(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_thermo")
        {
            process_ui_thermo(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_textbox")
        {
            process_ui_textbox(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_label")
        {
            process_ui_label(uiItem);
            continue;
        }
        if (uiItem["type"] == "UI_combobox")
        {
            process_ui_combobox(uiItem);
            continue;
        }
        if (uiItem["type"] == "UIRadioButton")
        {
            process_ui_radiobutton(uiItem);
            continue;
        }
    }
}

void SimulatorBase::process_ui_label(json& uiItem)
{
    QLabel* label = id_to_label(QString::fromStdString(uiItem["id"]));
    if (label == nullptr)
    {
        QD << "id_to_label returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    QString value = QString::fromStdString(uiItem["text"]);

    if (label->text() != value)
    {
        label->setText(value);
    }
}

void SimulatorBase::process_ui_slider(json& uiItem)
{
    QwtSlider* slider = id_to_slider(QString::fromStdString(uiItem["id"]));
    QLabel* label = id_to_slider_label(QString::fromStdString(uiItem["id"]));
    if (slider == nullptr)
    {
        QD << "id_to_slider returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    double value = uiItem["value"];
    bool enable = uiItem["enabled"];
    bool visible = uiItem["visible"];

    if (slider->value() != value)
    {
        if (label != nullptr)
        {
            label->setText(format_slider_value(QString::fromStdString(uiItem["id"]), value));
        }
        slider->setValue(value);
    }

    if (slider->isReadOnly() != !enable)
    {
        slider->setReadOnly(!enable);
    }

    if (slider->isVisible() != visible)
    {
        slider->setVisible(visible);
    }
    //TODO: Add color for scale
    return;
}

void SimulatorBase::process_ui_dial(json& uiItem)
{
    QwtDial* dial = id_to_dial(QString::fromStdString(uiItem["id"]));
    QLabel* label = id_to_dial_label(QString::fromStdString(uiItem["id"]));
    if (dial == nullptr)
    {
        QD << "id_to_dial returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    double value = uiItem["value"];
    bool visible = uiItem["visible"];

    if (dial->value() != value)
    {
        if (label != nullptr)
        {
            label->setText(format_slider_value(QString::fromStdString(uiItem["id"]), value));
        }
        dial->setValue(value);
    }


    if (dial->isVisible() != visible)
    {
        dial->setVisible(visible);
    }
    return;
}

void SimulatorBase::process_ui_thermo(json& uiItem)
{
    QwtThermo* thermo = id_to_thermo(QString::fromStdString(uiItem["id"]));
    QLabel* label = id_to_thermo_label(QString::fromStdString(uiItem["id"]));
    if (thermo == nullptr)
    {
        QD << "id_to_dial returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    double value = uiItem["value"];
    bool visible = uiItem["visible"];

    if (thermo->value() != value)
    {
        if (label != nullptr)
        {
            label->setText(format_slider_value(QString::fromStdString(uiItem["id"]), value));
        }
        thermo->setValue(value);
    }


    if (thermo->isVisible() != visible)
    {
        thermo->setVisible(visible);
    }
    return;
}

void SimulatorBase::process_ui_textbox(json& uiItem)
{
    QD << "Not implemented";
    emit quit();
}

void SimulatorBase::process_ui_combobox(json& uiItem)
{
    QComboBox* combobox = id_to_combobox(QString::fromStdString(uiItem["id"]));
    if (combobox == nullptr)
    {
        QD << "id_to_combobox returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    QString selected = QString::fromStdString(uiItem["selected"]);

    for (json& item : uiItem["options"])
    {
        if (combobox->findText(QString::fromStdString(item)) == -1)
            combobox->addItem(QString::fromStdString(item));
    }

    if (combobox->currentText() != selected)
    {
        combobox->setCurrentText(selected);
    }
}

void SimulatorBase::process_ui_radiobutton(json& uiItem)
{
    QD << "Not implemented";
    emit quit();
}

void SimulatorBase::process_ui_button(json& uiItem)
{
    //TODO: fix bgcolor and color
    QPushButton* button = id_to_button(QString::fromStdString(uiItem["id"]));

    if (button == nullptr)
    {
        QD << "id_to_button returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(uiItem["bg_color"]));
    QColor fg_color = QColor(QString::fromStdString(uiItem["fg_color"]));
    QString text = QString::fromStdString(uiItem["text"]);
    bool enabled = uiItem["enabled"];

    if (button->text() != text)
    {
        button->setText(text);
    }

    if (button->isEnabled() != enabled)
    {
        button->setEnabled(enabled);
    }

    if (bg_color == QColor(0x83, 0x91, 0x92))
    {
        reset_button_color(button);
        return;
    }

    if (bg_color == QColor(0xFF, 0x00, 0x00))
    {
        set_button_color(button, QString("black"), QString("darkred"));
        return;
    }

    if (bg_color == QColor(0x00, 0xFF, 0x00))
    {
        set_button_color(button, QString("white"), QString("darkgreen"));
        return;
    }

    if (bg_color == QColor(0xFF8800))
    {
        set_button_color(button, QString("darkred"), QString("darkorange"));
        return;
    }

    if (bg_color != button_bg_color(button) || fg_color != button_color(button))
    {
        set_button_color(button, fg_color, bg_color);
    }
}

void SimulatorBase::process_ui_led(json& uiItem)
{
    QPushButton* led = id_to_led(QString::fromStdString(uiItem["id"]));
    if (led == nullptr)
    {
        QD << "id_to_led returned null on " << QString::fromStdString(uiItem["id"]);
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(uiItem["led_color"]));
    QColor color = QColor(~bg_color.red(), ~bg_color.green(), ~bg_color.blue());
    QString text = QString::fromStdString(uiItem["text"]);

    if (false == is_read_only(led))
    {
        set_read_only(led);
    }

    if (text != led->text())
    {
        led->setText(text);
    }

    if (bg_color == QColor(0x83, 0x91, 0x92))
    {
        reset_button_color(led);
        return;
    }

    if (bg_color == QColor(0xFF, 0x00, 0x00))
    {
        set_button_color(led, QString("black"), QString("darkred"));
        return;
    }

    if (bg_color == QColor(0x00, 0xFF, 0x00))
    {
        set_button_color(led, QString("white"), QString("darkgreen"));
        return;
    }

    if (bg_color == QColor(0xFF8800))
    {
        set_button_color(led, QString("darkred"), QString("darkorange"));
        return;
    }

    if (bg_color != button_bg_color(led))
    {
        set_button_color(led, color, bg_color);
    }
}
