#pragma once
#include "ui_item.hpp"

#define UI_CHECKBOX_TYPE std::string("ui_checkbox")

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
    UI_checkbox(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);

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

    /**
     * @brief Deserialize the checkbox from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the checkbox to JSON
     *
     * @param id The id of the checkbox
     * @return json The JSON representation of the checkbox
     */
    json to_json(size_t id) const override;

  signals:
    /**
     * @brief Signal emitted when the checkbox state changes
     *
     * @param checked The new state of the checkbox
     */
    void changed(bool checked);

  private:
    bool m_checked;
};
