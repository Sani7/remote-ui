#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI button
 *
 */
class UI_button : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI button object
     *
     * @param text The text to display on the button
     * @param fg_color The foreground color of the button
     * @param bg_color The background color of the button
     * @param parent The parent QObject
     */
    UI_button(QString text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    /**
     * @brief Construct a UI button object
     *
     * @param parent The parent QObject
     */
    UI_button(QObject *parent = nullptr);

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
     * @brief Handle a click event on the button
     *
     */
    void click() override;

  signals:
    /**
     * @brief Signal emitted when the button is clicked
     *
     */
    void clicked();
};
