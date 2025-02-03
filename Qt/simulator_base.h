#ifndef SIMULATOR_BASE_H
#define SIMULATOR_BASE_H

#include <QMainWindow>
#include <QWidget>
#include "web_socket_wrapper.h"
#include "networkerror.h"
#include "helpers.h"
#include "led.h"
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
    QWidget* id_to_ui(size_t id);
    virtual QLabel* id_to_label(size_t name) {return nullptr;}

  signals:
    void quit();
  protected:
    void on_event_cb(json& j);
    void on_cmd_cb(json& j);

    void push_ui_item(QWidget* item);

    void button_update(size_t lookup);
    void slider_update(size_t lookup);
    void combobox_update(size_t lookup);

    void UI_item_parser(json& input);
    virtual void process_ui_label(json& uiItem);
    virtual void process_ui_slider(json& uiItem);
    virtual void process_ui_dial(json& uiItem);
    virtual void process_ui_thermo(json& uiItem);
    virtual void process_ui_textbox(json& uiItem);
    virtual void process_ui_combobox(json& uiItem);
    virtual void process_ui_radiobutton(json& uiItem);
    virtual void process_ui_button(json& uiItem);
    virtual void process_ui_led(json& uiItem);

    void setup_ui_item(QWidget* item, size_t index);
    void setup_button(QWidget* item, size_t index);
    void setup_combobox(QWidget* item, size_t index);
    void setup_dial(QWidget* item, size_t index);
    void setup_slider(QWidget* item, size_t index);

    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

  private:
    Web_socket_wrapper* m_web_socket;

    QTimer* m_timer_update;
    uint32_t m_refresh_rate;
    NetworkError* m_error_dialog;
    bool m_open = false;
    std::vector<QWidget*>m_ui_lookup;
};

#endif // SIMULATOR_BASE_H
