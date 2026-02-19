#pragma once
#include "QPushButton"

/**
 * @brief Class representing an LED widget
 *
 */
class Led : public QPushButton
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new Led object
     *
     * @param parent The parent widget
     */
    explicit Led(QWidget *parent = nullptr);

    /**
     * @brief Construct a new Led object
     *
     * @param text The text to display on the button
     * @param parent The parent widget
     */
    explicit Led(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief Construct a new Led object
     *
     * @param icon The icon to display on the button
     * @param text The text to display on the button
     * @param parent The parent widget
     */
    Led(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~Led();

  private:
    /**
     * @brief Check if the widget is read-only
     *
     * @param widget The widget to check
     * @return true If the widget is read-only
     * @return false If the widget is not read-only
     */
    bool is_read_only(QWidget *widget);

    /**
     * @brief Set the read-only state of the widget
     *
     * @param widget The widget to set
     * @param read_only The read-only state to set
     */
    void set_read_only(QWidget *widget, bool read_only);
};
