#pragma once
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QObject>

/**
 * @brief Class representing a CAN interface
 *
 */
class CAN_Interface : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new CAN Interface object
     *
     * @param parent The parent QObject
     */
    CAN_Interface(QObject *parent = nullptr);
    /**
     * @brief Destroy the CAN Interface object
     *
     */
    ~CAN_Interface();

    /**
     * @brief Connect to a CAN device
     *
     * @param dev The device to connect to (e.g. "can0")
     */
    void connect_to_dev(QString dev);

    /**
     * @brief Send a CAN frame
     *
     * @param frame The frame to send
     */
    void send_frame(const QCanBusFrame frame);
  signals:
    /**
     * @brief Signal emitted when a CAN frame is received
     *
     * @param frame The received CAN frame
     */
    void frame_received(QCanBusFrame frame);
    /**
     * @brief Signal emitted when a CAN frame is sent
     *
     * @param frame The sent CAN frame
     */
    void frame_send(QCanBusFrame frame);

  private:
    /**
     * @brief Private method to process CAN bus errors
     *
     * @param error The CAN bus error
     */
    void processErrors(QCanBusDevice::CanBusError error) const;
    QCanBusDevice *m_canDevice;
};
