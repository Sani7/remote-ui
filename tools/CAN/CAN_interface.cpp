#include "CAN_interface.hpp"

CAN_Interface::CAN_Interface(QString dev, QObject *parent)
: QObject(parent)
{
    if (!QCanBus::instance()->plugins().contains(QStringLiteral("socketcan"))) {
        throw std::runtime_error("CAN Interface: socketcan plugin not found");
    }
    QString errorString;
    m_canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), dev, &errorString);
    if (!m_canDevice)
    {
        // Error handling goes here
        throw std::runtime_error("CAN Interface: " + errorString.toStdString());
    }

    m_canDevice->connectDevice();
    connect(m_canDevice, &QCanBusDevice::framesReceived, this, [this] {
        emit frames_received();
    });
}

CAN_Interface::~CAN_Interface()
{
    m_canDevice->disconnectDevice();
    delete m_canDevice;
}

void CAN_Interface::send_frame(const QCanBusFrame &frame)
{
    m_canDevice->writeFrame(frame);
}

QCanBusFrame CAN_Interface::read_frame()
{
    return m_canDevice->readFrame();
}