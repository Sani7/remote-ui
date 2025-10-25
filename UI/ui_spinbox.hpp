#pragma once
#include "ui_item.hpp"

#define UI_SPINBOX_TYPE std::string("ui_spinbox")

/**
 * @brief Class representing a UI spinbox
 *
 */
class UI_spinbox : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI spinbox object
     *
     * @param parent The parent QObject
     */
    explicit UI_spinbox(QObject *parent = nullptr);
    /**
     * @brief Construct a UI spinbox object
     *
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_spinbox(double min, double max, double value, QObject *parent = nullptr);

    /**
     * @brief Set the value of the spinbox
     *
     * @param value The value to set
     */
    void set_value(double value) override;
    /**
     * @brief Get the value of the spinbox
     *
     * @return double The current value
     */
    double value() const;

    /**
     * @brief Get the minimum value of the spinbox
     *
     * @return double The minimum value
     */
    double min() const;
    /**
     * @brief Get the maximum value of the spinbox
     *
     * @return double The maximum value
     */
    double max() const;

    /**
     * @brief Deserialize the spinbox from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the spinbox to JSON
     *
     * @param id The id of the spinbox
     * @return json The JSON representation of the spinbox
     */
    json to_json(size_t id) const override;
  signals:
    /**
     * @brief Signal emitted when the spinbox value changes
     *
     * @param value The new value of the spinbox
     */
    void changed(double value);

  private:
    double m_min;
    double m_max;
    double m_value;
};
