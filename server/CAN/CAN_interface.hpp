#pragma once
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QObject>

class CAN_Interface : public QObject
{
    Q_OBJECT
  public:
    CAN_Interface(QObject *parent = nullptr);
    ~CAN_Interface();
    void connect_to_dev(QString dev);

    void send_frame(const QCanBusFrame frame);
  signals:
    void frame_received(QCanBusFrame frame);
    void frame_send(QCanBusFrame frame);

  private:
    void processErrors(QCanBusDevice::CanBusError error) const;
    QCanBusDevice *m_canDevice;
};
