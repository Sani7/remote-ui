#include "simulator_base.h"
#include "can_transceive.h"

SimulatorBase::SimulatorBase(Web_socket_wrapper* web_socket, QWidget* parent) :
      QMainWindow{parent},
      m_error_dialog(new NetworkError(this)),
      m_timer_update(new QTimer()),
      m_web_socket(web_socket)
{
    m_error_dialog->set_error("Connection timed out\nCheck if the server is running");
    m_ui_lookup.reserve(40);
}

QWidget* SimulatorBase::id_to_ui(size_t id)
{
    return m_ui_lookup.at(id);
}

void SimulatorBase::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    SPDLOG_DEBUG("Connecting callbacks");

    connect(m_web_socket, &Web_socket_wrapper::on_command_cb, this, [=, this](json& j){on_cmd_cb(j);});
    connect(m_web_socket, &Web_socket_wrapper::on_event_cb, this, [=, this](json& j){on_event_cb(j);});
    connect(m_web_socket, &Web_socket_wrapper::on_closed, this, [=, this]{m_error_dialog->open();});

    m_web_socket->send_command(Web_socket_wrapper::Command::get_UI_elements);
    this->m_timer_update->start(m_refresh_rate);
    m_open = true;
}

void SimulatorBase::closeEvent(QCloseEvent* event)
{
    if (m_open == true)
    {
        SPDLOG_DEBUG("Disconnecting callbacks");
        QWidget::closeEvent(event);
        m_timer_update->stop();
        disconnect(m_web_socket, nullptr, nullptr, nullptr);
        m_open = false;
        this->parentWidget()->show();
    }
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
        case Web_socket_wrapper::Event::ui_changed:
            UI_item_parser(j);
            break;
        default:
            break;
    }
}

void SimulatorBase::push_ui_item(QWidget* item)
{
    m_ui_lookup.emplace_back(item);
    setup_ui_item(item, m_ui_lookup.size() - 1);
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
        if (uiItem["type"] == "UI_can")
        {
            process_ui_can(uiItem);
            continue;
        }
    }
}

void SimulatorBase::process_ui_label(json& uiItem)
{
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto label = qobject_cast<QLabel*>(widget);
    if (label == nullptr)
    {
        SPDLOG_WARN("widget is not of type QLabel");
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
    QLabel* label = id_to_label(uiItem["id"]);
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto slider = qobject_cast<QwtSlider*>(widget);
    if (slider == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtSlider");
        return;
    }

    double value = uiItem["value"];
    bool enable = uiItem["enabled"];
    bool visible = uiItem["visible"];
    QString unit = QString::fromStdString(uiItem.at("unit"));

    if (slider->value() != value)
    {
        if (label != nullptr)
        {
            label->setText(QString("%0 %1").arg(value).arg(unit));
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
    QLabel* label = id_to_label(uiItem["id"]);
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto dial = qobject_cast<QwtDial*>(widget);
    if (dial == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtDial");
        return;
    }

    double value = uiItem["value"];
    bool visible = uiItem["visible"];
    QString unit = QString::fromStdString(uiItem.at("unit"));

    if (dial->value() != value)
    {
        if (label != nullptr)
        {
            label->setText(QString("%0 %1").arg(value).arg(unit));
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
    QLabel* label = id_to_label(uiItem["id"]);
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto thermo = qobject_cast<QwtThermo*>(widget);
    if (thermo == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtThermo");
        return;
    }

    double value = uiItem["value"];
    bool visible = uiItem["visible"];
    QString unit = QString::fromStdString(uiItem.at("unit"));

    if (thermo->value() != value)
    {
        if (label != nullptr)
        {
            label->setText(QString("%0 %1").arg(value).arg(unit));
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
    SPDLOG_CRITICAL("Not implemented");
}

void SimulatorBase::process_ui_combobox(json& uiItem)
{
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto combobox = qobject_cast<QComboBox*>(widget);
    if (combobox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QComboBox");
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
    SPDLOG_CRITICAL("Not implemented");
}

void SimulatorBase::process_ui_button(json& uiItem)
{
    //TODO: fix bgcolor and color
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto button = qobject_cast<QPushButton*>(widget);
    if (button == nullptr)
    {
        SPDLOG_WARN("widget is not of type QPushButton");
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
        reset_widget_color(button);
        return;
    }

    if (bg_color == QColor(0xFF, 0x00, 0x00))
    {
        set_widget_color(button, QString("black"), QString("darkred"));
        return;
    }

    if (bg_color == QColor(0x00, 0xFF, 0x00))
    {
        set_widget_color(button, QString("white"), QString("darkgreen"));
        return;
    }

    if (bg_color == QColor(0xFF8800))
    {
        set_widget_color(button, QString("darkred"), QString("darkorange"));
        return;
    }

    if (bg_color != widget_bg_color(button) || fg_color != widget_color(button))
    {
        set_widget_color(button, fg_color, bg_color);
    }
}

void SimulatorBase::process_ui_led(json& uiItem)
{
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto led = qobject_cast<Led*>(widget);
    if (led == nullptr)
    {
        SPDLOG_WARN("widget is not of type led");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(uiItem["led_color"]));
    QColor color = QColor(~bg_color.red(), ~bg_color.green(), ~bg_color.blue());
    QString text = QString::fromStdString(uiItem["text"]);

    if (text != led->text())
    {
        led->setText(text);
    }

    if (bg_color == QColor(0x83, 0x91, 0x92))
    {
        reset_widget_color(led);
        return;
    }

    if (bg_color == QColor(0xFF, 0x00, 0x00))
    {
        set_widget_color(led, QString("black"), QString("darkred"));
        return;
    }

    if (bg_color == QColor(0x00, 0xFF, 0x00))
    {
        set_widget_color(led, QString("white"), QString("darkgreen"));
        return;
    }

    if (bg_color == QColor(0xFF8800))
    {
        set_widget_color(led, QString("darkred"), QString("darkorange"));
        return;
    }

    if (bg_color != widget_bg_color(led))
    {
        set_widget_color(led, color, bg_color);
    }
}

void SimulatorBase::process_ui_can(json& uiItem)
{
    QWidget* widget = id_to_ui(uiItem["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)uiItem["id"]).toStdString());
        return;
    }

    auto can_ui = qobject_cast<Can_Transceive*>(widget);
    if (can_ui == nullptr)
    {
        SPDLOG_WARN("widget is not of type led");
        return;
    }

    can_ui->clear();
    for (auto& send_item : uiItem["send_msgs"])
    {
        can_ui->add_send_item(send_item["id"], send_item["dlc"], send_item["payload"]);
    }

    for (auto& recvd_item : uiItem["rcvd_msgs"])
    {
        can_ui->add_receive_item(recvd_item["id"], recvd_item["dlc"], recvd_item["payload"]);
    }

}

void SimulatorBase::setup_ui_item(QWidget* item, size_t index)
{
    setup_button(item, index);
    setup_combobox(item, index);
    setup_dial(item, index);
    setup_slider(item, index);
    setup_can_ui(item, index);
}

void SimulatorBase::setup_button(QWidget* item, size_t index)
{
    QPushButton* button = qobject_cast<QPushButton*>(item);
    if (button == nullptr)
        return;
    connect(button, &QPushButton::clicked, this, [=, this]{m_web_socket->send_event(Web_socket_wrapper::Event::clicked, index);});
}

void SimulatorBase::setup_combobox(QWidget* item, size_t index)
{
    QComboBox* combobox = qobject_cast<QComboBox*>(item);
    if (combobox == nullptr)
        return;

    connect(combobox, &QComboBox::currentIndexChanged, this, [=, this]{
        m_web_socket->send_event(Web_socket_wrapper::Event::selected, index,
                                 combobox->currentText());});
}

void SimulatorBase::setup_dial(QWidget* item, size_t index)
{
    QwtDial* dial = qobject_cast<QwtDial*>(item);
    if (dial == nullptr)
        return;

    create_dial_needle(dial);
}

void SimulatorBase::setup_slider(QWidget* item, size_t index)
{
    QwtSlider* slider = qobject_cast<QwtSlider*>(item);
    if (slider == nullptr)
        return;

    connect(slider, &QwtSlider::sliderMoved, this, [=, this]{
        m_web_socket->send_event(Web_socket_wrapper::Event::value_changed, index,
                                 slider->value());});
}

void SimulatorBase::setup_can_ui(QWidget* item, size_t index)
{
    Can_Transceive* can = qobject_cast<Can_Transceive*>(item);
    if (can == nullptr)
        return;

    connect(can, &Can_Transceive::send_can_message, this, [=, this](uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload){
        m_web_socket->send_event(Web_socket_wrapper::Event::can_send, index, id, dlc, payload);
    });
    connect(can, &Can_Transceive::can_clear, this, [=, this]{
        m_web_socket->send_event(Web_socket_wrapper::Event::can_clear, index);
    });
}
