#pragma once
#include "spdlog/spdlog.h"
#include <QMainWindow>
#include <QWidget>
#include <nlohmann/json.hpp>

Q_FORWARD_DECLARE_OBJC_CLASS(Web_socket_wrapper);
Q_FORWARD_DECLARE_OBJC_CLASS(QTimer);
Q_FORWARD_DECLARE_OBJC_CLASS(QLabel);
Q_FORWARD_DECLARE_OBJC_CLASS(QMessageBox);

using json = nlohmann::json;

#define PUSH_UI_ITEM(item) push_ui_item(ui->item)
#define EXPORT_UI(client)                                                                                              \
    extern "C" Q_DECL_EXPORT UI_base *get_ui(Web_socket_wrapper *api, QWidget *parent)                                 \
    {                                                                                                                  \
        return new client(api, parent);                                                                                \
    }

class UI_base : public QMainWindow
{
    Q_OBJECT
  public:
    explicit UI_base(QString name, Web_socket_wrapper *api, QWidget *parent = nullptr);
    void sim_update(void);
    QString name() const;
    QWidget *id_to_ui(size_t id);
    virtual QLabel *id_to_label(QWidget *widget)
    {
        Q_UNUSED(widget);
        return nullptr;
    }

  signals:
    void quit();

  protected:
    void on_event_cb(json &j);
    void on_cmd_cb(json &j);

    void push_ui_item(QWidget *item);

    void UI_item_parser(json &input);
    virtual void process_ui_label(json &ui_item, QWidget *widget);
    virtual void process_ui_slider(json &ui_item, QWidget *widget);
    virtual void process_ui_dial(json &ui_item, QWidget *widget);
    virtual void process_ui_thermo(json &ui_item, QWidget *widget);
    virtual void process_ui_textbox(json &ui_item, QWidget *widget);
    virtual void process_ui_combobox(json &ui_item, QWidget *widget);
    virtual void process_ui_radiobutton(json &ui_item, QWidget *widget);
    virtual void process_ui_checkbox(json &ui_item, QWidget *widget);
    virtual void process_ui_button(json &ui_item, QWidget *widget);
    virtual void process_ui_led(json &ui_item, QWidget *widget);
    virtual void process_ui_spinbox(json &ui_item, QWidget *widget);
    virtual void process_ui_double_spinbox(json &ui_item, QWidget *widget);
    virtual void process_ui_hex_spinbox(json &ui_item, QWidget *widget);
    virtual void process_ui_tab_widget(json &ui_item, QWidget *widget);
    virtual void process_ui_stacked_widget(json &ui_item, QWidget *widget);
    virtual void process_ui_status_bar(json &ui_item, QWidget *widget);
    virtual void process_ui_plot(json &ui_item, QWidget *widget);
    virtual void process_ui_table(json &ui_item, QWidget *widget);
    virtual void process_ui_can(json &ui_item, QWidget *widget);

    void setup_ui_item(QWidget *item, size_t index);
    void setup_button(QWidget *item, size_t index);
    void setup_combobox(QWidget *item, size_t index);
    void setup_checkbox(QWidget *item, size_t index);
    void setup_dial(QWidget *item, size_t index);
    void setup_slider(QWidget *item, size_t index);
    void setup_spinbox(QWidget *item, size_t index);
    void setup_double_spinbox(QWidget *item, size_t index);
    void setup_hex_spinbox(QWidget *item, size_t index);
    void setup_tab_widget(QWidget *item, size_t index);
    void setup_can_ui(QWidget *item, size_t index);

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

  private:
    Web_socket_wrapper *m_web_socket;
    QString m_name;

    QTimer *m_timer_update;
    uint32_t m_refresh_rate;
    QMessageBox *m_error;
    bool m_open = false;
    bool m_first_load = true;
    std::vector<QWidget *> m_ui_lookup;
};
