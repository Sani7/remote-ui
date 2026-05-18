#pragma once
#include "ui_item.hpp"

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
    UI_stacked_widget(qsizetype tab_count, qsizetype current_tab, QObject *parent = nullptr);

    /**
     * @brief Set the selected tab by index
     *
     * @param selected The index of the tab to select
     */
    void set_selected(qsizetype selected) override;
    /**
     * @brief Get the index of the selected tab
     *
     * @return size_t The index of the selected tab
     */
    qsizetype selected() const;
    /**
     * @brief Advance to the next tab
     *
     */
    void advance_tab();
};
