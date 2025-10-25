#pragma once
#include "ui_item.hpp"
#define UI_PLOT_TYPE std::string("ui_plot")

/**
 * @brief Class representing a UI plot
 *
 */
class UI_plot : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI plot object
     *
     * @param parent The parent QObject
     */
    explicit UI_plot(QObject *parent = nullptr);
    /**
     * @brief Construct a UI plot object
     *
     * @param text The text to display
     * @param x_label The label for the x-axis
     * @param y_label The label for the y-axis
     * @param parent The parent QObject
     */
    UI_plot(std::string text, std::string x_label, std::string y_label, QObject *parent = nullptr);

    /**
     * @brief Set the label for the x-axis
     *
     * @param name The name to set
     */
    void set_x_label(std::string name);
    /**
     * @brief Get the label for the x-axis
     *
     * @return std::string The x-axis label
     */
    std::string x_label() const;

    /**
     * @brief Set the label for the y-axis
     *
     * @param name The name to set
     */
    void set_y_label(std::string name);
    /**
     * @brief Get the label for the y-axis
     *
     * @return std::string The y-axis label
     */
    std::string y_label() const;

    /**
     * @brief Add a value to the plot
     *
     * @param y The y value to add
     */
    void add_value(double y);
    /**
     * @brief Add a value pair to the plot
     *
     * @param x The x value to add
     * @param y The y value to add
     */
    void add_value(double x, double y);
    /**
     * @brief Get the x and y values of the plot
     *        This is only for testing purposes
     *
     * @return a list of x values and a list of y values
     */
    const std::vector<double> &x_vals() const;
    /**
     * @brief Get the y values of the plot
     *        This is only for testing purposes
     *
     * @return a list of y values
     */
    const std::vector<double> &y_vals() const;
    /**
     * @brief Clear all values from the plot
     *
     */
    void clean_vals();

    /**
     * @brief Deserialize the plot from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the plot to JSON
     *
     * @param id The id of the plot
     * @return json The JSON representation of the plot
     */
    json to_json(size_t id) const override;

  private:
    std::vector<double> m_x_vals;
    std::vector<double> m_y_vals;
    std::string m_x_label;
    std::string m_y_label;
};
