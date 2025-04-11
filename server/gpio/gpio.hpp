#ifndef GPIO_HPP
#define GPIO_HPP

#include <QObject>
#if defined(__aarch64__) || defined(__x86_64__)
#include <gpiod.hpp>
#else

#endif

class GPIO : public QObject
{
    Q_OBJECT
  public:
    enum Direction
    {
        Input,
        Output
    };
    explicit GPIO(QObject *parent = nullptr);
    ~GPIO();
    void configure_pin(Direction dir, uint8_t gpio);
    void set_value(bool val);
    bool value();

  private:
    std::unique_ptr<gpiod::line_request> m_request;
    unsigned int m_offset;
};

#endif // GPIO_HPP
