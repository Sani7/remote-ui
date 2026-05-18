#pragma once
#include "ui_item.hpp"
#include <string>

/**
 * @brief Class representing a UI led
 *
 */
class UI_led : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI led object
     *
     * @param parent The parent QObject
     */
    UI_led(QObject *parent = nullptr);
    /**
     * @brief Construct a UI led object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param parent The parent QObject
     */
    UI_led(QString text, Color fg_color, Color bg_color, QObject *parent = nullptr);

    /**
     * @brief set_text
     * @param text The text field of the button
     */
    void set_text(QString text) override;

    /**
     * @brief text
     * @return The text field of the button
     */
    QString text() const;

    void set_fg_color(Color fg_color);
    Color fg_color() const;
    void set_bg_color(Color bg_color);
    Color bg_color() const;

    /**
     * @brief Set the state of the LED
     *
     * @param state The state to set
     * @param on The color when the LED is on
     * @param off The color when the LED is off
     * @return state
     */
    bool set_led(bool state, Color on, Color off);
};
