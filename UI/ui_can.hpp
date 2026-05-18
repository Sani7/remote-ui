#pragma once
#include "ui_item.hpp"
#include <QCanBusFrame>

/**
 * @brief Class representing a CAN UI item
 *
 */
class CanFrame
{
  public:
    /**
     * @brief Construct a new Can Frame object
     *
     * @param sid The standard identifier
     * @param dlc The data length code
     * @param payload The data payload
     */
    CanFrame(uint32_t sid, uint8_t dlc, std::array<uint8_t, 8> payload) : m_SID(sid), m_DLC(dlc), m_payload(payload)
    {
    }
    /**
     * @brief Get the Standard Identifier
     *
     * @return uint32_t The standard identifier
     */
    uint32_t m_SID;
    /**
     * @brief The Data Length Code
     *
     */
    uint8_t m_DLC;
    /**
     * @brief The Payload
     *
     */
    std::array<uint8_t, 8> m_payload;
};

/**
 * @brief Class representing a CAN UI item
 *
 */
class UI_can : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI can object
     *
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param parent The parent QObject
     */
    UI_can(Color fg_color, Color bg_color, QObject *parent = nullptr);
    /**
     * @brief Construct a UI can object
     *
     * @param parent The parent QObject
     */
    UI_can(QObject *parent = nullptr);

    /**
     * @brief Add a CAN message to the send buffer
     *
     * @param msg The CAN message to add
     */
    void add_send_message(QCanBusFrame msg);
    /**
     * @brief Add a CAN message to the receive buffer
     *
     * @param msg The CAN message to add
     */
    void add_received_message(QCanBusFrame msg);
    /**
     * @brief Clear the send buffer
     *
     */
    void clear_send_buffer();
    /**
     * @brief Clear the receive buffer
     *
     */
    void clear_receive_buffer();

    /**
     * @brief Handle a CAN send event
     *
     * @param id The CAN identifier
     * @param dlc The data length code
     * @param payload The data payload
     */
    void can_send(uint32_t id, uint64_t dlc, QByteArray payload) override;
    /**
     * @brief Clear the CAN item
     *
     */
    void clear() override;
  signals:
    /**
     * @brief Signal emitted when a CAN message wants to be sent
     * @param msg The CAN message sent
     */
    void send(QCanBusFrame msg);
};
