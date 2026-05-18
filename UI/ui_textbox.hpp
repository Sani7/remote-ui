#pragma once
#include "ui_item.hpp"
#include <string>

/**
 * @brief Class representing a UI textbox
 *
 */
class UI_textbox : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI textbox object
     *
     * @param hint The hint text
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param parent The parent QObject
     */
    UI_textbox(QString hint, Color fg_color, Color bg_color, QObject *parent = nullptr);
    /**
     * @brief Construct a UI textbox object
     *
     * @param parent The parent QObject
     */
    UI_textbox(QObject *parent = nullptr);

    /**
     * @brief Set the text of the textbox
     *
     * @param text The text to set
     */
    void set_text(QString text) override;

    QString text() const;

    void set_fg_color(Color fg_color);
    Color fg_color() const;
    void set_bg_color(Color bg_color);
    Color bg_color() const;

    /**
     * @brief Get the hint text of the textbox
     *
     * @param hint The hint text to set
     */
    void set_hint(QString hint);
    /**
     * @brief Get the hint text of the textbox
     *
     * @return QString The hint text
     */
    QString hint() const &;
  signals:
    /**
     * @brief Signal emitted when the text changes
     *
     * @param text The new text
     */
    void changed(QString text);
};
