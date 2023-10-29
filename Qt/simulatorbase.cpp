#include "simulatorbase.h"

SimulatorBase::SimulatorBase(OpenAPI::OAICANApiApi* api, QWidget* parent) :
      QMainWindow{parent},
      error_dialog(new NetworkError(this))
{
    this->api = api;
    timer_update = new QTimer(this);
    timer_update->connect(timer_update, &QTimer::timeout, this, [=]{sim_update();});
    refresh_rate = 100;
}

void SimulatorBase::sim_update(void)
{
    api->apiCanapiCurrentsimGet();
}

void SimulatorBase::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    QD << "Starting refresh timer";
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
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsPushbuttonPostSignal, this, [this](bool output){apiCanapiUielements_bool_cb(output);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsPushbuttonPostSignalError, this, [this](bool output, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiUielements_bool_cb(false, false, error_type, error_str);});

    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsPushbuttonarrayPostSignal, this, [this](bool output){apiCanapiUielements_bool_cb(output);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsPushbuttonarrayPostSignalError, this, [this](bool output, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiUielements_bool_cb(false, false, error_type, error_str);});

    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsTextboxPostSignal, this, [this](bool output){apiCanapiUielements_bool_cb(output);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsTextboxPostSignalError, this, [this](bool output, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiUielements_bool_cb(false, false, error_type, error_str);});

    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsSliderPostSignal, this, [this](bool output){apiCanapiUielements_bool_cb(output);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsSliderPostSignalError, this, [this](bool output, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiUielements_bool_cb(false, false, error_type, error_str);});

    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsTogglearrayPostSignal, this, [this](bool output){apiCanapiUielements_bool_cb(output);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiUielementsTogglearrayPostSignalError, this, [this](bool output, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiUielements_bool_cb(false, false, error_type, error_str);});

    connect(api, &OpenAPI::OAICANApiApi::apiCanapiCurrentsimGetSignal, this, [this](OpenAPI::OAISimulatorInfo summary){apiCanapiCurrentsimGet_cb(summary);});
    connect(api, &OpenAPI::OAICANApiApi::apiCanapiCurrentsimGetSignalError, this, [this](OpenAPI::OAISimulatorInfo summary, QNetworkReply::NetworkError error_type, QString error_str){apiCanapiCurrentsimGet_cb(OpenAPI::OAISimulatorInfo(), error_type, error_str);});
}

void SimulatorBase::apiCanapiUielements_bool_cb(bool output, bool currentsim, QNetworkReply::NetworkError error_type, QString error_str)
{
    if (error_type) {
        QD << "( " << Qt::hex << output << "," << error_type << "," << error_str << ")";
        error_dialog->set_error(error_str + "\n" + network_reply_to_fix(error_type));
        error_dialog->open();
        return;
    } if (!output) {
        QD << " got called with invalid data and no error!";
        error_dialog->set_error(QString(__FUNCTION__) + " got called with invalid data and no error!");
        error_dialog->open();
        return;
    }
}

void SimulatorBase::apiCanapiCurrentsimGet_cb(OpenAPI::OAISimulatorInfo summary, QNetworkReply::NetworkError error_type, QString error_str)
{
    if (error_type) {
        QD << "( " << summary.asJson() << "," << error_type << "," << error_str << ")";
        error_dialog->set_error(error_str + "\n" + network_reply_to_fix(error_type));
        error_dialog->open();
        return;
    } if (!summary.isValid()) {
        QD << " got called with invalid data and no error!";
        error_dialog->set_error(QString(__FUNCTION__) + " got called with invalid data and no error!");
        error_dialog->open();
        return;
    }
    OAISimulatorInfo_parser(summary);
}

void SimulatorBase::OAISimulatorInfo_parser(OpenAPI::OAISimulatorInfo input)
{
    const OpenAPI::OAIClientUIItem* uiItem;

    for (size_t i = 0; i < input.getUiItems().count(); i++)
    {
        uiItem = &(input.getUiItems().at(i));

        if (uiItem->getType() == "UIButtonArray")
        {
            process_ui_button_array(uiItem);
            continue;
        }
        if (uiItem->getType() == "UILedArray")
        {
            process_ui_led_array(uiItem);
            continue;
        }
        if (uiItem->getType() == "UISlider")
        {
            process_ui_slider(uiItem);
            continue;
        }
        if (uiItem->getType() == "UITextbox")
        {
            process_ui_textbox(uiItem);
            continue;
        }
        if (uiItem->getType() == "UILabel")
        {
            process_ui_label(uiItem);
            continue;
        }
        if (uiItem->getType() == "UICombobox")
        {
            process_ui_combobox(uiItem);
            continue;
        }
        if (uiItem->getType() == "UIRadioButton")
        {
            process_ui_radiobutton(uiItem);
            continue;
        }
    }
}

void SimulatorBase::process_ui_button_array(const OpenAPI::OAIClientUIItem* uiItem)
{
    QJsonObject state = uiItem->getState().asJsonObject();
    QJsonArray state_array = state["buttonArray"].toArray();
    for (size_t i = 0; i < state_array.count(); i++)
    {
        process_ui_button_array_element(state_array[i].toObject());
    }
}

void SimulatorBase::process_ui_led_array(const OpenAPI::OAIClientUIItem* uiItem)
{
    QJsonObject state = uiItem->getState().asJsonObject();
    QJsonArray state_array = state["ledArray"].toArray();
    for (size_t i = 0; i < state_array.count(); i++)
    {
        process_ui_led_array_element(state_array[i].toObject());
    }
}

void SimulatorBase::process_ui_label(const OpenAPI::OAIClientUIItem* uiItem)
{
    QLabel* label = id_to_label(uiItem->getName());
    if (label == nullptr)
    {
        QD << "id_to_label returned null on " << uiItem->getName();
        return;
    }

    QString value = uiItem->getState().asJsonObject()["textString"].toString();

    if (label->text() != value)
    {
        label->setText(value);
    }
}

void SimulatorBase::process_ui_slider(const OpenAPI::OAIClientUIItem* uiItem)
{
    if (uiItem->getName().startsWith('t'))
    {
        QwtSlider* slider = id_to_slider(uiItem->getName());
        QLabel* label = id_to_slider_label(uiItem->getName());
        if (slider == nullptr)
        {
            QD << "id_to_slider returned null on " << uiItem->getName();
            return;
        }

        double value = uiItem->getState().asJsonObject()["currentValue"].toDouble();
        bool enable = uiItem->getState().asJsonObject()["enable"].toBool();
        bool visible = uiItem->getState().asJsonObject()["visible"].toBool();

        if (slider->value() != value)
        {
            if (label != nullptr)
            {
                label->setText(format_slider_value(uiItem->getName(), value));
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

    if (uiItem->getName().startsWith('d'))
    {
        QwtDial* dial = id_to_dial(uiItem->getName());
        QLabel* label = id_to_dial_label(uiItem->getName());
        if (dial == nullptr)
        {
            QD << "id_to_dial returned null on " << uiItem->getName();
            return;
        }

        double value = uiItem->getState().asJsonObject()["currentValue"].toDouble();
        bool visible = uiItem->getState().asJsonObject()["visible"].toBool();

        if (dial->value() != value)
        {
            if (label != nullptr)
            {
                label->setText(format_dial_value(uiItem->getName(), value));
            }
            dial->setValue(value);
        }

        if (dial->isVisible() != visible)
        {
            dial->setVisible(visible);
        }
        return;
    }
}

void SimulatorBase::process_ui_textbox(const OpenAPI::OAIClientUIItem* uiItem)
{
    QD << "Not implemented";
    emit quit();
}

void SimulatorBase::process_ui_combobox(const OpenAPI::OAIClientUIItem* uiItem)
{
    QComboBox* combobox = id_to_combobox(uiItem->getName());
    if (combobox == nullptr)
    {
        QD << "id_to_combobox returned null on " << uiItem->getName();
        return;
    }

    QJsonObject state = uiItem->getState().asJsonObject();
    QString selected = state["selected"].toString();
    QJsonArray toggleArray = state["toggleArray"].toArray();


    for (size_t i = 0; i < toggleArray.count(); i++)
    {
        QString item = toggleArray[i].toObject()["id"].toString();
        if (combobox->findText(item) == -1)
            combobox->addItem(item);
    }

    if (combobox->currentText() != selected)
    {
        combobox->setCurrentText(selected);
    }
}

void SimulatorBase::process_ui_radiobutton(const OpenAPI::OAIClientUIItem* uiItem)
{
    QD << "Not implemented";
    emit quit();
}

void SimulatorBase::process_ui_button_array_element(QJsonObject jsonObject)
{
    //TODO: fix bgcolor and color
    QPushButton* button = id_to_button(jsonObject["id"].toString());

    if (button == nullptr)
    {
        QD << "id_to_button returned null on " << jsonObject["id"].toString();
        return;
    }

    QColor bg_color = QColor(jsonObject["color"].toString());
    QString text = jsonObject["title"].toString();
    bool enabled = jsonObject["enable"].toBool();

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

    if (bg_color != button_bg_color(button))
    {
        set_button_color(button, QColor(), bg_color);
    }
}

void SimulatorBase::process_ui_led_array_element(QJsonObject jsonObject)
{
    QPushButton* led = id_to_led(jsonObject["id"].toString());
    if (led == nullptr)
    {
        QD << "id_to_led returned null on " << jsonObject["id"].toString();
        return;
    }

    QColor bg_color = QColor(jsonObject["state"].toString());
    QColor color = QColor(~bg_color.red(), ~bg_color.green(), ~bg_color.blue());
    QString text = jsonObject["title"].toString();

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
