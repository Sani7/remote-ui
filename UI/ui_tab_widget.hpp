#pragma once
#include "ui_item.hpp"

#define UI_TAB_WIDGET_TYPE std::string("ui_tab_widget")

/**
 * @brief Class representing a UI tab widget
 *
 */
class UI_tab_widget : public UI_item
{
    Q_OBJECT
  public:
    /**  * @brief Construct a UI tab widget object
     *
     * @param parent The parent QObject
     */
    explicit UI_tab_widget(QObject *parent = nullptr);
    /**
     * @brief Construct a UI tab widget object
     *
     * @param tab_names The names of the tabs
     * @param selected The initially selected tab
     * @param parent The parent QObject
     */
    UI_tab_widget(std::vector<std::string> tab_names, size_t selected = 0, QObject *parent = nullptr);

    /**
     * @brief Set the selected tab by index
     *
     * @param selected The index of the tab to select
     */
    void set_selected(size_t selected) override;
    /**
     * @brief Get the index of the selected tab
     *
     * @return size_t The index of the selected tab
     */
    size_t selected() const;

    /**
     * @brief Set the visibility of a tab by index
     *
     * @param index The index of the tab
     * @param visible True to show, false to hide
     */
    void set_visible(size_t index, bool visible);
    /**
     * @brief Get the visibility of a tab by index
     *
     * @param index The index of the tab
     * @return true If the tab is visible
     * @return false If the tab is hidden
     */
    bool visible(size_t index) const;

    /**
     * @brief Set the name of a tab by index
     *
     * @param index The index of the tab
     * @param name The name to set
     */
    void set_tab_name(size_t index, std::string name);
    /**
     * @brief Get the name of a tab by index
     *
     * @param index The index of the tab
     * @return std::string The name of the tab
     */
    std::string tab_name(size_t index) const;

    /**
     * @brief Advance to the next tab
     *
     */
    void advance_tab();

    /**
     * @brief Deserialize the tab widget from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the tab widget to JSON
     *
     * @param id The id of the tab widget
     * @return json The JSON representation of the tab widget
     */
    json to_json(size_t id) const override;
  signals:
    /**
     * @brief Signal emitted when the selected tab changes
     *
     * @param index The index of the newly selected tab
     */
    void changed(size_t index);

  private:
    size_t m_selected_tab;
    std::vector<std::string> m_tab_names;
    std::vector<bool> m_tab_visible;
};
