#include "ui_base.hpp"
#include "can_transceive.hpp"
#include "helpers.hpp"
#include "led.hpp"
#include "plot_wrapper.hpp"
#include <QMessageBox>
#include <magic_enum/magic_enum.hpp>
#include "messages_client.grpc.qpb.h"

UI_base::UI_base(RemoteUiService::Client *remote_ui_client, QWidget *parent)
    : QMainWindow{parent}, m_timer_update(new QTimer()), m_error(new QMessageBox(this)), m_remote_ui_client(remote_ui_client)
{
    this->setWindowState(Qt::WindowMaximized);
    m_ui_lookup.reserve(40);

    m_error->setIcon(QMessageBox::Critical);
    m_error->setWindowTitle("Critical");
    m_error->setText("Connection timed out\nCheck if the server is running");
    QPushButton *exit_app = m_error->addButton("Exit Application", QMessageBox::AcceptRole);
    m_error->addButton("Close", QMessageBox::RejectRole);
    connect(exit_app, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);

    m_process_lookup.insert(UI_typeGadget::UI_type::ui_label, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto label = qobject_cast<QLabel *>(widget);
        if (label == nullptr)
        {
            LOG_WARN("widget is not of type QLabel");
            return;
        }

        QString value = ui_item.label().text();
        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

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
    });

    m_process_lookup.insert(UI_typeGadget::UI_type::ui_slider, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        QLabel *label = id_to_label(id_to_ui(ui_item.id_proto()));
        auto slider = qobject_cast<QwtSlider *>(widget);
        if (slider == nullptr)
        {
            LOG_WARN("widget is not of type QwtSlider");
            return;
        }

        double value = ui_item.range().value();
        bool enable = ui_item.enabled();
        bool visible = ui_item.visible();
        QString unit = ui_item.range().unit();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_dial, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        QLabel *label = id_to_label(id_to_ui(ui_item.id_proto()));
        auto dial = qobject_cast<QwtDial *>(widget);
        if (dial == nullptr)
        {
            LOG_WARN("widget is not of type QwtDial");
            return;
        }

        double value = ui_item.range().value();
        bool visible = ui_item.visible();
        QString unit = ui_item.range().unit();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_thermo, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        QLabel *label = id_to_label(id_to_ui(ui_item.id_proto()));
        auto thermo = qobject_cast<QwtThermo *>(widget);
        if (thermo == nullptr)
        {
            LOG_WARN("widget is not of type QwtThermo");
            return;
        }

        double value = ui_item.range().value();
        bool visible = ui_item.visible();
        QString unit = ui_item.range().unit();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_textbox, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto line_edit = qobject_cast<QLineEdit *>(widget);
        if (line_edit == nullptr)
        {
            LOG_WARN("widget is not of type QLineEdit");
            return;
        }

        bool visible = ui_item.visible();
        bool enabled = ui_item.enabled();
        QString hint = ui_item.textbox().hint();
        QString text = ui_item.textbox().text();

        if (line_edit->isVisible() != visible)
        {
            line_edit->setVisible(visible);
        }

        if (line_edit->isEnabled() != enabled)
        {
            line_edit->setEnabled(enabled);
        }
        if (line_edit->placeholderText() != hint)
        {
            line_edit->setPlaceholderText(hint);
        }
        if (line_edit->text() != text)
        {
            line_edit->setText(text);
        }
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_combobox, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto combobox = qobject_cast<QComboBox *>(widget);
        if (combobox == nullptr)
        {
            LOG_WARN("widget is not of type QComboBox");
            return;
        }

        size_t selected = ui_item.combobox().selected();
        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

        if (combobox->isEnabled() != enabled)
        {
            combobox->setEnabled(enabled);
        }

        if (combobox->isVisible() != visible)
        {
            combobox->setVisible(visible);
        }

        for (const QString &item : ui_item.combobox().options())
        {
            if (combobox->findText(item) == -1)
                combobox->addItem(item);
        }

        if (combobox->currentIndex() != selected)
        {
            combobox->setCurrentIndex(selected);
        }
    });
    // m_process_lookup.insert(UI_typeGadget::UI_type::ui_radiobutton,
    //                         [=, this](const UI_item_m &ui_item, QWidget *widget) { LOG_CRITICAL("Not implemented"); });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_checkbox, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto checkbox = qobject_cast<QCheckBox *>(widget);
        if (checkbox == nullptr)
        {
            LOG_WARN("widget is not of type QCheckbox");
            return;
        }

        QColor bg_color = QColor(ui_item.checkbox().color().bgColor());
        QColor fg_color = QColor(ui_item.checkbox().color().fgColor());
        QString text = ui_item.checkbox().text();
        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();
        bool checked = ui_item.checkbox().checked();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_button, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto button = qobject_cast<QPushButton *>(widget);
        if (button == nullptr)
        {
            LOG_WARN("widget is not of type QPushButton");
            return;
        }

        QColor bg_color = QColor(ui_item.label().color().bgColor());
        QColor fg_color = QColor(ui_item.label().color().fgColor());
        QString text = ui_item.label().text();
        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_led, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto led = qobject_cast<Led *>(widget);
        if (led == nullptr)
        {
            LOG_WARN("widget is not of type led");
            return;
        }

        QColor bg_color = QColor(ui_item.label().color().bgColor());
        QColor color = QColor(ui_item.label().color().fgColor());
        QString text = ui_item.label().text();
        bool visible = ui_item.visible();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_spinbox, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto spinbox = qobject_cast<QSpinBox *>(widget);
        if (spinbox == nullptr)
        {
            LOG_WARN("widget is not of type QSpinBox");
            return;
        }

        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();
        double min = ui_item.range().min();
        double max = ui_item.range().max();
        double value = ui_item.range().value();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_double_spinbox, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto spinbox = qobject_cast<QDoubleSpinBox *>(widget);
        if (spinbox == nullptr)
        {
            LOG_WARN("widget is not of type QDoubleSpinBox");
            return;
        }

        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();
        double min = ui_item.range().min();
        double max = ui_item.range().max();
        double value = ui_item.range().value();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_hex_spinbox, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto spinbox = qobject_cast<HexSpinBox *>(widget);
        if (spinbox == nullptr)
        {
            LOG_WARN("widget is not of type HexSpinBox");
            return;
        }

        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();
        double min = ui_item.range().min();
        double max = ui_item.range().max();
        double value = ui_item.range().value();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_tab_widget, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto tab_widget = qobject_cast<QTabWidget *>(widget);
        if (tab_widget == nullptr)
        {
            LOG_WARN("widget is not of type QTabWidget");
            return;
        }

        if (tab_widget->count() != ui_item.tabWidget().tabNames().size())
        {
            LOG_ERROR("QTabWidget has not the same amount of tabs as the server");
            return;
        }

        auto tab_names = ui_item.tabWidget().tabNames();
        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

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
            if (tab_widget->tabText(i) != tab_names[i])
            {
                tab_widget->setTabText(i, tab_names[i]);
            }
            if (tab_widget->isVisible() != ui_item.tabWidget().tabVisible()[i])
            {
                tab_widget->setTabVisible(i, ui_item.tabWidget().tabVisible()[i]);
            }
        }

        if (tab_widget->currentIndex() != ui_item.tabWidget().selecteTab())
        {
            tab_widget->setCurrentIndex(ui_item.tabWidget().selecteTab());
        }
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_stacked_widget, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto stacked_widget = qobject_cast<QStackedWidget *>(widget);
        if (stacked_widget == nullptr)
        {
            LOG_WARN("widget is not of type QStackedWidget");
            return;
        }

        if (stacked_widget->count() != ui_item.stackedWidget().tabCount())
        {
            LOG_ERROR("QStackedWidget has not the same amount of tabs as the server");
            return;
        }

        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

        if (stacked_widget->isEnabled() != enabled)
        {
            stacked_widget->setEnabled(enabled);
        }

        if (stacked_widget->isVisible() != visible)
        {
            stacked_widget->setVisible(visible);
        }

        if (stacked_widget->currentIndex() != ui_item.stackedWidget().currentTab())
        {
            stacked_widget->setCurrentIndex(ui_item.stackedWidget().currentTab());
        }
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_status_bar, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto status_bar = qobject_cast<QStatusBar *>(widget);
        if (status_bar == nullptr)
        {
            LOG_WARN("widget is not of type QStatusBar");
            return;
        }

        if (status_bar->currentMessage() != ui_item.statusBar().message())
        {
            status_bar->showMessage(ui_item.statusBar().message(), ui_item.statusBar().timeout());
        }
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_plot, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto plot = qobject_cast<Plot_wrapper *>(widget);
        if (plot == nullptr)
        {
            LOG_WARN("widget is not of type QwtPlot");
            return;
        }

        QString text = ui_item.plot().text();
        QColor bg_color = QColor(ui_item.plot().color().bgColor());
        QColor color = QColor(ui_item.plot().color().fgColor());
        QString x_label = ui_item.plot().xLabel();
        QString y_label = ui_item.plot().yLabel();

        QList<double> x_vals = ui_item.plot().xVals();
        QList<double> y_vals = ui_item.plot().yVals();

        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

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
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_table, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto table = qobject_cast<QTableWidget *>(widget);
        if (table == nullptr)
        {
            LOG_WARN("widget is not of type QTableWidget");
            return;
        }

        size_t row_count = ui_item.table().rowCount();
        QList<QString> row_labels = ui_item.table().rowLabels();
        size_t column_count = ui_item.table().columnCount();
        QList<QString> column_labels = ui_item.table().columnLabels();
        QList<QString> table_data = ui_item.table().table();
        bool enabled = ui_item.enabled();
        bool visible = ui_item.visible();

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
        if (row_count != 0)
        {
            for (size_t i = 0; i < row_labels.size(); i++)
            {
                if (table->verticalHeaderItem(i) == nullptr)
                    table->setVerticalHeaderItem(i, new QTableWidgetItem());
                if (row_labels[i] != table->verticalHeaderItem(i)->text())
                    table->verticalHeaderItem(i)->setText(row_labels[i]);
            }
        }
        if (column_count != 0)
        {
            for (size_t i = 0; i < column_labels.size(); i++)
            {
                if (table->horizontalHeaderItem(i) == nullptr)
                    table->setHorizontalHeaderItem(i, new QTableWidgetItem());
                if (column_labels[i] != table->horizontalHeaderItem(i)->text())
                    table->horizontalHeaderItem(i)->setText(column_labels[i]);
            }
        }
        for (size_t i = 0; i < table_data.size(); i++)
        {
            size_t row = i / column_count;
            size_t column = i % column_count;
            if (table->item(row, column) == nullptr)
            {
                table->setItem(row, column, new QTableWidgetItem());
            }
            table->item(row, column)->setText(table_data[i]);
        }
        table->resizeColumnsToContents();
    });
    m_process_lookup.insert(UI_typeGadget::UI_type::ui_can, [=, this](const UI_item_m &ui_item, QWidget *widget) {
        auto can_ui = qobject_cast<Can_Transceive *>(widget);
        if (can_ui == nullptr)
        {
            LOG_WARN("widget is not of type led");
            return;
        }

        can_ui->clear();
        for (auto &send_item : ui_item.can().sendFrames())
        {
            can_ui->add_send_item(send_item.sid(), send_item.dlc(), send_item.payload());
        }

        for (auto &recvd_item : ui_item.can().recvFrames())
        {
            can_ui->add_receive_item(recvd_item.sid(), recvd_item.dlc(), recvd_item.payload());
        }
    });
}

void UI_base::set_name(QString name)
{
    m_name = name;
}

QWidget *UI_base::id_to_ui(size_t id)
{
    return m_ui_lookup.at(id);
}

void UI_base::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setWindowTitle(m_name);
    LOG_DEBUG("Connecting callbacks");
    m_first_load = true;

    auto reply = m_remote_ui_client->Command_get_ui_elements(Empty());
    connect(reply.get(), &QGrpcCallReply::finished, reply.get(), [this, reply = std::move(reply)](const QGrpcStatus &status) {
                if (!status.isOk())
                {
                    LOG_ERROR(QString("Command get ui elements failed: %0").arg(status.message()));
                    return;
                }
                auto response = reply->read<UI_items_m>();
                if (!response.has_value())
                {
                    qDebug("Command get ui elements deserialization failed");
                    return;
                }
                UI_item_parser(response.value());
            },
            Qt::SingleShotConnection);

    m_stream = m_remote_ui_client->SubscribeUIChanges(Empty());

    connect(
        m_stream.get(), &QGrpcServerStream::finished, this,
        [this](const QGrpcStatus &status) {
            if (!status.isOk())
                m_error->show();
        },
        Qt::SingleShotConnection);

    connect(m_stream.get(), &QGrpcServerStream::messageReceived, this, [this, stream = m_stream.get()] {
        auto response = stream->read<UI_items_m>();
        if (!response.has_value())
        {
            qDebug("Client (UnaryCall) deserialization failed");
            return;
        }
        UI_item_parser(response.value());
    });
    this->m_timer_update->start(m_refresh_rate);
    m_open = true;
}

void UI_base::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    if (m_open == true)
    {
        LOG_DEBUG("Disconnecting callbacks");
        m_timer_update->stop();
        disconnect(m_stream.get(), nullptr, nullptr, nullptr);
        m_open = false;
        this->parentWidget()->show();
    }
}

void UI_base::push_ui_item(QWidget *item)
{
    m_ui_lookup.emplace_back(item);
    setup_ui_item(item, m_ui_lookup.size() - 1);
}

void UI_base::UI_item_parser(UI_items_m &input)
{
    for (auto &ui_item : input.item())
    {
        auto widget = id_to_ui(ui_item.id_proto());
        if (widget == nullptr)
        {
            LOG_WARN(QString("id_to_ui returned null on %0").arg((size_t)ui_item.id_proto()));
            return;
        }

        m_process_lookup[ui_item.type()](
            ui_item, widget);
    }
}

void UI_base::setup_ui_item(QWidget *item, size_t index)
{
    setup_button(item, index);
    setup_combobox(item, index);
    setup_checkbox(item, index);
    setup_dial(item, index);
    setup_slider(item, index);
    setup_textbox(item, index);
    setup_spinbox(item, index);
    setup_double_spinbox(item, index);
    setup_hex_spinbox(item, index);
    setup_tab_widget(item, index);
    setup_can_ui(item, index);
}

void UI_base::setup_button(QWidget *item, size_t index)
{
    QPushButton *button = qobject_cast<QPushButton *>(item);
    if (button == nullptr)
        return;
    connect(button, &QPushButton::clicked, this, [=, this] {
        auto event = Event_m();
        event.setId_proto(index);
        event.setClicked(Clicked_m());
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_combobox(QWidget *item, size_t index)
{
    QComboBox *combobox = qobject_cast<QComboBox *>(item);
    if (combobox == nullptr)
        return;

    connect(combobox, &QComboBox::activated, this, [=, this](int combo_index) {
        auto event = Event_m();
        event.setSelected(Selected_m());
        event.setId_proto(index);
        event.selected().setSelected(combo_index);
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_checkbox(QWidget *item, size_t index)
{
    QCheckBox *checkbox = qobject_cast<QCheckBox *>(item);
    if (checkbox == nullptr)
        return;

    connect(checkbox, &QCheckBox::clicked, this, [=, this] {
        auto event = Event_m();
        event.setId_proto(index);
        event.setClicked(Clicked_m());
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_dial(QWidget *item, size_t index)
{
    auto dial = qobject_cast<QwtDial *>(item);
    if (dial == nullptr)
    {
        LOG_WARN("widget is not of type QwtDial");
        return;
    }

    create_dial_needle(dial);
}

void UI_base::setup_slider(QWidget *item, size_t index)
{
    auto slider = qobject_cast<QwtSlider *>(item);
    if (slider == nullptr)
    {
        LOG_WARN("widget is not of type QwtSlider");
        return;
    }

    connect(slider, &QwtSlider::sliderMoved, this, [=, this] (double value) {
        auto event = Event_m();
        event.setId_proto(index);
        event.setValueChanged(Value_changed_m());
        event.valueChanged().setValue(value);
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_textbox(QWidget *item, size_t index)
{
    QLineEdit *line_edit = qobject_cast<QLineEdit *>(item);
    if (line_edit == nullptr)
        return;

    connect(line_edit, &QLineEdit::textEdited, this, [=, this] {
        auto event = Event_m();
        event.setId_proto(index);
        event.setTextChanged(Text_changed_m());
        event.textChanged().setText(line_edit->text());
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_spinbox(QWidget *item, size_t index)
{
    QSpinBox *spinbox = qobject_cast<QSpinBox *>(item);
    if (spinbox == nullptr)
        return;

    connect(spinbox, &QSpinBox::valueChanged, this, [=, this](int value) {
        auto event = Event_m();
        event.setId_proto(index);
        event.setValueChanged(Value_changed_m());
        event.valueChanged().setValue(value);
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_double_spinbox(QWidget *item, size_t index)
{
    QDoubleSpinBox *spinbox = qobject_cast<QDoubleSpinBox *>(item);
    if (spinbox == nullptr)
        return;

    connect(spinbox, &QDoubleSpinBox::valueChanged, this, [=, this](double value) {
        auto event = Event_m();
        event.setId_proto(index);
        event.setValueChanged(Value_changed_m());
        event.valueChanged().setValue(value);
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_hex_spinbox(QWidget *item, size_t index)
{
    HexSpinBox *spinbox = qobject_cast<HexSpinBox *>(item);
    if (spinbox == nullptr)
        return;

    connect(spinbox, &HexSpinBox::valueChanged, this, [=, this](int value) {
        auto event = Event_m();
        event.setId_proto(index);
        event.setValueChanged(Value_changed_m());
        event.valueChanged().setValue(value);
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_tab_widget(QWidget *item, size_t index)
{
    QTabWidget *tab_widget = qobject_cast<QTabWidget *>(item);
    if (tab_widget == nullptr)
        return;

    connect(tab_widget, &QTabWidget::currentChanged, this, [=, this](int tab_index) {
        auto event = Event_m();
        event.setSelected(Selected_m());
        event.setId_proto(index);
        event.selected().setSelected(tab_index);
        (void)m_remote_ui_client->SendEvent(event);
    });
}

void UI_base::setup_can_ui(QWidget *item, size_t index)
{
    Can_Transceive *can = qobject_cast<Can_Transceive *>(item);
    if (can == nullptr)
        return;

    connect(can, &Can_Transceive::send_can_message, this,
            [=, this](uint32_t id, uint8_t dlc, QByteArray payload) {
                auto event = Event_m();
                event.setId_proto(index);
                event.setCanSend(CAN_send_m());
                event.canSend().mutFrame().setSid(id);
                event.canSend().mutFrame().setDlc(dlc);
                event.canSend().mutFrame().setPayload(payload);
                (void)m_remote_ui_client->SendEvent(event);
            });
    connect(can, &Can_Transceive::can_clear, this,
            [=, this] {
                auto event = Event_m();
                event.setId_proto(index);
                event.setClear(Clear_m());
                (void)m_remote_ui_client->SendEvent(event);
            });
}
