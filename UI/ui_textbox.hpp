#pragma once
#include "ui_item.hpp"
#include <string>

#define UI_TEXTBOX_TYPE std::string("ui_textbox")

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
    UI_textbox(std::string hint, Color fg_color, Color bg_color, QObject *parent = nullptr);
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
    void set_text(std::string text) override;

    /**
     * @brief Get the hint text of the textbox
     *
     * @param hint The hint text to set
     */
    void set_hint(std::string hint);
    /**
     * @brief Get the hint text of the textbox
     *
     * @return std::string The hint text
     */
    std::string hint() const;

    /**
     * @brief Deserialize the textbox from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the textbox to JSON
     *
     * @param id The id of the textbox
     * @return json The JSON representation of the textbox
     */
    json to_json(size_t id) const override;
  signals:
    /**
     * @brief Signal emitted when the text changes
     *
     * @param text The new text
     */
    void changed(std::string text);

  private:
    std::string m_hint;
};
