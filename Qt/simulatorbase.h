#ifndef SIMULATORBASE_H
#define SIMULATORBASE_H

#include <QMainWindow>
#include <QWidget>
#include "web_socket_wrapper.h"
#include "networkerror.h"
#include "helpers.h"
#include <magic_enum.hpp>
#include "IterableEnum.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define QD qDebug() << "Debug:" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ":"

class SimulatorBase : public QMainWindow {
    Q_OBJECT
  public:
    explicit SimulatorBase(Web_socket_wrapper* api, QWidget* parent = nullptr);
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
    void on_event_cb(json& j);
    void on_cmd_cb(json& j);
    void UI_item_parser(json& input);
    virtual void process_ui_label(json& uiItem);
    virtual void process_ui_slider(json& uiItem);
    virtual void process_ui_textbox(json& uiItem);
    virtual void process_ui_combobox(json& uiItem);
    virtual void process_ui_radiobutton(json& uiItem);
    virtual void process_ui_button(json& uiItem);
    virtual void process_ui_led(json& uiItem);

    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

    Web_socket_wrapper* m_web_socket;

    QTimer* timer_update;
    uint32_t refresh_rate;
    NetworkError* error_dialog;
    bool closed = false;
};

#endif // SIMULATORBASE_H
