#pragma once
#include "ui_item.hpp"

#define UI_BUTTON_TYPE std::string("ui_button")

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
    UI_button(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    /**
     * @brief Construct a UI button object
     *
     * @param parent The parent QObject
     */
    UI_button(QObject *parent = nullptr);

    /**
     * @brief Deserialize the button from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the button to JSON
     *
     * @param id The id of the button
     * @return json The JSON representation of the button
     */
    json to_json(size_t id) const override;

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
