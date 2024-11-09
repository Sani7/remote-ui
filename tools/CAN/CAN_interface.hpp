#pragma once
#include <ctime>
#include <cstdint>
#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>

class CAN_Interface : public QObject
{
    Q_OBJECT
  public:
    CAN_Interface(QString dev, QObject *parent = nullptr);
    ~CAN_Interface();

    void send_frame(const QCanBusFrame &frame);
    uint8_t next_mid();
  signals:
    void frame_received(const QCanBusFrame frame);

  private:
    QCanBusDevice *m_canDevice;
    uint8_t m_mid;
};
