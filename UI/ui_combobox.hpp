#pragma once
#include "ui_item.hpp"

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
    UI_combobox(QList<QString> options, qsizetype selected = 0,
                QObject *parent = nullptr);

    void set_fg_color(Color fg_color);
    Color fg_color() const;
    void set_bg_color(Color bg_color);
    Color bg_color() const;

    /**
     * @brief Set the selected option by index
     *
     * @param selected The index of the option to select
     */
    void set_selected(qsizetype selected) override;
    /**
     * @brief Set the selected option by text
     *
     * @param selected The text of the option to select
     */
    void set_selected(QString selected);
    /**
     * @brief Get the index of the selected option
     *
     * @return size_t The index of the selected option
     */
    qsizetype selected() const;
    /**
     * @brief Get the text of the selected option
     *
     * @return std::string The text of the selected option
     */
    QString selected_text() const &;
  signals:
    /**
     * @brief Signal emitted when the selected option changes
     *
     * @param selected The text of the newly selected option
     */
    void changed(QString selected);
};
