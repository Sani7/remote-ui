#pragma once

#include "UI.hpp"
#include "spdlog/spdlog.h"
#include <QSerialPort>
#include <QTimer>

class Simulator_base : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new Simulator_base object
     *
     * @param name The name of the simulator
     * @param interval The interval of the timer (default is 100ms)
     */
    Simulator_base(std::string name, CAN_Wrapper *can_wrapper = nullptr, QSerialPort *serial = nullptr,
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
     * @brief Run the simulator
     */
    void run();

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

    void shutdown();
    void reboot();

    bool check_input_high(uint32_t data, uint16_t pinnummer);
  signals:
    void sim_changed();

  protected:
    std::string m_name;
    std::vector<UI_item *> m_UI_items;
    QTimer *m_timer;
    std::chrono::milliseconds m_interval;
    CAN_Wrapper *m_can_wrapper;
    QSerialPort *m_serial;
};
