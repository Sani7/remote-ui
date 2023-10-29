#ifndef SIMULATORBASE_H
#define SIMULATORBASE_H

#include <QMainWindow>
#include <QWidget>
#include "OAICANApiApi.h"
#include "networkerror.h"
#include "helpers.h"
#include <magic_enum.hpp>
#include "IterableEnum.hpp"

#define QD qDebug() << "Debug:" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ":"

class SimulatorBase : public QMainWindow {
    Q_OBJECT
  public:
    explicit SimulatorBase(OpenAPI::OAICANApiApi* api, QWidget* parent = nullptr);
    void setup_cb(void);
    void sim_update(void);
    virtual QLabel* id_to_label(QString name) {return nullptr;}
    virtual QPushButton* id_to_led(QString name) {return nullptr;}
    virtual QPushButton* id_to_button(QString name) {return nullptr;}
    virtual QwtSlider* id_to_slider(QString name) {return nullptr;}
    virtual QLabel* id_to_slider_label(QString name) {return nullptr;}
    virtual QString format_slider_value(QString name, double val) {return QString();}
    virtual QwtDial* id_to_dial(QString name) {return nullptr;}
    virtual QLabel* id_to_dial_label(QString name) {return nullptr;}
    virtual QString format_dial_value(QString name, double val) {return QString();}
    virtual QComboBox* id_to_combobox(QString name) {return nullptr;}
    virtual QTextEdit* id_to_textbox(QString name) {return nullptr;}

  signals:
    void quit();
  protected:
    void apiCanapiUielements_bool_cb(bool ret, bool currentsim = false, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());
    void apiCanapiCurrentsimGet_cb(OpenAPI::OAISimulatorInfo output, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());

    void OAISimulatorInfo_parser(OpenAPI::OAISimulatorInfo input);
    void process_ui_button_array(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_led_array(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_label(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_slider(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_textbox(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_combobox(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_radiobutton(const OpenAPI::OAIClientUIItem* uiItem);
    void process_ui_button_array_element(QJsonObject jsonObject);
    void process_ui_led_array_element(QJsonObject jsonObject);

    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

    OpenAPI::OAICANApiApi* api;

    QTimer* timer_update;
    uint32_t refresh_rate;
    NetworkError* error_dialog;
    bool closed = false;
};

#endif // SIMULATORBASE_H
