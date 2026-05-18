#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI dial
 *
 */
class UI_dial : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI dial object
     *
     * @param text The text to display
     * @param unit The unit of the dial
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_dial(QString text, QString unit, double min, double max, double value, QObject *parent = nullptr);
    /**
     * @brief Construct a UI dial object
     *
     * @param text The text to display
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_dial(QString text, double min, double max, double value, QObject *parent = nullptr);
    /**
     * @brief Construct a UI dial object
     *
     * @param parent The parent QObject
     */
    UI_dial(QObject *parent = nullptr);

    /**
     * @brief Set the value of the dial
     *
     * @param value The value to set
     */
    void set_value(double value) override;
    /**
     * @brief Get the value of the dial
     *
     * @return double The current value
     */
    double value() const;

    /**
     * @brief Get the minimum value of the dial
     *
     * @return double The minimum value
     */
    double min() const;
    /**
     * @brief Get the maximum value of the dial
     *
     * @return double The maximum value
     */
    double max() const;
    /**
     * @brief Get the unit of the dial
     *
     * @return QString The unit of the dial
     */
    QString unit() const &;
};
