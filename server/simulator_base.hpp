#pragma once
#include "spdlog/spdlog.h"
#include <QSerialPort>
#include <QThread>
#include <QTimer>

#ifndef INTERNAL
#include "ui.hpp"
#else
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "CAN_wrapper.hpp"
Q_FORWARD_DECLARE_OBJC_CLASS(UI_item);
#endif

#define PUSH_UI_ITEM(item) push_ui_item(&item)
#define EXPORT_SIM(sim)                                                                                                \
    extern "C" Q_DECL_EXPORT Simulator_base *get_sim(Communication *com, QObject *parent)                              \
    {                                                                                                                  \
        return new sim(com, parent);                                                                                   \
    }

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
    std::unique_ptr<CAN_Wrapper> can_if[3] = {nullptr, nullptr, nullptr};
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
    bool check_input_high(uint32_t data, uint16_t pinnummer);
  signals:
    /**
     * @brief Signal emitted when the ui of the simulator changes
     *
     */
    void sim_changed();

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
