#include "CAN_interface.hpp"
#include "spdlog/spdlog.h"
#include "magic_enum.hpp"

CAN_Interface::CAN_Interface(QString dev, QObject *parent)
    : QObject(parent)
{
    if (!QCanBus::instance()->plugins().contains(QStringLiteral("socketcan"))) {
        spdlog::critical("Runtime error: CAN Interface: socketcan plugin not found");
        throw std::runtime_error("CAN Interface: socketcan plugin not found");
    }
    QString errorString;
    m_canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), dev, &errorString);
    if (!m_canDevice)
    {
        // Error handling goes here
        spdlog::critical("Runtime error: CAN Interface: {}", errorString.toStdString());
        throw std::runtime_error("CAN Interface: " + errorString.toStdString());
    }

    connect(m_canDevice, &QCanBusDevice::errorOccurred,
            this, [=, this](QCanBusDevice::CanBusError error){processErrors(error);});

    m_canDevice->connectDevice();
    connect(m_canDevice, &QCanBusDevice::framesReceived, this, [this] {
        for (const QCanBusFrame &frame : m_canDevice->readAllFrames())
        {
            emit frame_received(frame);
        }
    });
}

CAN_Interface::~CAN_Interface()
{
    m_canDevice->disconnectDevice();
    delete m_canDevice;
}

void CAN_Interface::send_frame(const QCanBusFrame frame)
{
    m_canDevice->writeFrame(frame);
    emit frame_send(frame);
}

void CAN_Interface::processErrors(QCanBusDevice::CanBusError error) const
{
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        spdlog::critical("CAN Interface: {} {}", magic_enum::enum_name<QCanBusDevice::CanBusError>(error), m_canDevice->errorString().toStdString());
        break;
    default:
        break;
    }
}
