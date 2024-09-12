#pragma once

#include "UI.hpp"
#include "spdlog/spdlog.h"
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
    Simulator_base(std::string name, std::chrono::milliseconds interval = std::chrono::milliseconds(100), QObject* parrent = nullptr);

    /**
     * @brief Returns the name of the simulator
     *
     * @return std::string The name of the simulator
     */
    std::string name() const;

    /**
     * @brief This function checks if the UI id is unique
     *         (hence not already pushed in with the add_UI_item function)
     *
     * @param id The id to check
     * @return true The id is unique
     * @return false The id is already in use
     */
    bool is_ui_id_unique(std::string id) const;

    /**
     * @brief This function adds a UI item to the simulator
     *        only if the id is unique
     *        Use this function only in the constructor of the simulator
     *
     * @param item The UI item to add
     */
    void add_UI_item(UI_item *item);
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
    UI_item *get_UI_item(std::string id) const;

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

    void shutdown();
    void reboot();
  signals:
    void sim_changed();

  protected:
    std::string m_name;
    std::vector<UI_item *> m_UI_items;
    std::map<std::string, UI_item *> m_UI_items_map;
    QTimer* m_timer;
    std::chrono::milliseconds m_interval;
};
