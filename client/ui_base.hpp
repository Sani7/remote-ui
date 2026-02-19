#pragma once
#include <QMainWindow>
#include <QWidget>
#include <nlohmann/json.hpp>

#ifdef INTERNAL
#define EXPORT Q_DECL_EXPORT
#else
#define EXPORT Q_DECL_IMPORT
#endif

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

#define LOG_SRC_LOC(level, msg) emit log_signal(__FILE__, __LINE__, __FUNCTION__, (int)level, msg)
#define LOG_TRACE(msg) LOG_SRC_LOC(Log_level_enum::trace, msg)
#define LOG_DEBUG(msg) LOG_SRC_LOC(Log_level_enum::debug, msg)
#define LOG_INFO(msg) LOG_SRC_LOC(Log_level_enum::info, msg)
#define LOG_WARN(msg) LOG_SRC_LOC(Log_level_enum::warn, msg)
#define LOG_ERROR(msg) LOG_SRC_LOC(Log_level_enum::err, msg)
#define LOG_CRITICAL(msg) LOG_SRC_LOC(Log_level_enum::critical, msg)

/**
 * @brief Base class for client UIs
 */
class EXPORT UI_base : public QMainWindow
{
    Q_OBJECT
  public:
    enum class Log_level_enum : int
    {
        trace,
        debug,
        info,
        warn,
        err,
        critical,
        off,
        n_levels
    };
    /**
     * @brief Construct a new ui base object
     *
     * @param name The name of the simulator
     * @param api The weboscket connection to the server API
     * @param parent The parent widget
     */
    explicit UI_base(Web_socket_wrapper *api, QWidget *parent = nullptr);
    void set_name(QString name);
    /**
     * @brief Get the UI element by its ID
     *
     * @param id The ID of the UI element
     * @return QWidget* The UI element
     */
    QWidget *id_to_ui(size_t id);
    /**
     * @brief Get the label associated with a widget
     *        This function has to be overridden in derived classes if labels are used
     *
     * @param widget The widget
     * @return QLabel* The associated label, or nullptr if none
     */
    virtual QLabel *id_to_label(QWidget *widget)
    {
        Q_UNUSED(widget);
        return nullptr;
    }

  protected:
    /**
     * @brief Handle an event callback from the server
     *
     * @param j The JSON event
     */
    void on_event_cb(json &j);
    /**
     * @brief Handle a command callback from the server
     *
     * @param j The JSON command
     */
    void on_cmd_cb(json &j);

    /**
     * @brief Push a UI item to the internal list and setup its connections
     *        The item have to be in the same order as from the server
     *
     * @param item The UI item to push
     */
    void push_ui_item(QWidget *item);

    /**
     * @brief Parse UI items from JSON and update the UI accordingly
     *
     * @param input The JSON input containing UI items
     */
    void UI_item_parser(json &input);
    /**
     * @brief Process a label UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_label(json &ui_item, QWidget *widget);
    /**
     * @brief Process a slider UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_slider(json &ui_item, QWidget *widget);
    /**
     * @brief Process a dial UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_dial(json &ui_item, QWidget *widget);
    /**
     * @brief Process a thermo UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_thermo(json &ui_item, QWidget *widget);
    /**
     * @brief Process a textbox UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_textbox(json &ui_item, QWidget *widget);
    /**
     * @brief Process a combobox UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_combobox(json &ui_item, QWidget *widget);
    /**
     * @brief Process a radiobutton UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_radiobutton(json &ui_item, QWidget *widget);
    /**
     * @brief Process a checkbox UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_checkbox(json &ui_item, QWidget *widget);
    /**
     * @brief Process a button UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_button(json &ui_item, QWidget *widget);
    /**
     * @brief Process a LED UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_led(json &ui_item, QWidget *widget);
    /**
     * @brief Process a spinbox UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_spinbox(json &ui_item, QWidget *widget);
    /**
     * @brief Process a double spinbox UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_double_spinbox(json &ui_item, QWidget *widget);
    /**
     * @brief Process a hex spinbox UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_hex_spinbox(json &ui_item, QWidget *widget);
    /**
     * @brief Process a tab widget UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_tab_widget(json &ui_item, QWidget *widget);
    /**
     * @brief Process a stacked widget UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_stacked_widget(json &ui_item, QWidget *widget);
    /**
     * @brief Process a status bar UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_status_bar(json &ui_item, QWidget *widget);
    /**
     * @brief Process a plot UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_plot(json &ui_item, QWidget *widget);
    /**
     * @brief Process a table UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_table(json &ui_item, QWidget *widget);
    /**
     * @brief Process a CAN UI item
     *
     * @param ui_item The JSON UI item
     * @param widget The associated widget
     */
    void process_ui_can(json &ui_item, QWidget *widget);

    /**
     * @brief Setup a generic UI item
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_ui_item(QWidget *item, size_t index);
    /**
     * @brief Setup a button UI item
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_button(QWidget *item, size_t index);
    /**
     * @brief Setup the combobox object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_combobox(QWidget *item, size_t index);
    /**
     * @brief Setup the checkbox object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_checkbox(QWidget *item, size_t index);
    /**
     * @brief Setup the dial object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_dial(QWidget *item, size_t index);
    /**
     * @brief Setup the slider object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_slider(QWidget *item, size_t index);
    /**
     * @brief Setup the slider object
     * @param item
     * @param index
     */
    void setup_textbox(QWidget *item, size_t index);
    /**
     * @brief Setup the spinbox object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_spinbox(QWidget *item, size_t index);
    /**
     * @brief Setup the double spinbox object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_double_spinbox(QWidget *item, size_t index);
    /**
     * @brief Setup the hex spinbox object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_hex_spinbox(QWidget *item, size_t index);
    /**
     * @brief Setup the tab widget object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_tab_widget(QWidget *item, size_t index);
    /**
     * @brief Setup the CAN UI object
     *
     * @param item The UI item
     * @param index The index of the UI item
     */
    void setup_can_ui(QWidget *item, size_t index);

    /**
     * @brief Handle the show event
     *        And will connect the WebSocket signals
     *
     * @param event
     */
    void showEvent(QShowEvent *event) override;
    /**
     * @brief Handle the close event
     *        And will disconnect the WebSocket signals
     *
     * @param event
     */
    void closeEvent(QCloseEvent *event) override;

  signals:
    void log_signal(const char *filename_in, int line_in, const char *funcname_in, int level, QString msg);

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
