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
    QCanBusFrame read_frame();
  signals:
    void frames_received();
  private:
    QCanBusDevice *m_canDevice;
};
