#include "CAN_interface.hpp"
#include "spdlog/spdlog.h"

CAN_Interface::CAN_Interface(QObject *parent) : QObject(parent), m_canDevice(nullptr)
{
    if (!QCanBus::instance()->plugins().contains(QStringLiteral("socketcan")))
    {
        SPDLOG_CRITICAL("Runtime error: CAN Interface: socketcan plugin not found");
        throw std::runtime_error("CAN Interface: socketcan plugin not found");
    }
}

CAN_Interface::~CAN_Interface()
{
    m_canDevice->disconnectDevice();
    delete m_canDevice;
}

void CAN_Interface::connect_to_dev(QString dev)
{
    QString errorString;
    m_canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), dev, &errorString);
    if (!m_canDevice)
    {
        // Error handling goes here
        SPDLOG_CRITICAL("Runtime error: CAN Interface: {}", errorString.toStdString());
        throw std::runtime_error("CAN Interface: " + errorString.toStdString());
    }

    connect(m_canDevice, &QCanBusDevice::errorOccurred, this,
            [=, this](QCanBusDevice::CanBusError error) { processErrors(error); });

    m_canDevice->connectDevice();
    connect(m_canDevice, &QCanBusDevice::framesReceived, this, [this] {
        for (const QCanBusFrame &frame : m_canDevice->readAllFrames())
        {
            emit frame_received(frame);
        }
    });
}

void CAN_Interface::send_frame(const QCanBusFrame frame)
{
    if (m_canDevice == nullptr)
        return;
    m_canDevice->writeFrame(frame);
    emit frame_send(frame);
}

void CAN_Interface::processErrors(QCanBusDevice::CanBusError error) const
{
    switch (error)
    {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        SPDLOG_CRITICAL("CAN Interface: {} {}", qt_getEnumName(error), m_canDevice->errorString().toStdString());
        break;
    default:
        break;
    }
}
