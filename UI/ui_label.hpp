#pragma once
#include "ui_item.hpp"
#include <string>

/**
 * @brief Class representing a UI label
 *
 */
class UI_label : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI label object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param parent The parent QObject
     */
    UI_label(QString text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    /**
     * @brief Construct a UI label object
     *
     * @param parent The parent QObject
     */
    UI_label(QObject *parent = nullptr);

    /**
     * @brief set_text
     * @param text The text field of the button
     */
    void set_text(QString text);

    /**
     * @brief append_text Append
     * @param text
     */
    void append_text(QString text);

    /**
     * @brief text
     * @return The text field of the button
     */
    QString text() const;

    void set_fg_color(Color fg_color);
    Color fg_color() const;
    void set_bg_color(Color bg_color);
    Color bg_color() const;
};
