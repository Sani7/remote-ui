#include "simulator_base.hpp"
#include "can_transceive.hpp"
#include "helpers.hpp"
#include "led.hpp"
#include "networkerror.hpp"
#include "plot_wrapper.hpp"
#include "web_socket_wrapper.hpp"
#include <magic_enum/magic_enum.hpp>

Simulator_base::Simulator_base(QString name, Web_socket_wrapper *web_socket, QWidget *parent)
    : QMainWindow{parent}, m_error_dialog(new NetworkError(this)), m_timer_update(new QTimer()),
      m_web_socket(web_socket), m_name(name)
{
    if (windowTitle() != m_name)
    {
        setWindowTitle(m_name);
    }
    this->setWindowState(Qt::WindowMaximized);
    m_error_dialog->set_error("Connection timed out\nCheck if the server is running");
    m_ui_lookup.reserve(40);
}

QWidget *Simulator_base::id_to_ui(size_t id)
{
    return m_ui_lookup.at(id);
}

QString Simulator_base::name() const
{
    return m_name;
}

void Simulator_base::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    SPDLOG_DEBUG("Connecting callbacks");
    m_first_load = true;

    connect(m_web_socket, &Web_socket_wrapper::on_command_cb, this, [=, this](json &j) { on_cmd_cb(j); });
    connect(m_web_socket, &Web_socket_wrapper::on_event_cb, this, [=, this](json &j) { on_event_cb(j); });
    connect(m_web_socket, &Web_socket_wrapper::on_closed, this, [=, this] { m_error_dialog->open(); });

    m_web_socket->inhibit_events(true);
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
    if (m_first_load)
    {
        m_web_socket->inhibit_events(false);
        m_first_load = false;
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
    for (auto &ui_item : input.at("UI_items"))
    {
        auto widget = id_to_ui(ui_item.at("id"));
        if (widget == nullptr)
        {
            SPDLOG_WARN("id_to_ui returned null on {}", QString::number((size_t)ui_item.at("id")).toStdString());
            return;
        }

        if (ui_item.at("type") == "ui_button")
        {
            process_ui_button(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_led")
        {
            process_ui_led(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_slider")
        {
            process_ui_slider(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_dial")
        {
            process_ui_dial(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_thermo")
        {
            process_ui_thermo(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_textbox")
        {
            process_ui_textbox(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_label")
        {
            process_ui_label(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_combobox")
        {
            process_ui_combobox(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_RadioButton")
        {
            process_ui_radiobutton(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_checkbox")
        {
            process_ui_checkbox(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_spinbox")
        {
            process_ui_spinbox(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_double_spinbox")
        {
            process_ui_double_spinbox(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_hex_spinbox")
        {
            process_ui_hex_spinbox(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_tab_widget")
        {
            process_ui_tab_widget(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_stacked_widget")
        {
            process_ui_stacked_widget(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_status_bar")
        {
            process_ui_status_bar(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_plot")
        {
            process_ui_plot(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_table")
        {
            process_ui_table(ui_item, widget);
            continue;
        }
        if (ui_item.at("type") == "ui_can")
        {
            process_ui_can(ui_item, widget);
            continue;
        }
    }
}

void Simulator_base::process_ui_label(json &ui_item, QWidget *widget)
{
    auto label = qobject_cast<QLabel *>(widget);
    if (label == nullptr)
    {
        SPDLOG_WARN("widget is not of type QLabel");
        return;
    }

    QString value = QString::fromStdString(ui_item.at("text"));
    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (label->isEnabled() != enabled)
    {
        label->setEnabled(enabled);
    }

    if (label->isVisible() != visible)
    {
        label->setVisible(visible);
    }

    if (label->text() != value)
    {
        label->setText(value);
    }
}

void Simulator_base::process_ui_slider(json &ui_item, QWidget *widget)
{
    QLabel *label = id_to_label(ui_item.at("id"));

    auto slider = qobject_cast<QwtSlider *>(widget);
    if (slider == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtSlider");
        return;
    }

    double value = ui_item.at("value");
    bool enable = ui_item.at("enabled");
    bool visible = ui_item.at("visible");
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

void Simulator_base::process_ui_dial(json &ui_item, QWidget *widget)
{
    QLabel *label = id_to_label(ui_item.at("id"));

    auto dial = qobject_cast<QwtDial *>(widget);
    if (dial == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtDial");
        return;
    }

    double value = ui_item.at("value");
    bool visible = ui_item.at("visible");
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

void Simulator_base::process_ui_thermo(json &ui_item, QWidget *widget)
{
    QLabel *label = id_to_label(ui_item.at("id"));

    auto thermo = qobject_cast<QwtThermo *>(widget);
    if (thermo == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtThermo");
        return;
    }

    double value = ui_item.at("value");
    bool visible = ui_item.at("visible");
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

void Simulator_base::process_ui_textbox(json &ui_item, QWidget *widget)
{
    SPDLOG_CRITICAL("Not implemented");
}

void Simulator_base::process_ui_combobox(json &ui_item, QWidget *widget)
{
    auto combobox = qobject_cast<QComboBox *>(widget);
    if (combobox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QComboBox");
        return;
    }

    size_t selected = ui_item.at("selected");
    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (combobox->isEnabled() != enabled)
    {
        combobox->setEnabled(enabled);
    }

    if (combobox->isVisible() != visible)
    {
        combobox->setVisible(visible);
    }

    for (json &item : ui_item.at("options"))
    {
        if (combobox->findText(QString::fromStdString(item)) == -1)
            combobox->addItem(QString::fromStdString(item));
    }

    if (combobox->currentIndex() != selected)
    {
        combobox->setCurrentIndex(selected);
    }
}

void Simulator_base::process_ui_radiobutton(json &ui_item, QWidget *widget)
{
    SPDLOG_CRITICAL("Not implemented");
}

void Simulator_base::process_ui_checkbox(json &ui_item, QWidget *widget)
{
    auto checkbox = qobject_cast<QCheckBox *>(widget);
    if (checkbox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QCheckbox");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(ui_item.at("bg_color")));
    QColor fg_color = QColor(QString::fromStdString(ui_item.at("fg_color")));
    QString text = QString::fromStdString(ui_item.at("text"));
    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");
    bool checked = ui_item.at("checked");

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

    if (checkbox->isVisible() != visible)
    {
        checkbox->setVisible(visible);
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

void Simulator_base::process_ui_button(json &ui_item, QWidget *widget)
{
    auto button = qobject_cast<QPushButton *>(widget);
    if (button == nullptr)
    {
        SPDLOG_WARN("widget is not of type QPushButton");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(ui_item.at("bg_color")));
    QColor fg_color = QColor(QString::fromStdString(ui_item.at("fg_color")));
    QString text = QString::fromStdString(ui_item.at("text"));
    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (button->isEnabled() != enabled)
    {
        button->setEnabled(enabled);
    }

    if (button->isVisible() != visible)
    {
        button->setVisible(visible);
    }

    if (button->text() != text)
    {
        button->setText(text);
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

void Simulator_base::process_ui_led(json &ui_item, QWidget *widget)
{
    auto led = qobject_cast<Led *>(widget);
    if (led == nullptr)
    {
        SPDLOG_WARN("widget is not of type led");
        return;
    }

    QColor bg_color = QColor(QString::fromStdString(ui_item.at("bg_color")));
    QColor color = QColor(QString::fromStdString(ui_item.at("fg_color")));
    QString text = QString::fromStdString(ui_item.at("text"));
    bool visible = ui_item.at("visible");

    if (led->isVisible() != visible)
    {
        led->setVisible(visible);
    }

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

void Simulator_base::process_ui_spinbox(json &ui_item, QWidget *widget)
{
    auto spinbox = qobject_cast<QSpinBox *>(widget);
    if (spinbox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QSpinBox");
        return;
    }

    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");
    double min = ui_item.at("min");
    double max = ui_item.at("max");
    double value = ui_item.at("value");

    if (enabled != spinbox->isEnabled())
    {
        spinbox->setEnabled(enabled);
    }

    if (visible != spinbox->isVisible())
    {
        spinbox->setVisible(visible);
    }

    if (min != spinbox->minimum())
    {
        spinbox->setMinimum(min);
    }
    if (max != spinbox->maximum())
    {
        spinbox->setMaximum(max);
    }
    if (value != spinbox->value())
    {
        spinbox->setValue((int)value);
    }
}

void Simulator_base::process_ui_double_spinbox(json &ui_item, QWidget *widget)
{
    auto spinbox = qobject_cast<QDoubleSpinBox *>(widget);
    if (spinbox == nullptr)
    {
        SPDLOG_WARN("widget is not of type QDoubleSpinBox");
        return;
    }

    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");
    double min = ui_item.at("min");
    double max = ui_item.at("max");
    double value = ui_item.at("value");

    if (enabled != spinbox->isEnabled())
    {
        spinbox->setEnabled(enabled);
    }

    if (visible != spinbox->isVisible())
    {
        spinbox->setVisible(visible);
    }

    if (min != spinbox->minimum())
    {
        spinbox->setMinimum(min);
    }
    if (max != spinbox->maximum())
    {
        spinbox->setMaximum(max);
    }
    if (value != spinbox->value())
    {
        spinbox->setValue(value);
    }
}

void Simulator_base::process_ui_hex_spinbox(json &ui_item, QWidget *widget)
{
    auto spinbox = qobject_cast<HexSpinBox *>(widget);
    if (spinbox == nullptr)
    {
        SPDLOG_WARN("widget is not of type HexSpinBox");
        return;
    }

    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");
    double min = ui_item.at("min");
    double max = ui_item.at("max");
    double value = ui_item.at("value");

    if (enabled != spinbox->isEnabled())
    {
        spinbox->setEnabled(enabled);
    }

    if (visible != spinbox->isVisible())
    {
        spinbox->setVisible(visible);
    }

    if (min != spinbox->minimum())
    {
        spinbox->setMinimum(min);
    }
    if (max != spinbox->maximum())
    {
        spinbox->setMaximum(max);
    }
    if (value != spinbox->value())
    {
        spinbox->setValue((int)value);
    }
}

void Simulator_base::process_ui_tab_widget(json &ui_item, QWidget *widget)
{
    auto tab_widget = qobject_cast<QTabWidget *>(widget);
    if (tab_widget == nullptr)
    {
        SPDLOG_WARN("widget is not of type QTabWidget");
        return;
    }

    if (tab_widget->count() != ui_item.at("tab_names").size())
    {
        SPDLOG_ERROR("QTabWidget has not the same amount of tabs as the server");
        return;
    }

    auto tab_names = ui_item.at("tab_names");
    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (tab_widget->isEnabled() != enabled)
    {
        tab_widget->setEnabled(enabled);
    }

    if (tab_widget->isVisible() != visible)
    {
        tab_widget->setVisible(visible);
    }

    for (size_t i = 0; i < tab_names.size(); i++)
    {
        if (tab_widget->tabText(i) != QString::fromStdString(tab_names[i]))
        {
            tab_widget->setTabText(i, QString::fromStdString(tab_names[i]));
        }
        if (tab_widget->isVisible() != ui_item.at("tab_visible")[i])
        {
            tab_widget->setTabVisible(i, ui_item.at("tab_visible")[i]);
        }
    }

    if (tab_widget->currentIndex() != ui_item.at("selected"))
    {
        tab_widget->setCurrentIndex(ui_item.at("selected"));
    }
}

void Simulator_base::process_ui_stacked_widget(json &ui_item, QWidget *widget)
{
    auto stacked_widget = qobject_cast<QStackedWidget *>(widget);
    if (stacked_widget == nullptr)
    {
        SPDLOG_WARN("widget is not of type QStackedWidget");
        return;
    }

    if (stacked_widget->count() != ui_item.at("tab_count"))
    {
        SPDLOG_ERROR("QStackedWidget has not the same amount of tabs as the server");
        return;
    }

    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (stacked_widget->isEnabled() != enabled)
    {
        stacked_widget->setEnabled(enabled);
    }

    if (stacked_widget->isVisible() != visible)
    {
        stacked_widget->setVisible(visible);
    }

    if (stacked_widget->currentIndex() != ui_item.at("current_tab"))
    {
        stacked_widget->setCurrentIndex(ui_item.at("current_tab"));
    }
}

void Simulator_base::process_ui_status_bar(json &ui_item, QWidget *widget)
{
    auto status_bar = qobject_cast<QStatusBar *>(widget);
    if (status_bar == nullptr)
    {
        SPDLOG_WARN("widget is not of type QStatusBar");
        return;
    }

    if (status_bar->currentMessage() != QString::fromStdString(ui_item.at("current_message")))
    {
        status_bar->showMessage(QString::fromStdString(ui_item.at("current_message")), ui_item.at("timeout"));
    }
}

void Simulator_base::process_ui_plot(json &ui_item, QWidget *widget)
{
    auto plot = qobject_cast<Plot_wrapper *>(widget);
    if (plot == nullptr)
    {
        SPDLOG_WARN("widget is not of type QwtPlot");
        return;
    }

    QString text = QString::fromStdString(ui_item.at("text"));
    QColor bg_color = QColor(QString::fromStdString(ui_item.at("bg_color")));
    QColor color = QColor(QString::fromStdString(ui_item.at("fg_color")));
    QString x_label = QString::fromStdString(ui_item.at("x_label"));
    QString y_label = QString::fromStdString(ui_item.at("y_label"));

    std::vector<double> x_vals = (std::vector<double>)ui_item.at("x_vals");
    std::vector<double> y_vals = (std::vector<double>)ui_item.at("y_vals");

    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (plot->isEnabled() != enabled)
    {
        plot->setEnabled(enabled);
    }

    if (plot->isVisible() != visible)
    {
        plot->setVisible(visible);
    }

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

void Simulator_base::process_ui_table(json &ui_item, QWidget *widget)
{
    auto table = qobject_cast<QTableWidget *>(widget);
    if (table == nullptr)
    {
        SPDLOG_WARN("widget is not of type QTableWidget");
        return;
    }

    size_t row_count = ui_item.at("row_count");
    std::vector<std::string> row_labels = (std::vector<std::string>)ui_item.at("row_labels");
    size_t column_count = ui_item.at("column_count");
    std::vector<std::string> column_labels = (std::vector<std::string>)ui_item.at("column_labels");
    std::vector<bool> valid = (std::vector<bool>)ui_item.at("valid");
    std::vector<std::string> table_data = (std::vector<std::string>)ui_item.at("table");
    bool enabled = ui_item.at("enabled");
    bool visible = ui_item.at("visible");

    if (table->isEnabled() != enabled)
    {
        table->setEnabled(enabled);
    }

    if (table->isVisible() != visible)
    {
        table->setVisible(visible);
    }

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
        if (valid[i] && table->item(row, column) == nullptr)
        {
            table->setItem(row, column, new QTableWidgetItem());
        }
        if (valid[i])
        {
            table->item(row, column)->setText(QString::fromStdString(table_data[i]));
        }
    }
    // table->
}

void Simulator_base::process_ui_can(json &ui_item, QWidget *widget)
{
    auto can_ui = qobject_cast<Can_Transceive *>(widget);
    if (can_ui == nullptr)
    {
        SPDLOG_WARN("widget is not of type led");
        return;
    }

    can_ui->clear();
    for (auto &send_item : ui_item.at("send_msgs"))
    {
        can_ui->add_send_item(send_item.at("id"), send_item.at("dlc"), send_item.at("payload"));
    }

    for (auto &recvd_item : ui_item.at("rcvd_msgs"))
    {
        can_ui->add_receive_item(recvd_item.at("id"), recvd_item.at("dlc"), recvd_item.at("payload"));
    }
}

void Simulator_base::setup_ui_item(QWidget *item, size_t index)
{
    setup_button(item, index);
    setup_combobox(item, index);
    setup_checkbox(item, index);
    setup_dial(item, index);
    setup_slider(item, index);
    setup_spinbox(item, index);
    setup_double_spinbox(item, index);
    setup_hex_spinbox(item, index);
    setup_tab_widget(item, index);
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

    connect(combobox, &QComboBox::activated, this, [=, this] {
        m_web_socket->send_event(Web_socket_wrapper::Event::selected, index, (size_t)combobox->currentIndex());
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

void Simulator_base::setup_spinbox(QWidget *item, size_t index)
{
    QSpinBox *spinbox = qobject_cast<QSpinBox *>(item);
    if (spinbox == nullptr)
        return;

    connect(spinbox, &QSpinBox::valueChanged, this, [=, this](int value) {
        m_web_socket->send_event(Web_socket_wrapper::Event::value_changed, index, (double)value);
    });
}

void Simulator_base::setup_double_spinbox(QWidget *item, size_t index)
{
    QDoubleSpinBox *spinbox = qobject_cast<QDoubleSpinBox *>(item);
    if (spinbox == nullptr)
        return;

    connect(spinbox, &QDoubleSpinBox::valueChanged, this, [=, this](double value) {
        m_web_socket->send_event(Web_socket_wrapper::Event::value_changed, index, value);
    });
}

void Simulator_base::setup_hex_spinbox(QWidget *item, size_t index)
{
    HexSpinBox *spinbox = qobject_cast<HexSpinBox *>(item);
    if (spinbox == nullptr)
        return;

    connect(spinbox, &HexSpinBox::valueChanged, this, [=, this](int value) {
        m_web_socket->send_event(Web_socket_wrapper::Event::value_changed, index, (double)value);
    });
}

void Simulator_base::setup_tab_widget(QWidget *item, size_t index)
{
    QTabWidget *tab_widget = qobject_cast<QTabWidget *>(item);
    if (tab_widget == nullptr)
        return;

    connect(tab_widget, &QTabWidget::currentChanged, this, [=, this](int index) {
        m_web_socket->send_event(Web_socket_wrapper::Event::selected, index, (size_t)index);
    });
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
