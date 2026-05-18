#pragma once
#include "ui_item.hpp"

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
    UI_status_bar(QString message, QObject *parent = nullptr);

    /**
     * @brief Show a message on the status bar
     *
     * @param message The message to show
     * @param timeout The timeout in milliseconds (0 for no timeout)
     */
    void show_message(QString message, size_t timeout = 0);
    /**
     * @brief Get the current message
     *
     * @return QString The current message
     */
    QString message() const &;
    /**
     * @brief Get the timeout for the current message
     *
     * @return size_t The timeout in milliseconds
     */
    size_t timeout() const;
};
