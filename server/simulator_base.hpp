#pragma once
#include <QSerialPort>
#include <QThread>
#include <QTimer>

#include "extensions/CAN_interface.hpp"
#ifndef INTERNAL
#include "ui.hpp"
#else
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Q_FORWARD_DECLARE_OBJC_CLASS(UI_item);
#endif

#define PUSH_UI_ITEM(item) push_ui_item(&item)
#define EXPORT_SIM(sim)                                                                                                \
    extern "C" Q_DECL_EXPORT Simulator_base *get_sim(Communication *com, QObject *parent)                              \
    {                                                                                                                  \
        return new sim(com, parent);                                                                                   \
    }

#define LOG_SRC_LOC(level, msg) emit log_signal(__FILE__, __LINE__, __FUNCTION__, (int)level, msg)
#define LOG_TRACE(msg) LOG_SRC_LOC(Log_level_enum::trace, msg)
#define LOG_DEBUG(msg) LOG_SRC_LOC(Log_level_enum::debug, msg)
#define LOG_INFO(msg) LOG_SRC_LOC(Log_level_enum::info, msg)
#define LOG_WARN(msg) LOG_SRC_LOC(Log_level_enum::warn, msg)
#define LOG_ERROR(msg) LOG_SRC_LOC(Log_level_enum::err, msg)
#define LOG_CRITICAL(msg) LOG_SRC_LOC(Log_level_enum::critical, msg)

/**
 * @brief Struct holding all communication interfaces
 *
 */
class Communication
{
  public:
    /**
     * @brief Array of CAN interfaces
     *
     */
    std::unique_ptr<CAN_Interface> can_if[3] = {nullptr, nullptr, nullptr};
    /**
     * @brief Array of UART interfaces
     *
     */
    std::unique_ptr<QSerialPort> uart_if[4] = {nullptr, nullptr, nullptr, nullptr};
};

/**
 * @brief Base class for all simulators
 *
 */
class Simulator_base : public QObject
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
     * @brief Construct a new Simulator_base object
     *
     * @param name The name of the simulator
     * @param comms The communication interfaces
     * @param interval The interval of the timer (default is 100ms)
     * @param parent The parent QObject
     */
    Simulator_base(std::string name, Communication *comms,
                   std::chrono::milliseconds interval = std::chrono::milliseconds(100), QObject *parent = nullptr);

    /**
     * @brief Returns the name of the simulator
     *
     * @return std::string The name of the simulator
     */
    std::string name() const;

    /**
     * @brief This function adds a UI item to the simulator
     *        only if the id is unique
     *        Use this function only in the constructor of the simulator
     *
     * @param item The UI item to add
     */
    void push_ui_item(UI_item *item);
    /**
     * @brief Get the UI items object
     *
     * @return json The UI items in json format
     */
    json get_UI_items() const;

    /**
     * @brief Get the UI item object by id
     *
     * @param id The id of the UI item
     * @return UI_item* A pointer to the UI item object
     */
    UI_item *get_UI_item(size_t id) const;

    /**
     * @brief Get the id of a UI item by pointer
     *
     * @param ptr A pointer to the UI item
     * @return size_t The id of the UI item, or -1 if not found
     */
    size_t ptr_to_id(UI_item *ptr) const
    {
        size_t id = 0;
        while (id < m_UI_items.size())
        {
            if (m_UI_items[id] == ptr)
            {
                return id;
            }
            id++;
        }
        return (size_t)-1;
    }

    /**
     * @brief Start the simulator
     */
    void start();

    /**
     * @brief Stop the simulator
     */
    void stop();

    /**
     * @brief Timer function
     *       This function is called every time the timer ticks
     *       Override this function in the derived class
     */
    virtual void timer() {};

    /**
     * @brief Run at startup
     *       This function is called at startup
     *       Override this function in the derived class
     */
    virtual void run_at_startup() {};

    /**
     * @brief Run at stop
     *        This function is called when a simulator is stoped
     *        Override this function in the derived class
     */
    virtual void run_at_stop() {};

    /**
     * @brief Shutdown the system
     *
     */
    void shutdown();
    /**
     * @brief Reboot the system
     *
     */
    void reboot();

    /**
     * @brief Check if a specific bit is high in the input data
     *
     * @param data The input data
     * @param pinnummer The pin number to check
     * @return true If the bit is high
     * @return false If the bit is low
     */
    inline bool check_input_high(uint32_t data, uint16_t pinnummer)
    {
        return (data & (1 << pinnummer));
    }
  signals:
    /**
     * @brief Signal emitted when the ui of the simulator changes
     *
     */
    void sim_changed();

    void log_signal(const char *filename_in, int line_in, const char *funcname_in, int level, QString msg);

  protected:
    /**
     * @brief The name of the simulator
     *
     */
    std::string m_name;
    /**
     * @brief The UI items of the simulator
     *
     */
    std::vector<UI_item *> m_UI_items;
    /**
     * @brief The timer for the simulator
     *
     */
    QTimer *m_timer;
    /**
     * @brief The interval of the timer
     *
     */
    std::chrono::milliseconds m_interval;
    /**
     * @brief The communication interface
     *
     */
    Communication *m_com;
};
