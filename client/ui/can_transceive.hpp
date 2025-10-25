#pragma once
#include "hex_spinbox.hpp"
#include <QWidget>

namespace Ui
{
class Can_Transceive;
}

/**
 * @brief Class representing a CAN transceive UI component
 *
 */
class Can_Transceive : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Can_Transceive object
     *
     * @param parent A pointer to the parent widget
     */
    explicit Can_Transceive(QWidget *parent = nullptr);
    /**
     * @brief Destroy the Can_Transceive object
     *
     */
    ~Can_Transceive();

    /**
     * @brief This function enables or disables the CAN data input fields based on the selected DLC
     *
     */
    void enable_can_send_data();

    /**
     * @brief Clear the CAN message lists
     *
     */
    void clear();

    /**
     * @brief Add a sent CAN message to the send list
     *
     * @param id The CAN message ID
     * @param dlc The Data Length Code
     * @param payload The CAN message payload
     */
    void add_send_item(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);

    /**
     * @brief Add a received CAN message to the receive list
     *
     * @param id The CAN message ID
     * @param dlc The Data Length Code
     * @param payload The CAN message payload
     */
    void add_receive_item(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);

  signals:
    /**
     * @brief Emitted when a CAN message is to be sent
     *
     * @param id The CAN message ID
     * @param dlc The Data Length Code
     * @param payload The CAN message payload
     */
    void send_can_message(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);

    /**
     * @brief Emitted when the CAN message lists are to be cleared
     *
     */
    void can_clear();

  private:
    Ui::Can_Transceive *ui;
    std::vector<HexSpinBox *> can_send_hexspinboxes;
};
