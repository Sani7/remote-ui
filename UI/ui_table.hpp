#pragma once
#include "ui_item.hpp"

#define UI_TABLE_TYPE std::string("ui_table")

/**
 * @brief Class representing a UI table
 *
 */
class UI_table : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI table object
     *
     * @param parent The parent QObject
     */
    explicit UI_table(QObject *parent = nullptr);
    /**
     * @brief Construct a UI table object
     *
     * @param row_count The number of rows
     * @param column_count The number of columns
     * @param row_labels The labels for the rows
     * @param column_labels The labels for the columns
     * @param parent The parent QObject
     */
    explicit UI_table(size_t row_count, size_t column_count, std::vector<std::string> row_labels,
                      std::vector<std::string> column_labels, QObject *parent = nullptr);

    /**
     * @brief Set the number of rows in the table
     *
     * @param count The number of rows
     */
    void set_row_count(size_t count);
    /**
     * @brief Get the number of rows in the table
     *
     * @return size_t The number of rows
     */
    size_t row_count() const;

    /**
     * @brief Set the number of columns in the table
     *
     * @param count The number of columns
     */
    void set_column_count(size_t count);
    /**
     * @brief Get the number of columns in the table
     *
     * @return size_t The number of columns
     */
    size_t column_count() const;

    /**
     * @brief Get the capacity of the table (rows * columns)
     *
     * @return size_t The capacity of the table
     */
    size_t capacity() const;

    /**
     * @brief Set the label for a specific row
     *
     * @param index The index of the row
     * @param label The label to set
     */
    void set_row_label(size_t index, std::string label);
    /**
     * @brief Get the label of a specific row
     *
     * @param index The index of the row
     * @return std::string The label of the row
     */
    std::string row_label(size_t index) const;

    /**
     * @brief Set the label for a specific column
     *
     * @param index The index of the column
     * @param label The label to set
     */
    void set_column_label(size_t index, std::string label);
    /**
     * @brief Get the label of a specific column
     *
     * @param index The index of the column
     * @return std::string The label of the column
     */
    std::string column_label(size_t index) const &;

    /**
     * @brief Insert an item into the table at the specified row and column
     *
     * @param row The row of the item
     * @param column The column of the item
     * @param text The text of the item
     */
    void insert_item(size_t row, size_t column, std::string text);
    /**
     * @brief Get the item at the specified row and column
     *
     * @param row The row of the item
     * @param column The column of the item
     * @return std::string The text of the item
     */
    std::string item(size_t row, size_t column) const &;

    /**
     * @brief empty_item This function clears the item
     * @param row The row of the item
     * @param column The column of the item
     */
    void empty_item(size_t row, size_t column);

    /**
     * @brief Deserialize the table from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the table to JSON
     *
     * @param id The id of the table
     * @return json The JSON representation of the table
     */
    json to_json(size_t id) const override;

  private:
    size_t m_row_count;
    size_t m_column_count;
    std::vector<std::string> m_row_labels;
    std::vector<std::string> m_column_labels;

    std::vector<std::string> m_table;
};
