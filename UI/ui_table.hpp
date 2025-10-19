#pragma once
#include "ui_item.hpp"

#define UI_TABLE_TYPE std::string("ui_table")

class UI_table : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_table(QObject *parent = nullptr);
    explicit UI_table(size_t row_count, size_t column_count, std::vector<std::string> row_labels,
                      std::vector<std::string> column_labels, QObject *parent = nullptr);

    void set_row_count(size_t count);
    size_t row_count() const;
    void set_column_count(size_t count);
    size_t column_count() const;
    size_t capacity() const;
    void set_row_label(size_t index, std::string label);
    std::string row_label(size_t index) const;
    void set_column_label(size_t index, std::string label);
    std::string column_label(size_t index) const;

    void insert_item(size_t row, size_t column, std::string text);
    std::string item(size_t row, size_t column) const;

    /**
     * @brief empty_item This function clears the item and makes it invalid
     * @param row The row of the item
     * @param column The column of the item
     */
    void empty_item(size_t row, size_t column);

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  private:
    size_t m_row_count;
    size_t m_column_count;
    std::vector<std::string> m_row_labels;
    std::vector<std::string> m_column_labels;

    std::vector<std::string> m_table;
};
