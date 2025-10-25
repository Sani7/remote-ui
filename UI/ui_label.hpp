#pragma once
#include "ui_item.hpp"
#include <string>

#define UI_LABEL_TYPE std::string("ui_label")

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
    UI_label(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    /**
     * @brief Construct a UI label object
     *
     * @param parent The parent QObject
     */
    UI_label(QObject *parent = nullptr);

    /**
     * @brief Deserialize the label from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the label to JSON
     *
     * @param id The id of the label
     * @return json The JSON representation of the label
     */
    json to_json(size_t id) const override;
};
