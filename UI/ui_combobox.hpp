#pragma once
#include "ui_item.hpp"

#define UI_COMBOBOX_TYPE std::string("ui_combobox")

/**
 * @brief Class representing a UI combobox
 *
 */
class UI_combobox : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI combobox object
     *
     * @param parent The parent QObject
     */
    UI_combobox(QObject *parent = nullptr);
    /**
     * @brief Construct a UI combobox object
     *
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param options The list of options
     * @param selected The initially selected option
     * @param parent The parent QObject
     */
    UI_combobox(Color fg_color, Color bg_color, std::vector<std::string> options, size_t selected = 0,
                QObject *parent = nullptr);

    /**
     * @brief Set the selected option by index
     *
     * @param selected The index of the option to select
     */
    void set_selected(size_t selected) override;
    /**
     * @brief Set the selected option by text
     *
     * @param selected The text of the option to select
     */
    void set_selected(std::string selected);
    /**
     * @brief Get the index of the selected option
     *
     * @return size_t The index of the selected option
     */
    size_t selected() const;
    /**
     * @brief Get the text of the selected option
     *
     * @return std::string The text of the selected option
     */
    std::string selected_text() const;

    /**
     * @brief Deserialize the combobox from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the combobox to JSON
     *
     * @param id The id of the combobox
     * @return json The JSON representation of the combobox
     */
    json to_json(size_t id) const override;
  signals:
    /**
     * @brief Signal emitted when the selected option changes
     *
     * @param selected The text of the newly selected option
     */
    void changed(std::string selected);

  private:
    std::vector<std::string> m_options;
    size_t m_selected;
};
