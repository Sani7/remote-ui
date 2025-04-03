#include "simulator_base.hpp"
#include "can_transceive.hpp"
#include "plot_wrapper.hpp"

Simulator_base::Simulator_base(QString sim_name, Web_socket_wrapper *web_socket, QWidget *parent)
    : QMainWindow{parent}, m_error_dialog(new NetworkError(this)), m_timer_update(new QTimer()),
      m_web_socket(web_socket), m_sim_name(sim_name)
{
    m_error_dialog->set_error("Connection timed out\nCheck if the server is running");
    m_ui_lookup.reserve(40);
}

QWidget *Simulator_base::id_to_ui(size_t id)
{
    return m_ui_lookup.at(id);
}

QString Simulator_base::sim_name() const
{
    return m_sim_name;
}

void Simulator_base::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    SPDLOG_DEBUG("Connecting callbacks");

    connect(m_web_socket, &Web_socket_wrapper::on_command_cb, this, [=, this](json &j) { on_cmd_cb(j); });
    connect(m_web_socket, &Web_socket_wrapper::on_event_cb, this, [=, this](json &j) { on_event_cb(j); });
    connect(m_web_socket, &Web_socket_wrapper::on_closed, this, [=, this] { m_error_dialog->open(); });

    m_web_socket->send_command(Web_socket_wrapper::Command::get_UI_elements);
    this->m_timer_update->start(m_refresh_rate);
    m_open = true;
}

void Simulator_base::closeEvent(QCloseEvent *event)
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

void Simulator_base::on_cmd_cb(json &j)
{
    auto response = magic_enum::enum_cast<Web_socket_wrapper::Command>(std::string(j.at("type")))
                        .value_or(Web_socket_wrapper::Command::end);
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

void Simulator_base::on_event_cb(json &j)
{
    auto response = magic_enum::enum_cast<Web_socket_wrapper::Event>(std::string(j.at("type")))
                        .value_or(Web_socket_wrapper::Event::end);
    switch (response)
    {
    case Web_socket_wrapper::Event::ui_changed:
        UI_item_parser(j);
        break;
    default:
        break;
    }
}

void Simulator_base::push_ui_item(QWidget *item)
{
    m_ui_lookup.emplace_back(item);
    setup_ui_item(item, m_ui_lookup.size() - 1);
}

void Simulator_base::UI_item_parser(json &input)
{
    for (auto &ui_item : input["UI_items"])
    {
        if (ui_item["type"] == "UI_button")
        {
            process_ui_button(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_led")
        {
            process_ui_led(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_slider")
        {
            process_ui_slider(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_dial")
        {
            process_ui_dial(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_thermo")
        {
            process_ui_thermo(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_textbox")
        {
            process_ui_textbox(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_label")
        {
            process_ui_label(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_combobox")
        {
            process_ui_combobox(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_RadioButton")
        {
            process_ui_radiobutton(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_checkbox")
        {
            process_ui_checkbox(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_plot")
        {
            process_ui_plot(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_table")
        {
            process_ui_table(ui_item);
            continue;
        }
        if (ui_item["type"] == "UI_can")
        {
            process_ui_can(ui_item);
            continue;
        }
    }
}

void Simulator_base::process_ui_label(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto label = qobject_cast<QLabel *>(widget);
    if (label == nullptr)
    {
        SPDLOG_WARN("widget is not of type QLabel");
        return;
    }

    QString value = QString::fromStdString(ui_item["text"]);

    if (label->text() != value)
    {
        label->setText(value);
    }
}

void Simulator_base::process_ui_slider(json &ui_item)
{
    QLabel *label = id_to_label(ui_item["id"]);
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto slider = qobject_cast<QwtSlider *>(widget);
    if (slider == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtSlider");
        return;
    }

    double value = ui_item["value"];
    bool enable = ui_item["enabled"];
    bool visible = ui_item["visible"];
    QString unit = QString::fromStdString(ui_item.at("unit"));

    if (label != nullptr)
    {
        label->setText(QString("%0 %1").arg(value).arg(unit));
    }

    if (slider->value() != value)
    {
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
    // TODO: Add color for scale
    return;
}

void Simulator_base::process_ui_dial(json &ui_item)
{
    QLabel *label = id_to_label(ui_item["id"]);
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto dial = qobject_cast<QwtDial *>(widget);
    if (dial == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtDial");
        return;
    }

    double value = ui_item["value"];
    bool visible = ui_item["visible"];
    QString unit = QString::fromStdString(ui_item.at("unit"));

    if (label != nullptr)
    {
        label->setText(QString("%0 %1").arg(value).arg(unit));
    }

    if (dial->value() != value)
    {
        dial->setValue(value);
    }

    if (dial->isVisible() != visible)
    {
        dial->setVisible(visible);
    }
    return;
}

void Simulator_base::process_ui_thermo(json &ui_item)
{
    QLabel *label = id_to_label(ui_item["id"]);
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto thermo = qobject_cast<QwtThermo *>(widget);
    if (thermo == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtThermo");
        return;
    }

    double value = ui_item["value"];
    bool visible = ui_item["visible"];
    QString unit = QString::fromStdString(ui_item.at("unit"));

    if (label != nullptr)
    {
        label->setText(QString("%0 %1").arg(value).arg(unit));
    }

    if (thermo->value() != value)
    {
        thermo->setValue(value);
    }

    if (thermo->isVisible() != visible)
    {
        thermo->setVisible(visible);
    }
    return;
}

void Simulator_base::process_ui_textbox(json &ui_item)
{
    SPDLOG_CRITICAL("Not implemented");
}

void Simulator_base::process_ui_combobox(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto combobox = qobject_cast<QComboBox *>(widget);
    if (combobox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QComboBox");
        return;
    }

    QString selected = QString::fromStdString(ui_item["selected"]);

    for (json &item : ui_item["options"])
    {
        if (combobox->findText(QString::fromStdString(item)) == -1)
            combobox->addItem(QString::fromStdString(item));
    }

    if (combobox->currentText() != selected)
    {
        combobox->setCurrentText(selected);
    }
}

void Simulator_base::process_ui_radiobutton(json &ui_item)
{
    SPDLOG_CRITICAL("Not implemented");
}

void Simulator_base::process_ui_checkbox(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto checkbox = qobject_cast<QCheckBox *>(widget);
    if (checkbox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QCheckbox");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(ui_item["bg_color"]));
    QColor fg_color = QColor(QString::fromStdString(ui_item["fg_color"]));
    QString text = QString::fromStdString(ui_item["text"]);
    bool enabled = ui_item["enabled"];
    bool checked = ui_item["checked"];

    if (checkbox->isChecked() != checked)
    {
        checkbox->setChecked(checked);
    }

    if (checkbox->text() != text)
    {
        checkbox->setText(text);
    }

    if (checkbox->isEnabled() != enabled)
    {
        checkbox->setEnabled(enabled);
    }

    if (bg_color == QColor(0x83, 0x91, 0x92))
    {
        reset_widget_color(checkbox);
        return;
    }

    if (bg_color == QColor(0xFF, 0x00, 0x00))
    {
        set_widget_color(checkbox, QString("black"), QString("darkred"));
        return;
    }

    if (bg_color == QColor(0x00, 0xFF, 0x00))
    {
        set_widget_color(checkbox, QString("white"), QString("darkgreen"));
        return;
    }

    if (bg_color == QColor(0xFF8800))
    {
        set_widget_color(checkbox, QString("darkred"), QString("darkorange"));
        return;
    }

    if (bg_color != widget_bg_color(checkbox) || fg_color != widget_color(checkbox))
    {
        set_widget_color(checkbox, fg_color, bg_color);
    }
}

void Simulator_base::process_ui_button(json &ui_item)
{
    // TODO: fix bgcolor and color
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto button = qobject_cast<QPushButton *>(widget);
    if (button == nullptr)
    {
        SPDLOG_WARN("widget is not of type QPushButton");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(ui_item["bg_color"]));
    QColor fg_color = QColor(QString::fromStdString(ui_item["fg_color"]));
    QString text = QString::fromStdString(ui_item["text"]);
    bool enabled = ui_item["enabled"];

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

void Simulator_base::process_ui_led(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto led = qobject_cast<Led *>(widget);
    if (led == nullptr)
    {
        SPDLOG_WARN("widget is not of type led");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(ui_item["bg_color"]));
    QColor color = QColor(QString::fromStdString(ui_item["fg_color"]));
    QString text = QString::fromStdString(ui_item["text"]);

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

    if (bg_color != widget_bg_color(led) || color != widget_color(led))
    {
        set_widget_color(led, color, bg_color);
    }
}

void Simulator_base::process_ui_plot(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto plot = qobject_cast<Plot_wrapper *>(widget);
    if (plot == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtPlot");
        return;
    }

    QString text = QString::fromStdString(ui_item["text"]);
    QColor bg_color = QColor(QString::fromStdString(ui_item["bg_color"]));
    QColor color = QColor(QString::fromStdString(ui_item["fg_color"]));
    QString x_label = QString::fromStdString(ui_item["x_label"]);
    QString y_label = QString::fromStdString(ui_item["y_label"]);

    std::vector<double> x_vals = (std::vector<double>)ui_item.at("x_vals");
    std::vector<double> y_vals = (std::vector<double>)ui_item.at("y_vals");

    if (text != plot->title().text())
    {
        plot->setTitle(text);
    }

    if (x_label != plot->axisTitle(QwtAxis::XBottom).text())
    {
        plot->setAxisTitle(QwtAxis::XBottom, x_label);
    }

    if (y_label != plot->axisTitle(QwtAxis::YLeft).text())
    {
        plot->setAxisTitle(QwtAxis::YLeft, y_label);
    }

    if (color != plot->curve()->pen().color())
    {
        plot->curve()->setPen(QPen(color));
    }

    if (color == QColor(0x83, 0x91, 0x92))
    {
        plot->curve()->setPen(QPen(Qt::red));
    }

    plot->curve()->setSamples(x_vals.data(), y_vals.data(), x_vals.size());
    if (x_vals.size() > 0)
        plot->setAxisScale(QwtAxis::XBottom, x_vals[0], x_vals[x_vals.size() - 1]);
    plot->replot();
}

void Simulator_base::process_ui_table(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto table = qobject_cast<QTableWidget *>(widget);
    if (table == nullptr)
    {
        SPDLOG_WARN("widget is not of type QTableWidget");
        return;
    }

    size_t row_count = ui_item["row_count"];
    std::vector<std::string> row_labels = (std::vector<std::string>)ui_item["row_labels"];
    size_t column_count = ui_item["column_count"];
    std::vector<std::string> column_labels = (std::vector<std::string>)ui_item["column_labels"];
    std::vector<bool> valid = (std::vector<bool>)ui_item["valid"];
    std::vector<std::string> table_data = (std::vector<std::string>)ui_item["table"];

    table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    if (table->rowCount() != row_count)
    {
        table->setRowCount(row_count);
    }
    if (table->columnCount() != column_count)
    {
        table->setColumnCount(column_count);
    }
    for (size_t i = 0; i < row_labels.size(); i++)
    {
        if (table->horizontalHeaderItem(i) == nullptr)
            table->setHorizontalHeaderItem(i, new QTableWidgetItem());
        if (row_labels[i] != table->horizontalHeaderItem(i)->text())
            table->horizontalHeaderItem(i)->setText(QString::fromStdString(row_labels[i]));
    }
    for (size_t i = 0; i < column_labels.size(); i++)
    {
        if (table->verticalHeaderItem(i) == nullptr)
            table->setVerticalHeaderItem(i, new QTableWidgetItem());
        if (column_labels[i] != table->verticalHeaderItem(i)->text())
            table->verticalHeaderItem(i)->setText(QString::fromStdString(column_labels[i]));
    }
    for (size_t i = 0; i < table_data.size(); i++)
    {
        size_t row = i / column_count;
        size_t column = i % column_count;
        if (valid[i])
        {
            table->setItem(row, column, new QTableWidgetItem(QString::fromStdString(table_data[i])));
        }
    }
    // table->
}

void Simulator_base::process_ui_can(json &ui_item)
{
    auto widget = id_to_ui(ui_item["id"]);
    if (widget == nullptr)
    {
        SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item["id"]).toStdString());
        return;
    }

    auto can_ui = qobject_cast<Can_Transceive *>(widget);
    if (can_ui == nullptr)
    {
        SPDLOG_WARN("widget is not of type led");
        return;
    }

    can_ui->clear();
    for (auto &send_item : ui_item["send_msgs"])
    {
        can_ui->add_send_item(send_item["id"], send_item["dlc"], send_item["payload"]);
    }

    for (auto &recvd_item : ui_item["rcvd_msgs"])
    {
        can_ui->add_receive_item(recvd_item["id"], recvd_item["dlc"], recvd_item["payload"]);
    }
}

void Simulator_base::setup_ui_item(QWidget *item, size_t index)
{
    setup_button(item, index);
    setup_combobox(item, index);
    setup_checkbox(item, index);
    setup_dial(item, index);
    setup_slider(item, index);
    setup_qwtplot(item, index);
    setup_can_ui(item, index);
}

void Simulator_base::setup_button(QWidget *item, size_t index)
{
    QPushButton *button = qobject_cast<QPushButton *>(item);
    if (button == nullptr)
        return;
    connect(button, &QPushButton::clicked, this,
            [=, this] { m_web_socket->send_event(Web_socket_wrapper::Event::clicked, index); });
}

void Simulator_base::setup_combobox(QWidget *item, size_t index)
{
    QComboBox *combobox = qobject_cast<QComboBox *>(item);
    if (combobox == nullptr)
        return;

    connect(combobox, &QComboBox::currentIndexChanged, this, [=, this] {
        m_web_socket->send_event(Web_socket_wrapper::Event::selected, index, combobox->currentText());
    });
}

void Simulator_base::setup_checkbox(QWidget *item, size_t index)
{
    QCheckBox *checkbox = qobject_cast<QCheckBox *>(item);
    if (checkbox == nullptr)
        return;

    connect(checkbox, &QCheckBox::clicked, this,
            [=, this] { m_web_socket->send_event(Web_socket_wrapper::Event::clicked, index); });
}

void Simulator_base::setup_dial(QWidget *item, size_t index)
{
    QwtDial *dial = qobject_cast<QwtDial *>(item);
    if (dial == nullptr)
        return;

    create_dial_needle(dial);
}

void Simulator_base::setup_slider(QWidget *item, size_t index)
{
    QwtSlider *slider = qobject_cast<QwtSlider *>(item);
    if (slider == nullptr)
        return;

    connect(slider, &QwtSlider::sliderMoved, this,
            [=, this] { m_web_socket->send_event(Web_socket_wrapper::Event::value_changed, index, slider->value()); });
}

void Simulator_base::setup_qwtplot(QWidget *item, size_t index)
{
    auto *plot = qobject_cast<QwtPlot *>(item);
    if (plot == nullptr)
        return;
}

void Simulator_base::setup_can_ui(QWidget *item, size_t index)
{
    Can_Transceive *can = qobject_cast<Can_Transceive *>(item);
    if (can == nullptr)
        return;

    connect(can, &Can_Transceive::send_can_message, this,
            [=, this](uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload) {
                m_web_socket->send_event(Web_socket_wrapper::Event::can_send, index, id, dlc, payload);
            });
    connect(can, &Can_Transceive::can_clear, this,
            [=, this] { m_web_socket->send_event(Web_socket_wrapper::Event::can_clear, index); });
}
