#pragma once
#include "ui_item.hpp"
#define UI_STATUS_BAR_TYPE std::string("ui_status_bar")

/**
 * @brief Class representing a UI status bar
 *
 */
class UI_status_bar : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI status bar object
     *
     * @param parent The parent QObject
     */
    explicit UI_status_bar(QObject *parent = nullptr);
    /**
     * @brief Construct a UI status bar object
     *
     * @param message The initial message
     * @param parent The parent QObject
     */
    UI_status_bar(std::string message, QObject *parent = nullptr);

    /**
     * @brief Show a message on the status bar
     *
     * @param message The message to show
     * @param timeout The timeout in milliseconds (0 for no timeout)
     */
    void show_message(std::string message, size_t timeout = 0);
    /**
     * @brief Get the current message
     *
     * @return std::string The current message
     */
    std::string message() const &;
    /**
     * @brief Get the timeout for the current message
     *
     * @return size_t The timeout in milliseconds
     */
    size_t timeout() const;

    /**
     * @brief Deserialize the status bar from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the status bar to JSON
     *
     * @param id The id of the status bar
     * @return json The JSON representation of the status bar
     */
    json to_json(size_t id) const override;

  private:
    size_t m_timeout;
};
