#pragma once
#include "ui_item.hpp"

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
    UI_tab_widget(QList<QString> tab_names, qsizetype selected = 0, QObject *parent = nullptr);

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
     * @brief Set the visibility of a tab by index
     *
     * @param index The index of the tab
     * @param visible True to show, false to hide
     */
    void set_visible(qsizetype index, bool visible);
    /**
     * @brief Get the visibility of a tab by index
     *
     * @param index The index of the tab
     * @return true If the tab is visible
     * @return false If the tab is hidden
     */
    bool visible(qsizetype index) const;

    /**
     * @brief Set the name of a tab by index
     *
     * @param index The index of the tab
     * @param name The name to set
     */
    void set_tab_name(qsizetype index, QString name);
    /**
     * @brief Get the name of a tab by index
     *
     * @param index The index of the tab
     * @return std::string The name of the tab
     */
    QString tab_name(qsizetype index) const;

    /**
     * @brief Advance to the next tab
     *
     */
    void advance_tab();
  signals:
    /**
     * @brief Signal emitted when the selected tab changes
     *
     * @param index The index of the newly selected tab
     */
    void changed(qsizetype index);
};
