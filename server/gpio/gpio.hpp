#pragma once
#include <QObject>
#if defined(__linux__) && (defined(__aarch64__) || defined(__x86_64__))
#include <gpiod.hpp>
#else

#endif

/**
 * @brief Class representing a GPIO pin
 *
 */
class GPIO : public QObject
{
    Q_OBJECT
  public:
    /**
     * @brief Enum representing pin direction
     *
     */
    enum Direction
    {
        Input,
        Output
    };
    /**
     * @brief Construct a new GPIO object
     *
     * @param parent The parent QObject
     */
    explicit GPIO(QObject *parent = nullptr);
    /**
     * @brief Destroy the GPIO object
     *
     */
    ~GPIO();

    /**
     * @brief Configure a GPIO pin as input or output
     *
     * @param dir The direction of the pin (Input or Output)
     * @param gpio The GPIO pin number
     */
    void configure_pin(Direction dir, uint8_t gpio);
    /**
     * @brief Set the value of the GPIO pin
     *
     * @param val The value to set (true for high, false for low)
     */
    void set_value(bool val);
    /**
     * @brief Get the value of the GPIO pin
     *
     * @return bool The value of the pin (true for high, false for low)
     */
    bool value();

  private:
#if defined(__linux__) && (defined(__aarch64__) || defined(__x86_64__))
    std::unique_ptr<gpiod::line_request> m_request;
#endif
    unsigned int m_offset;
};
