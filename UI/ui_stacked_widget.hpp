#pragma once
#include "ui_item.hpp"
#define UI_STACKED_WIDGET_TYPE std::string("ui_stacked_widget")

/**
 * @brief Class representing a UI stacked widget
 *
 */
class UI_stacked_widget : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI stacked widget object
     *
     * @param parent The parent QObject
     */
    explicit UI_stacked_widget(QObject *parent = nullptr);
    /**
     * @brief Construct a UI stacked widget object
     *
     * @param tab_count The number of tabs
     * @param current_tab The currently selected tab
     * @param parent The parent QObject
     */
    UI_stacked_widget(size_t tab_count, size_t current_tab, QObject *parent = nullptr);

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
     * @brief Advance to the next tab
     *
     */
    void advance_tab();

    /**
     * @brief Deserialize the stacked widget from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the stacked widget to JSON
     *
     * @param id The id of the stacked widget
     * @return json The JSON representation of the stacked widget
     */
    json to_json(size_t id) const override;

  private:
    size_t m_tab_count;
    size_t m_current_tab;
};
