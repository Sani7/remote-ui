#ifndef CAN_TRANSCEIVE_H
#define CAN_TRANSCEIVE_H

#include "hex_spinbox.hpp"
#include <QWidget>

namespace Ui
{
class Can_Transceive;
}

class Can_Transceive : public QWidget
{
    Q_OBJECT

  public:
    explicit Can_Transceive(QWidget *parent = nullptr);
    ~Can_Transceive();
    void enable_can_send_data();
    void clear();
    void add_send_item(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);
    void add_receive_item(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);

  signals:
    void send_can_message(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload);
    void can_clear();

  private:
    Ui::Can_Transceive *ui;
    std::vector<HexSpinBox *> can_send_hexspinboxes;
};

#endif // CAN_TRANSCEIVE_H
