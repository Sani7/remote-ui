#pragma once
#include "color.hpp"
#include <magic_enum/magic_enum.hpp>
#include "m_ui_items.qpb.h"
#include <QProtobufSerializer>
#include <cstdint>
#include <string>

#include <QCanBusFrame>
#include <QObject>

/**
 * @brief Base class representing a UI item
 *
 */
class UI_item : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new UI item object
     *
     * @param type The type of the UI item
     * @param parent The parent QObject
     */
    explicit UI_item(UI_typeGadget::UI_type type, QObject *parent = nullptr);

    /**
     * @brief set_id Function for setting ID, DO NOT USE THIS IN A SIM YOURSELF
     *
     * @param id
     */
    void set_id(size_t id);

    /**
     * @brief Enable or disable the UI item
     *
     * @param enabled True to enable, false to disable
     */
    void set_enabled(bool enabled);
    /**
     * @brief Check if the UI item is enabled
     *
     * @return true If enabled
     * @return false If disabled
     */
    bool is_enabled() const;

    /**
     * @brief Set the visibility of the UI item
     *
     * @param visible True to show, false to hide
     */
    void set_visible(bool visible);
    /**
     * @brief Check if the UI item is visible
     *
     * @return true If visible
     * @return false If hidden
     */
    bool is_visible() const;

    UI_item_m to_proto() const;
    void from_proto(UI_item_m item);

    /**
     * @brief Virtual method to handle click events
     *
     */
    virtual void click();
    /**
     * @brief Virtual method to set the selected option (for comboboxes)
     *
     * @param selected The index of the option to select
     */
    virtual void set_selected(qsizetype selected);

    virtual void set_text(QString text);

    /**
     * @brief Virtual method to set the value (for spinboxes)
     *
     * @param value The value to set
     */
    virtual void set_value(double value);
    /**
     * @brief Virtual method to handle CAN send events
     *
     * @param id The CAN ID
     * @param dlc The data length code
     * @param payload The payload data
     */
    virtual void can_send(uint32_t id, uint64_t dlc, QByteArray payload);
    /**
     * @brief Virtual method to clear the UI item
     *
     */
    virtual void clear();

  signals:
    /**
     * @brief Signal emitted when the UI item changes
     *
     */
    void ui_changed();

  protected:
    UI_item_m m_ui_item;
};
