#include "gpio.hpp"
#include "spdlog/spdlog.h"
#include <filesystem>
#if __aarch64__
#else
#endif

GPIO::GPIO(QObject *parent) : QObject(parent), m_offset(0)
{
}

GPIO::~GPIO()
{
    if (!m_request.get())
        return;
    m_request->release();
}

void GPIO::configure_pin(Direction dir, uint8_t gpio)
{
    if (m_request.get() != nullptr)
        return;
    std::filesystem::path chip_path("/dev/gpiochip0");
    m_offset = gpio;
    if (dir == Direction::Output)
    {
        try
        {
            m_request = std::make_unique<gpiod::line_request>(
                ::gpiod::chip(chip_path)
                    .prepare_request()
                    .set_consumer("unisim_cpp")
                    .add_line_settings(m_offset,
                                       ::gpiod::line_settings().set_direction(::gpiod::line::direction::OUTPUT))
                    .do_request());
        }
        catch (...)
        {
            SPDLOG_CRITICAL("Could not set gpio {} as output, run binary as root", gpio);
        }
    }
    if (dir == Direction::Input)
    {
        try
        {
            m_request = std::make_unique<gpiod::line_request>(
                ::gpiod::chip(chip_path)
                    .prepare_request()
                    .set_consumer("unisim_cpp")
                    .add_line_settings(m_offset,
                                       ::gpiod::line_settings().set_direction(::gpiod::line::direction::INPUT))
                    .do_request());
        }
        catch (...)
        {
            SPDLOG_CRITICAL("Could not set gpio {} as input, run binary as root", gpio);
        }
    }
}
void GPIO::set_value(bool value)
{
#if defined(__aarch64__) || defined(__x86_64__)
    if (!m_request.get())
        return;
    m_request->set_value(m_offset, (gpiod::line::value)value);
#endif
}

bool GPIO::value()
{
#if defined(__aarch64__) || defined(__x86_64__)
    if (!m_request.get())
        return false;
    return (bool)m_request->get_value(m_offset);
#else
    return 0;
#endif
}
