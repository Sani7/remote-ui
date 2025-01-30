#pragma once
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
  signals:
    void frame_received(const QCanBusFrame frame);
    void frame_send(const QCanBusFrame frame);

  private:
    void processErrors(QCanBusDevice::CanBusError error) const;
    QCanBusDevice *m_canDevice;
};
