#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI checkbox
 *
 */
class UI_checkbox : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI checkbox object
     *
     * @param parent The parent QObject
     */
    UI_checkbox(QObject *parent = nullptr);
    /**
     * @brief Construct a UI checkbox object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param parent The parent QObject
     */
    UI_checkbox(QString text, Color fg_color, Color bg_color, QObject *parent = nullptr);

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
     * @brief Handle a click event on the checkbox
     *
     */
    void click() override;
    /**
     * @brief Set the state of the checkbox
     *
     * @param state The state to set
     */
    void set_state(bool state);
    /**
     * @brief Get the state of the checkbox
     *
     * @return true If checked
     * @return false If unchecked
     */
    bool state() const;

  signals:
    /**
     * @brief Signal emitted when the checkbox state changes
     *
     * @param checked The new state of the checkbox
     */
    void changed(bool checked);
};
