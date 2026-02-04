#pragma once
#include "color.hpp"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

#include <QCanBusFrame>
#include <QObject>
using json = nlohmann::json;

#define UNUSED(x) (void)(x)

/**
 * @brief Base class representing a UI item
 *
 */
class UI_item : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new UI item object
     *
     * @param type The type of the UI item
     * @param parent The parent QObject
     */
    explicit UI_item(std::string type, QObject *parent = nullptr);
    /**
     * @brief Construct a new UI item object
     *
     * @param type The type of the UI item
     * @param text The text of the UI item
     * @param fg_color The foreground color of the UI item
     * @param bg_color The background color of the UI item
     * @param parent The parent QObject
     */
    explicit UI_item(std::string type, std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);

    /**
     * @brief Get the type of the UI item
     *
     * @return std::string The type of the UI item
     */
    std::string type() const &;
    /**
     * @brief Check if the JSON object is of the correct type
     *
     * @param j The JSON object
     * @return true If the JSON object is of the correct type
     * @return false If the JSON object is not of the correct type
     */
    bool is_type(json j);

    /**
     * @brief Set the text of the UI item
     *
     * @param text The text to set
     */
    virtual void set_text(std::string text);
    /**
     * @brief Append text to the UI item
     *
     * @param text The text to append
     */
    void append_text(std::string text);
    /**
     * @brief Get the text of the UI item
     *
     * @return std::string The text of the UI item
     */
    std::string text() const &;

    /**
     * @brief Set the foreground color of the UI item
     *
     * @param fg_color The foreground color to set
     */
    void set_fg_color(Color fg_color);
    /**
     * @brief Get the foreground color of the UI item
     *
     * @return Color The foreground color of the UI item
     */
    Color fg_color() const &;

    /**
     * @brief Set the background color of the UI item
     *
     * @param bg_color The background color to set
     */
    void set_bg_color(Color bg_color);
    /**
     * @brief Get the background color of the UI item
     *
     * @return Color The background color of the UI item
     */
    Color bg_color() const &;

    /**
     * @brief Enable or disable the UI item
     *
     * @param enabled True to enable, false to disable
     */
    void set_enabled(bool enabled);
    /**
     * @brief Check if the UI item is enabled
     *
     * @return true If enabled
     * @return false If disabled
     */
    bool is_enabled() const;

    /**
     * @brief Set the visibility of the UI item
     *
     * @param visible True to show, false to hide
     */
    void set_visible(bool visible);
    /**
     * @brief Check if the UI item is visible
     *
     * @return true If visible
     * @return false If hidden
     */
    bool is_visible() const;

    /**
     * @brief Deserialize the UI item from JSON
     *
     * @param j The JSON object
     */
    virtual void from_json(const json &j);

    /**
     * @brief Serialize the UI item to JSON
     *
     * @param id The id of the UI item
     * @return json The JSON representation of the UI item
     */
    virtual json to_json(size_t id) const;

    /**
     * @brief Virtual method to handle click events
     *
     */
    virtual void click();
    /**
     * @brief Virtual method to set the selected option (for comboboxes)
     *
     * @param selected The index of the option to select
     */
    virtual void set_selected(size_t selected);
    /**
     * @brief Virtual method to set the value (for spinboxes)
     *
     * @param value The value to set
     */
    virtual void set_value(double value);
    /**
     * @brief Virtual method to handle CAN send events
     *
     * @param id The CAN ID
     * @param dlc The data length code
     * @param payload The payload data
     */
    virtual void can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);
    /**
     * @brief Virtual method to clear the UI item
     *
     */
    virtual void clear();

  signals:
    /**
     * @brief Signal emitted when the UI item changes
     *
     */
    void ui_changed();

  protected:
    /**
     * @brief Setup the UI item properties
     *
     * @param text Enable text property
     * @param fg_color Enable foreground color property
     * @param bg_color Enable background color property
     */
    void setup_item(bool text, bool fg_color, bool bg_color);

    /**
     * @brief The id of the UI item
     *
     */
    size_t m_id;
    /**
     * @brief The type of the UI item
     *
     */
    std::string m_type;
    /**
     * @brief The text of the UI item
     *
     */
    std::string m_text;
    /**
     * @brief Whether the text property is enabled
     *
     */
    bool m_text_enabled = true;
    /**
     * @brief The foreground color of the UI item
     *
     */
    Color m_fg_color;
    /**
     * @brief Whether the foreground color property is enabled
     *
     */
    bool m_fg_color_enabled = true;
    /**
     * @brief The background color of the UI item
     *
     */
    Color m_bg_color;
    /**
     * @brief Whether the background color property is enabled
     *
     */
    bool m_bg_color_enabled = true;
    /**
     * @brief Whether the UI item is enabled
     *
     */
    bool m_enabled;
    /**
     * @brief Whether the UI item is visible
     *
     */
    bool m_visible;
};
